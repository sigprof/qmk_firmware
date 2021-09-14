"""Functions to convert to and from QMK formats
"""
from collections import OrderedDict
from types import SimpleNamespace
import hjson
import re


kle_row_col_re = re.compile(r'^\((\w+),(\w+)\)$', re.ASCII)
kle_direct_re = re.compile(r'^\[(\w+)\]$', re.ASCII)

def kle_parse_pins_from_name(name, row2col):
    if row2col:
        row_group = 2
        col_group = 1
    else:
        row_group = 1
        col_group = 2
    label_parts = name.split('\n')[1:]
    for label_part in label_parts:
        m = kle_row_col_re.match(label_part)
        if m:
            return SimpleNamespace(direct = False, row = m.group(row_group), col = m.group(col_group))
        m = kle_direct_re.match(label_part)
        if m:
            return SimpleNamespace(direct = True, pin = m.group(1))
    return None

def kle_extract_matrix_pins(kle, row2col = False):
    """Extract matrix pins from KLE data.
    """
    row_pins = []
    col_pins = []
    direct_pins = []
    for row in kle:
        for key in row:
            if key['decal']:
                continue
            if 'name' in key and key['name']:
                pins = kle_parse_pins_from_name(key['name'], row2col)
                if pins:
                    if pins.direct:
                        if pins.pin not in direct_pins:
                            direct_pins.append(pins.pin)
                    else:
                        if pins.col not in col_pins:
                            col_pins.append(pins.col)
                        if pins.row not in row_pins:
                            row_pins.append(pins.row)
    if len(direct_pins) > 0:
        return { 'direct': [ direct_pins ] }
    elif len(col_pins) > 0 and len(row_pins) > 0:
        return { 'cols': col_pins, 'rows': row_pins }
    else:
        return None

def kle2qmk(kle, matrix_pins = None, row2col = False):
    """Convert a KLE layout to QMK's layout format.
    """
    layout = []

    for row in kle:
        for key in row:
            if key['decal']:
                continue

            qmk_key = OrderedDict(
                label="",
                x=key['column'],
                y=key['row'],
            )

            if key['width'] != 1:
                qmk_key['w'] = key['width']
            if key['height'] != 1:
                qmk_key['h'] = key['height']
            if 'name' in key and key['name']:
                qmk_key['label'] = key['name'].split('\n', 1)[0]
                if matrix_pins:
                    pins = kle_parse_pins_from_name(key['name'], row2col)
                    matrix = None
                    if pins:
                        if pins.direct:
                            matrix = [ 0, matrix_pins['direct'].index(pins.pin) ]
                        else:
                            matrix = [ matrix_pins['rows'].index(pins.row), matrix_pins['cols'].index(pins.col) ]
                    if matrix:
                        qmk_key['matrix'] = matrix
            else:
                del (qmk_key['label'])

            layout.append(qmk_key)

    return layout

def kle_raw_to_via(kle_raw_str, matrix_pins, row2col):
    """Convert raw KLE data to VIA JSON with matrix location info.
    """
    # Wrap KLE raw data in a dictionary so hjson will parse it
    kle_json_str = '{"layout": [' + kle_raw_str + ']}'
    kle_rows = hjson.loads(kle_json_str)['layout']

    # Get matrix size
    if 'direct' in matrix_pins:
        matrix_rows = 1
        matrix_cols = len(matrix_pins['direct'])
    else:
        matrix_rows = len(matrix_pins['rows'])
        matrix_cols = len(matrix_pins['cols'])

    # Process raw KLE data, replacing the key labels with matrix locations
    via_keymap = []
    for kle_row in kle_rows:
        via_row = []
        for kle_entry in kle_row:
            if isinstance(kle_entry, str):
                pins = kle_parse_pins_from_name(kle_entry, row2col)
                matrix = None
                if pins:
                    if pins.direct:
                        matrix = [ 0, matrix_pins['direct'].index(pins.pin) ]
                    else:
                        matrix = [ matrix_pins['rows'].index(pins.row), matrix_pins['cols'].index(pins.col) ]
                if matrix:
                    kle_entry = '%d,%d' % tuple(matrix)
            via_row.append(kle_entry)
        via_keymap.append(via_row)

    # Wrap the keymap into the structure required by VIA
    return OrderedDict(
        name = '<TODO>',
        vendorId = '<TODO>',
        productId = '<TODO>',
        lighting = '<TODO>',
        matrix = OrderedDict(rows = matrix_rows, cols = matrix_cols),
        layouts = OrderedDict(keymap = via_keymap)
    )
