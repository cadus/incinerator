#!/usr/bin/env python3

from gitversion import version_string
import shutil
import os

def postbuild(source, target, env):
    # Copy firmare.bin to versioned 'progname'
    bin_name = source[0].rstr()
    shutil.copy(bin_name, f".pio/build/{progname}")

    gh_env = os.getenv("GITHUB_ENV")
    if gh_env:
        # If we run on GitHub CI, export the binary's name for CI script
        with open(gh_env, "a") as f:
            f.write("BIN_NAME=" + progname + "\n")


progname = f"incinerator-{version_string()}.bin"
if "Import" in globals():
    Import("env")
    print("Current build targets:", list(map(str, BUILD_TARGETS)))
    env.AddPostAction("buildprog", postbuild)
