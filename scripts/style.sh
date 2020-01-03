#!/bin/bash

scriptDir="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
projDir="${scriptDir}/../"

astyle -A3 -s4 -p -xg -n -r -j "src/*.cpp" "src/*.h"
