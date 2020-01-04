#!/bin/bash

scriptDir="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
projDir="${scriptDir}/../"

find "${projDir}/src" -regex '.*\.\(cpp\|hpp\|c\|h\)' -exec clang-format -style=file -i {} \;
