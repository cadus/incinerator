#!/usr/bin/env python3

import subprocess
from subprocess import PIPE
import sys
import os
import datetime
import re
import shutil

def postbuild(source, target, env):
    # Copy firmare.bin to versioned 'progname'
    bin_name = source[0].rstr()
    shutil.copy(bin_name, f".pio/build/{progname}")

    gh_env = os.getenv("GITHUB_ENV")
    if gh_env:
        # If we run on GitHub CI, export the binary's name for CI script
        with open(gh_env, "a") as f:
            f.write("BIN_NAME=" + progname + "\n")

git_cmd = [
    "git",
    "--no-pager", "describe",
    "--dirty",
    "--always",
    "--tags",
    "--match=[vV]*.*",
    "--abbrev=8",
    "--long"
]
re_git_ver = re.compile(r"^[vV]0*([0-9]+).0*([0-9]+)-([0-9]+)-g([0-9a-f]+)-?(dirty)?$")

git_cmd_result = subprocess.run(git_cmd, stdout=PIPE, stderr=PIPE)

if git_cmd_result.returncode != 0:
    print("Git command failed: {git_cmd_result.stderr}")
    sys.exit(-1)

gitversion = git_cmd_result.stdout.strip()
gitversion = gitversion.decode('utf-8')

print(f"Git version: {gitversion}")

match = re_git_ver.match(gitversion)
if not match:
    print(f"Cannot parse Git version ({gitversion})")
    sys.exit(-1)

ver_major, ver_minor, num_commits_since_tag, commit_hash, dirty_flag = match.groups()
ver_major = int(ver_major)
ver_minor = int(ver_minor)
num_commits_since_tag = int(num_commits_since_tag)
ver_txt = f"V{ver_major}.{ver_minor:02d}"
if num_commits_since_tag:
    ver_txt += f"-{num_commits_since_tag}"
if dirty_flag:
    ver_txt += f"-{dirty_flag}"

builddate = datetime.datetime.utcnow()
builddate = builddate.strftime("%F %T")

print(f"Build date: {builddate}")
with open("src/version.cpp", "w") as ofile:
    ofile.write(f'#include "version.h"\n\n')
    ofile.write(f'const char gitversion[] = "{ver_txt}";\n')
    ofile.write(f'const char builddate[] = "{builddate}";\n')

progname = f"incinerator-{ver_txt}.bin"
if "Import" in globals():
    Import("env")
    print("Current build targets:", list(map(str, BUILD_TARGETS)))
    env.AddPostAction("buildprog", postbuild)
