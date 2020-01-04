#!/usr/bin/env python3

import subprocess
from subprocess import PIPE
import sys
import datetime

git_cmd = [
    "git",
    "--no-pager",
    "describe",
    "--dirty",
    "--always",
    "--tags",
    "--abbrev=8"
]

git_cmd_result = subprocess.run(git_cmd, stdout=PIPE, stderr=PIPE)

if git_cmd_result.returncode != 0:
    print("Git command failed: {git_cmd_result.stderr}")
    sys.exit(-1)

gitversion = git_cmd_result.stdout.strip()
gitversion = gitversion.decode('utf-8')

print(f"Git version: {gitversion}")

builddate = datetime.datetime.utcnow()
builddate = builddate.strftime("%F %T")

print(f"Build date: {builddate}")

with open("src/version.cpp", "w") as ofile:
    ofile.write(f'#include "version.h"\n\n')
    ofile.write(f'const char gitversion[] = "{gitversion}";\n')
    ofile.write(f'const char builddate[] = "{builddate}";\n')
