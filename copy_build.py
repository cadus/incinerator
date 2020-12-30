#!/usr/bin/env python3

from gitversion import version_string
import shutil
import os
import glob

ver_str = version_string()

def postbuild(source, target, env):
    # File name for .bin and dir name for flasher tool
    prog_name = f"incinerator-{ver_str}.bin"
    flasher_name = f"flasher-{ver_str}"

    # Remove old artifacts
    for path in glob.glob(".pio/build/*.bin"):
        os.unlink(path)
    for path in glob.glob(".pio/build/flasher-*"):
        shutil.rmtree(path, ignore_errors=True)

    # Copy firmare.bin to versioned 'progname'
    bin_path = source[0].rstr()
    shutil.copy(bin_path, f".pio/build/{prog_name}")

    # Copy flasher tool and insert current firmware.bin
    flasher_path = f".pio/build/{flasher_name}"
    shutil.copytree("flasher", flasher_path)
    shutil.copy(bin_path, flasher_path)

    gh_env = os.getenv("GITHUB_ENV")
    if gh_env:
        # If we run on GitHub CI, export the binary's name for CI script
        with open(gh_env, "a") as f:
            f.write("BIN_NAME=" + prog_name + "\n")
            f.write("FLASHER_NAME=" + flasher_name + "\n")

if "Import" in globals():
    Import("env")
    print("Current build targets:", list(map(str, BUILD_TARGETS)))
    env.AddPostAction("buildprog", postbuild)
