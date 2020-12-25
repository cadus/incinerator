#!/usr/bin/env python3
#
# SPDX-License-Identifier: GPL-3.0-or-later
# This file is part of CADUS Incinerator, https://github.com/cadus/incinerator
# Copyright (C) 2019-2020 Patrick Huesmann <info@patrick-huesmann.de>
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <https://www.gnu.org/licenses/>.
#

import argparse
import sys
import os
import subprocess

parser = argparse.ArgumentParser(description="SVG/PNG to CPP conversion helper")
parser.add_argument("gfxfile", help="SVG/PNG source file")
parser.add_argument("--width", type=int, help="Icon width")
parser.add_argument("--height", type=int, help="Icon height")
parser.add_argument("-s", "--source", help="Append to source file")
parser.add_argument("-i", "--include", help="Append to include file")
args = parser.parse_args()

icon_name_base = os.path.splitext(os.path.basename(args.gfxfile))
icon_name_ext = icon_name_base[1].lower()
icon_name = icon_name_base[0]

print(f"Converting {icon_name} ...")

if icon_name_ext == ".svg":
    # Convert the SVG to PNG first

    inkscape_cmd = ["inkscape", "-z", "-e", "tmp.png",
                    "-w", f"{args.width}", args.gfxfile]

    if subprocess.run(inkscape_cmd).returncode != 0:
        print("Inkscape command failed!")
        sys.exit(-1)
    
    png_name = "tmp.png"
else:
    png_name = args.gfxfile

# Convert the PNG to Portable Anymap (PNM)

imagemagick_cmd = ["convert"]
if args.width or args.height:
    w_arg = args.width or ""
    h_arg = args.height or ""
    imagemagick_cmd += ["-resize", f"{w_arg}x{h_arg}"]

imagemagick_cmd += ["-background", "white",
                    "-alpha", "remove",
                    "-colorspace", "gray",
                    "-auto-level", "-threshold", "50%",
                    png_name, "tmp.pnm"]

if subprocess.run(imagemagick_cmd).returncode != 0:
    print("Imagemagick command failed!")
    sys.exit(-1)

# Convert PNM to source code

with open("tmp.pnm", 'rb') as f:
    if f.readline().strip() != b"P4":
        print("Intermediate file error (portable bitmap expected!)")
        sys.exit(-1)
    img_dimensions = f.readline().strip().decode('utf-8').split(" ")

    with open(args.include, "a+") as ofile:
        ofile.write(f"extern const Icon icon_{icon_name};\n")

    with open(args.source, "a+") as ofile:
        ofile.write(f"\n")
        ofile.write(f"const Icon icon_{icon_name}" + " {\n")
        ofile.write(f"    {img_dimensions[0]}, {img_dimensions[1]}," + " {\n")
        img_data = f.read()
        out_str = ""
        for img_chunk in [img_data[i:i+16] for i in range(0, len(img_data), 16)]:
            line_str = "    " * 2 + " ".join(['0x%02x,' % val for val in img_chunk]) + "\n"
            out_str += line_str
        ofile.write(out_str[:-2]) # discard last comma
        ofile.write("\n    }\n};\n")

if png_name == "tmp.png":
    os.remove("tmp.png")
os.remove("tmp.pnm")

print("Ready.")
