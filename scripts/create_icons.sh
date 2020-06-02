#!/bin/bash
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

scriptDir="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
projDir="${scriptDir}/../"

incFile="${projDir}/src/ui/icons.h"
srcFile="${projDir}/src/ui/icons.cpp"

cat > "${incFile}" <<EOF
#pragma once

#include <stdint.h>

EOF

cat > "${srcFile}" <<EOF
#include "icons.h"
#include <stdint.h>

EOF

AUTO_GEN_COMMENT="// Auto-generated by $0 - do not edit\n"

echo -en "${AUTO_GEN_COMMENT}" >> "${incFile}"
echo -en "${AUTO_GEN_COMMENT}" >> "${srcFile}"

#${scriptDir}/icon2cpp.py "${projDir}/icons/test.svg" --width 32 -s "${srcFile}" -i "${incFile}"
${scriptDir}/icon2cpp.py "${projDir}/icons/box_clear.png" -s "${srcFile}" -i "${incFile}"
${scriptDir}/icon2cpp.py "${projDir}/icons/box_filled.png" -s "${srcFile}" -i "${incFile}"
${scriptDir}/icon2cpp.py "${projDir}/icons/clock.png" --height 30 -s "${srcFile}" -i "${incFile}"
${scriptDir}/icon2cpp.py "${projDir}/icons/flame.png" --height 30 -s "${srcFile}" -i "${incFile}"
${scriptDir}/icon2cpp.py "${projDir}/icons/thermometer.png" --height 30 -s "${srcFile}" -i "${incFile}"
