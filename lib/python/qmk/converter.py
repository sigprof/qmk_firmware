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

def kle_raw_parse(kle_raw_str):
    """Parse raw KLE data.
    """
    # Wrap KLE raw data in a dictionary so hjson will parse it
    kle_json_str = '{"layout": [' + kle_raw_str + ']}'
    return hjson.loads(kle_json_str)['layout']

def kle_raw_to_via(kle_raw_str, matrix_pins, row2col):
    """Convert raw KLE data to VIA JSON with matrix location info.
    """
    kle_rows = kle_raw_parse(kle_raw_str)

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

kle_issi_led_re = re.compile(r'^(\d+),(\w+)$', re.ASCII)
kle_other_led_re = re.compile(r'^\+(\d+),(R|G|B)$', re.ASCII)

def parse_issi_led_data(label_part):
    m = kle_issi_led_re.match(label_part)
    if m:
        return SimpleNamespace(device = int(m.group(1)), register = m.group(2))
    return None

def parse_other_led_data(label_part):
    m = kle_other_led_re.match(label_part)
    if m:
        return SimpleNamespace(index = int(m.group(1)), color = m.group(2))
    return None

def parse_led_data(label):
    label_parts = label.split('\n')
    if len(label_parts) >= 7:
        # Try to parse data for ISSI LEDs
        data = SimpleNamespace(
            type = 'issi',
            r = parse_issi_led_data(label_parts[2]),
            b = parse_issi_led_data(label_parts[3]),
            g = parse_issi_led_data(label_parts[7])
        )
        if data.r and data.g and data.b:
            return data

        # If ISSI parsing failed, try to parse data for other LEDs
        data = SimpleNamespace(
            type = 'other',
            r = parse_other_led_data(label_parts[2]),
            b = parse_other_led_data(label_parts[3]),
            g = parse_other_led_data(label_parts[7])
        )
        if data.r and data.g and data.b:
            if data.r.index == data.g.index and data.r.index == data.b.index:
                if data.r.color == 'R' and data.g.color == 'G' and data.b.color == 'B':
                    return data

    return None

def key_to_phys_point(key):
    return SimpleNamespace(
        x = key['x'] + key.get('w', 1)/2,
        y = key['y'] + key.get('h', 1)/2
    )

def layout_bounds(layout):
    bounds = None
    for key in layout:
        pp = key_to_phys_point(key)
        if not bounds:
            bounds = SimpleNamespace(x_min = pp.x, x_max = pp.x, y_min = pp.y, y_max = pp.y)
        else:
            bounds.x_min = min(bounds.x_min, pp.x)
            bounds.x_max = max(bounds.x_max, pp.x)
            bounds.y_min = min(bounds.y_min, pp.y)
            bounds.y_max = max(bounds.y_max, pp.y)
    return bounds

def kle_to_rgb_matrix(kle, kb_info_json, layout_name):
    layout = kb_info_json['layouts'][layout_name]['layout']
    bounds = layout_bounds(layout)
    led_x_max = 224
    led_y_max = 64
    if bounds.x_max > bounds.x_min:
        x_offset = -bounds.x_min
        x_factor = int(led_x_max / (bounds.x_max - bounds.x_min) / 4) * 4
    else:
        x_offset = led_x_max / 2 - bounds.x_min
        x_factor = 0
    if bounds.y_max > bounds.y_min:
        y_offset = -bounds.y_min
        y_factor = int(led_y_max / (bounds.y_max - bounds.y_min) / 4) * 4
    else:
        y_offset = led_y_max / 2 - bounds.y_min
        y_factor = 0

    key_matrix_to_led_index = [None] * kb_info_json['matrix_size']['rows']
    for i in range(len(key_matrix_to_led_index)):
        key_matrix_to_led_index[i] = [None] * kb_info_json['matrix_size']['cols']

    led_data_by_index = []

    issi_leds = []

    # Pass 1: collect data for per-key LEDs on ISSI chips.
    for kle_row in kle:
        for key in kle_row:
            if not key['decal']:
                led_data = parse_led_data(key['name'])
                if led_data:
                    if led_data.type == 'issi':
                        issi_leds.append(led_data)

    # Pass 2: collect data for underglow LEDs on ISSI chips.
    for kle_row in kle:
        for key in kle_row:
            if key['decal']:
                led_data = parse_led_data(key['name'])
                if led_data:
                    if led_data.type == 'issi':
                        issi_leds.append(led_data)

    # Pass 3: prepare data for per-key LEDs.

    issi_led_index = 0

    # Initial value is -1, so that it would be incremented to 0 for the first
    # normal key.
    key_index = -1

    for kle_row in kle:
        for key in kle_row:
            if not key['decal']:
                # Update key_index (it should be in sync with the numbering
                # that is used in info.json).
                key_index += 1

                # Determine flags from the keycap color.
                if key['keycap_color'] == '#777777':
                    led_flag = 1 # accent color - use led_flag_modifier
                elif key['keycap_color'] == '#aaaaaa':
                    led_flag = 1 # modifier color - use led_flag_modifier
                else:
                    led_flag = 4 # default is led_flag_keylight

                led_index = None
                led_data = parse_led_data(key['name'])
                if led_data:
                    if led_data.type == 'issi':
                        led_index = issi_led_index
                        issi_led_index += 1
                    elif led_data.type == 'other':
                        led_index = led_data.r.index + len(issi_leds)

                if led_index is not None:
                    key_matrix = layout[key_index]['matrix']
                    key_matrix_to_led_index[key_matrix[0]][key_matrix[1]] = led_index
                    while len(led_data_by_index) <= led_index:
                        led_data_by_index.append(None)
                    pp = SimpleNamespace(x = float(key['column'] + key['width']/2), y = float(key['row'] + key['height']/2))
                    led_point = SimpleNamespace(
                        x = int((pp.x + x_offset) * x_factor),
                        y = int((pp.y + y_offset) * y_factor)
                    )
                    led_data_by_index[led_index] = SimpleNamespace(point = led_point, flag = led_flag)

    # Pass 4: prepare data for underglow LEDs
    for kle_row in kle:
        for key in kle_row:
            if key['decal']:
                led_index = None
                led_data = parse_led_data(key['name'])
                if led_data:
                    if led_data.type == 'issi':
                        led_index = issi_led_index
                        issi_led_index += 1
                    elif led_data.type == 'other':
                        led_index = led_data.r.index + len(issi_leds)

                led_flag = 2 # LED_FLAG_UNDERGLOW

                led_index = None
                led_data = parse_led_data(key['name'])
                if led_data:
                    if led_data.type == 'issi':
                        led_index = issi_led_index
                        issi_led_index += 1
                    elif led_data.type == 'other':
                        led_index = led_data.r.index + len(issi_leds)

                if led_index is not None:
                    while len(led_data_by_index) <= led_index:
                        led_data_by_index.append(None)
                    pp = SimpleNamespace(x = float(key['column'] + key['width']/2), y = float(key['row'] + key['height']/2))
                    led_point = SimpleNamespace(
                        x = int((pp.x + x_offset) * x_factor),
                        y = int((pp.y + y_offset) * y_factor)
                    )
                    led_data_by_index[led_index] = SimpleNamespace(point = led_point, flag = led_flag)

    return SimpleNamespace(
        issi_leds = issi_leds,
        key_matrix_to_led_index = key_matrix_to_led_index,
        led_data_by_index = led_data_by_index
    );
