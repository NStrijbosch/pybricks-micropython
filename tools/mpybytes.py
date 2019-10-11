#!/usr/bin/env python3
import os
import mpy_cross
import argparse
import tempfile


def get_bytes_from_file(path, fake_name=None):
    """Compile a Python file with mpy-cross and return as list of bytes."""

    # Cross-compile Python file to .mpy
    mpy_path = os.path.splitext(path)[0] + '.mpy'
    proc = mpy_cross.run(path, '-mno-unicode', '-o', mpy_path)
    proc.wait()

    # Read the .mpy file
    with open(mpy_path, 'rb') as mpy:
        contents = mpy.read()

    # Override the module name if requested (else it is /path/to/script.py)
    if fake_name:
        bin_path = bytes(path.encode('utf-8'))
        fake_path = bytes(fake_name.encode('utf-8'))
        contents = contents.replace(bin_path, fake_path)

    # Remove the temporary .mpy file and return the contents
    os.remove(mpy_path)
    return [int(c) for c in contents]


def get_bytes_from_str(string):
    """Compile a Python command with mpy-cross and return as list of bytes."""

    # Write Python command to a temporary file and convert as a regular script.
    with tempfile.NamedTemporaryFile(mode='w', suffix='.py') as f:
        f.write(string + '\n')
        return get_bytes_from_file(f.name, fake_name='script.py')


if __name__ == "__main__":
    parser = argparse.ArgumentParser(description='Convert Python scripts or commands to .mpy byte streams.')
    group = parser.add_mutually_exclusive_group(required=True)
    group.add_argument('--file', dest='file', nargs='?', const=1, type=str)
    group.add_argument('--string', dest='string', nargs='?', const=1, type=str)
    args = parser.parse_args()

    if args.file:
        print(get_bytes_from_file(args.file))

    if args.string:
        print(get_bytes_from_str(args.string))
