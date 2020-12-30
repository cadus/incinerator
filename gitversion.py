#!/usr/bin/env python3

import subprocess
from subprocess import PIPE
import sys
import datetime
import re

def version_string():
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
    
    return ver_txt

ver_txt = version_string()

builddate = datetime.datetime.utcnow()
builddate = builddate.strftime("%F %T")

print(f"Build date: {builddate}")
with open("src/version.cpp", "w") as ofile:
    ofile.write(f'#include "version.h"\n\n')
    ofile.write(f'const char gitversion[] = "{ver_txt}";\n')
    ofile.write(f'const char builddate[] = "{builddate}";\n')
