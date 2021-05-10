{ avr ? true, arm ? true, teensy ? true }:
let
  # We specify sources via Niv: use "niv update nixpkgs" to update nixpkgs, for example.
  sources = import ./nix/sources.nix {};
  pkgs = import sources.nixpkgs {};

  # Some Python packages require patching to make them find the required native
  # libraries in Nix-specific locations.
  pythonOverrides = self: super: {
    hid = super.hid.overridePythonAttrs (old: {
      postPatch =
        ''
          found=
          for name in libhidapi-hidraw libhidapi-libusb libhidapi-iohidmanager libhidapi; do
            full_path=${pkgs.hidapi.out}/lib/$name${pkgs.stdenv.hostPlatform.extensions.sharedLibrary}
            if test -f $full_path; then
              found=t
              sed -i -e "s|'$name\..*'|'$full_path'|" hid/__init__.py
            fi
          done
          test -n "$found" || { echo "ERROR: No known libraries found in ${pkgs.hidapi.out}/lib, please update/fix this build expression."; exit 1; }
        '';
    });
    pyusb = super.pyusb.overridePythonAttrs (old: {
      postPatch =
        ''
          libusb=${pkgs.libusb1.out}/lib/libusb-1.0${pkgs.stdenv.hostPlatform.extensions.sharedLibrary}
          test -f $libusb || { echo "ERROR: $libusb doesn't exist, please update/fix this build expression."; exit 1; }
          sed -i -e "s|find_library=None|find_library=lambda _:\"$libusb\"|" usb/backend/libusb1.py
        '';
    });
  };

  # Builds the python env based on nix/pyproject.toml and
  # nix/poetry.lock Use the "poetry update --lock", "poetry add
  # --lock" etc. in the nix folder to adjust the contents of those
  # files if the requirements*.txt files change

  # "Stage 1" Python environment, which includes all required packages, but the
  # wrapper for the `qmk` executable in this environment will prepend a
  # directory with a wrong `python3` executable to `$PATH`, which will break
  # subsequent calls to `bin/qmk` from the QMK build system (`bin/qmk` uses
  # `#!/usr/bin/env python3` and expects various Python modules to be available
  # in that `python3`).
  pythonEnvStage1 = pkgs.poetry2nix.mkPoetryEnv {
    projectDir = ./nix;
    overrides = pkgs.poetry2nix.overrides.withDefaults pythonOverrides;
  };

  # Final Python environment, which includes the same Python packages, but the
  # wrapper for the `qmk` executable is modified to prepend two more
  # directories to `$PATH`:
  #
  #  1) `${pythonEnvStage1}/bin`, which contains a `python3` executable with a
  #     proper Python library path configuration, so calling `bin/qmk` from
  #     the QMK build system invoked via `qmk` works.
  #
  #  2) `$out/bin` (added before the item 1 above), which contains the modified
  #     `qmk` wrapper itself, so that even if some code in the QMK build system
  #     recursively calls `qmk`, and that recursive invocation ends up calling
  #     `bin/qmk`, the nested `qmk` invocation will run the same modified
  #     wrapper, and not the problematic wrapper from `pythonEnvStage1`.
  #
  # Doing this within a single Python environment does not seem to be feasible,
  # because using `${pythonEnvStage1}/bin` while building `pythonEnvStage1`
  # causes infinite recursion.
  pythonEnv = pkgs.poetry2nix.mkPoetryEnv {
    projectDir = ./nix;
    overrides = pkgs.poetry2nix.overrides.withDefaults (self: super:
      pythonOverrides self super // {
        qmk = super.qmk.overridePythonAttrs (old: {
          makeWrapperArgs = [ "--prefix PATH : $out/bin:${pythonEnvStage1}/bin" ];
        });
      }
    );
  };
in

with pkgs;
let
  avrlibc = pkgsCross.avr.libcCross;

  avr_incflags = [
    "-isystem ${avrlibc}/avr/include"
    "-B${avrlibc}/avr/lib/avr5"
    "-L${avrlibc}/avr/lib/avr5"
    "-B${avrlibc}/avr/lib/avr35"
    "-L${avrlibc}/avr/lib/avr35"
    "-B${avrlibc}/avr/lib/avr51"
    "-L${avrlibc}/avr/lib/avr51"
  ];
in
mkShell {
  name = "qmk-firmware";

  buildInputs = [ clang-tools dfu-programmer dfu-util diffutils git pythonEnv poetry niv ]
    ++ lib.optional avr [
      pkgsCross.avr.buildPackages.binutils
      pkgsCross.avr.buildPackages.gcc8
      avrlibc
      avrdude
    ]
    ++ lib.optional arm [ gcc-arm-embedded ]
    ++ lib.optional teensy [ teensy-loader-cli ];

  AVR_CFLAGS = lib.optional avr avr_incflags;
  AVR_ASFLAGS = lib.optional avr avr_incflags;
  shellHook = ''
    # Prevent the avr-gcc wrapper from picking up host GCC flags
    # like -iframework, which is problematic on Darwin
    unset NIX_CFLAGS_COMPILE_FOR_TARGET
  '';
}
