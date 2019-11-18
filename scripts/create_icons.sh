#!/bin/bash

scriptDir="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
projDir="${scriptDir}/../"

incFile="${projDir}/include/icons.h"
srcFile="${projDir}/src/icons.cpp"

cat > "${incFile}" <<EOF
#pragma once
#include "avr/pgmspace.h"

EOF

cat > "${srcFile}" <<EOF
#include "icons.h"
#include <stdint.h>

EOF

${scriptDir}/icon2cpp.py "${projDir}/icons/test.svg" -w 32 -s "${srcFile}" -i "${incFile}"
${scriptDir}/icon2cpp.py "${projDir}/icons/blub.png" -s "${srcFile}" -i "${incFile}"
