"""Convert raw KLE with RGB Matrix annotations to the RGB Matrix configuration code.
"""
import json
import os
from pathlib import Path

from argcomplete.completers import FilesCompleter
from milc import cli
from kle2xy import KLE2xy

from qmk.info import info_json
from qmk.keyboard import keyboard_completer, keyboard_folder
from qmk.converter import kle2qmk, kle_to_rgb_matrix
from qmk.path import is_keyboard

RGB_MATRIX_CONFIG_FILENAME = 'rgb_matrix_config.c'

@cli.argument('-kb', '--keyboard', type=keyboard_folder, completer=keyboard_completer, required=True, help=f'Keyboard to generate {RGB_MATRIX_CONFIG_FILENAME} for.')
@cli.argument('-l', '--layout', help='Layout name (required if the keyboard has multiple layouts).')
@cli.argument('-f', '--force', action='store_true', help=f'Overwrite {RGB_MATRIX_CONFIG_FILENAME} if it exists.')
@cli.argument('filename', completer=FilesCompleter('.json'), help='The KLE raw txt to convert.')
@cli.subcommand('Convert a KLE layout to RGB Matrix configuration code', hidden=False if cli.config.user.developer else True)
def kle2rgbmatrix(cli):
    """Convert the RGB Matrix configuration data from KLE to the C source code for QMK.
    """
    # If filename is a path
    if cli.args.filename.startswith("/") or cli.args.filename.startswith("./"):
        file_path = Path(cli.args.filename)
    # Otherwise assume it is a file name
    else:
        file_path = Path(os.environ['ORIG_CWD'], cli.args.filename)
    # Check for valid file_path for more graceful failure
    if not file_path.exists():
        cli.log.error('File {fg_cyan}%s{style_reset_all} was not found.', file_path)
        return False
    out_path = file_path.parent
    raw_code = file_path.read_text(encoding='utf-8')

    # Determine the keyboard
    if not is_keyboard(cli.args.keyboard):
        cli.log.error('Invalid keyboard: "%s"', cli.args.keyboard)
        return False

    # Build the info.json file
    kb_info_json = info_json(cli.args.keyboard)

    # Find the layout
    layouts = kb_info_json['layouts']
    if cli.args.layout:
        if cli.args.layout in layouts:
            layout_name = cli.args.layout
        else:
            cli.log.error('Layout does not exist: "%s"', cli.args.layout)
            return False
    else:
        if len(layouts) > 1:
            cli.log.error('The "%s" keyboard has multiple layouts; specify one with the "--layout" option.', cli.args.keyboard)
            return False
        else:
            layout_name = list(layouts)[0]

    # Check if the output file exists, allow overwrite with force
    if Path(out_path, RGB_MATRIX_CONFIG_FILENAME).exists() and not cli.args.force:
        cli.log.error('File {fg_cyan}%s/%s{style_reset_all} already exists, use -f or --force to overwrite.', out_path, RGB_MATRIX_CONFIG_FILENAME)
        return False

    try:
        # Convert KLE raw to x/y coordinates (using kle2xy package from skullydazed)
        kle = KLE2xy(raw_code)
    except Exception as e:
        cli.log.error('Could not parse KLE raw data: %s', raw_code)
        cli.log.exception(e)
        return False

    # Convert parsed KLE data to RGB Matrix data
    rgb_matrix_data = kle_to_rgb_matrix(kle, kb_info_json, layout_name)

    # Wrap the generated code into the appropriate ifdef
    code = '#ifdef RGB_MATRIX_ENABLE\n\n'

    if rgb_matrix_data.issi_leds:
        # Generate the list of LEDs for ISSI drivers
        code += 'const is31_led __flash g_is31_leds[DRIVER_LED_TOTAL] = {\n'
        led_index = -1
        for issi_led in rgb_matrix_data.issi_leds:
            led_index = led_index + 1
            device = issi_led.r.device
            if issi_led.g.device != device or issi_led.b.device != device:
                # Connecting different color parts of the same RGB LED to
                # different LED controller chips may be possible if 6-pin LEDs
                # are used, but the current driver code does not support that.
                cli.log.error('ISSI LED %d uses different devices for R/G/B components; this is not supported.', led_index)
            code += '    { %d, %s, %s, %s },\n' % (device, issi_led.r.register, issi_led.g.register, issi_led.b.register)
        code += "};\n\n"

    # Generate g_led_config
    code += 'led_config_t g_led_config = {\n'

    # Write g_led_config.matrix_co
    code += '    {\n'
    for row in rgb_matrix_data.key_matrix_to_led_index:
        code += '        { '
        for led_index in row:
            if led_index is None:
                code += 'NO_LED'
            else:
                code += '%6d' % led_index
            code += ', '
        code += '},\n'
    code += '    },\n'

    # Write g_led_config.point
    code += '    {'
    last_y = None
    for led in rgb_matrix_data.led_data_by_index:
        if (last_y is None) or (abs(last_y - led.point.y) > 4):
            # Add a line break when the y coordinate changes (with a
            # normal-looking layout this would correspond to a different
            # keyboard row).  However, allow some limited y coordinate changes
            # to account for some 65%/75% layouts where cursor keys have a
            # 0.25u vertical offset.
            code += '\n       ' # one more space will be added below
            last_y = led.point.y
        code += ' {%3d,%3d},' % (led.point.x, led.point.y)
    code += '\n    },\n'

    # Write g_led_config.flags
    code += '    {'
    last_y = None
    for led in rgb_matrix_data.led_data_by_index:
        if (last_y is None) or (abs(last_y - led.point.y) > 4):
            code += '\n       ' # one more space will be added below
            last_y = led.point.y
        code += ' %d,' % led.flag
    code += '\n    },\n'

    # Finish g_led_config
    code += '};\n\n'

    # Close the initial #ifdef
    code += '#endif\n'

    # Write the generated C code
    out_file = out_path / RGB_MATRIX_CONFIG_FILENAME
    out_file.write_text(code)
    cli.log.info('Wrote out {fg_cyan}%s/%s', out_path, RGB_MATRIX_CONFIG_FILENAME)
