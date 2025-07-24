#!/usr/bin/env python3

import os
import sys
import argparse

from litex.build.tools import replace_in_file

def main():
    parser = argparse.ArgumentParser(description="LiteX Bare Metal Software Test")
    parser.add_argument('--build-path', help="Target's build path", required=True)
    parser.add_argument("--with-cxx",   action="store_true", help="Enable CXX support.")
    parser.add_argument("--mem",        default="main_ram",  help="Memory Region where code will be loaded/executed.")
    args = parser.parse_args()

    # Create hello_world directory
    os.makedirs("hello_world", exist_ok=True)

    # Copy contents to hello_world directory
    os.system(f"cp {os.path.abspath(os.path.dirname(__file__))}/* hello_world")
    os.system("chmod -R u+w hello_world") # Nix specific: Allow linker script to be modified.

    # Update memory region.
    replace_in_file("hello_world/linker.ld", "main_ram", args.mem)

    # Compile hello_world
    build_path = args.build_path if os.path.isabs(args.build_path) else os.path.join("..", args.build_path)
    os.system(f"export BUILD_DIR={build_path} && {'export WITH_CXX=1 &&' if args.with_cxx else ''} cd hello_world && make")

    # Copy hello_world.bin
    os.system("cp hello_world/hello_world.bin ./")

    # Prepare flash boot image.
    python3 = sys.executable or "python3" # Nix specific: Reuse current Python executable if available.
    os.system(f"{python3} -m litex.soc.software.crcfbigen hello_world.bin -o hello_world.fbi --fbi --little") # FIXME: Endianness.

if __name__ == "__main__":
    main()

