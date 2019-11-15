#!/usr/bin/env python3

import argparse
import sys
import os
import subprocess

parser = argparse.ArgumentParser(description="SVG to CPP conversion helper")
parser.add_argument("svgfile", help="SVG source file")
parser.add_argument("-w", "--width", type=int, help="Icon width")
parser.add_argument("-s", "--source", help="Append to source file")
parser.add_argument("-i", "--include", help="Append to include file")
args = parser.parse_args()

icon_name = os.path.basename(args.svgfile)
icon_name = os.path.splitext(icon_name)[0]

# Convert the SVG to PNG

inkscape_cmd = ["inkscape", "-z", "-e", "tmp.png",
                "-w", f"{args.width}", args.svgfile]

if subprocess.run(inkscape_cmd).returncode != 0:
    print("Inkscape command failed!")
    sys.exit(-1)

# Convert the PNG to Portable Anymap (PNM)

imagemagick_cmd = ["convert",
                   "-background", "white",
                   "-alpha", "remove",
                   "-colorspace", "gray",
                   "-auto-level", "-threshold", "50%",
                   "tmp.png", "tmp.pnm"]

if subprocess.run(imagemagick_cmd).returncode != 0:
    print("Imagemagick command failed!")
    sys.exit(-1)

with open("tmp.pnm", 'rb') as f:
    if f.readline().strip() != b"P4":
        print("Intermediate file error (portable bitmap expected!)")
        sys.exit(-1)
    img_dimensions = f.readline().strip().decode('utf-8').split(" ")

    with open(args.include, "a+") as ofile:
        ofile.write(f"#define ICON_{icon_name}_WIDTH {img_dimensions[0]}\n")
        ofile.write(f"#define ICON_{icon_name}_HEIGHT {img_dimensions[1]}\n")
        ofile.write(f"extern const PROGMEM uint8_t ICON_{icon_name}_DATA[];\n\n")

    with open(args.source, "a+") as ofile:
        ofile.write(f"const PROGMEM uint8_t ICON_{icon_name}_DATA[] = " + "{\n")
        img_data = f.read()
        for img_chunk in [img_data[i:i+16] for i in range(0, len(img_data), 16)]:
            line_str = "    " + "".join(['0x%02x,' % val for val in img_chunk]) + "\n"
            ofile.write(line_str)
        ofile.write("};\n\n")

os.remove("tmp.png")
os.remove("tmp.pnm")

print("Ready.")
