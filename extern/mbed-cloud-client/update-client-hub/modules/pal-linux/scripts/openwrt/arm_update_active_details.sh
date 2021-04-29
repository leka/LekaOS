#!/bin/sh
# ----------------------------------------------------------------------------
# Copyright 2016-2017 ARM Ltd.
#
# SPDX-License-Identifier: Apache-2.0
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http:#www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
# ----------------------------------------------------------------------------

# header name (not prefix)
HEADER_NAME="header"

# header size
HEADER_SIZE=112

# Find mtd partitions
#
# ACTIVE_SLOT_MTD
# ACTIVE_FLAGS_MTD
# UPDATE_SLOT_MTD
# UPDATE_FLAGS_MTD
#
. arm_update_kernel.sh

#
# get active flags
#

# expected location for header
echo "/tmp/${HEADER_NAME}.bin"

# dump to temporary file
nanddump -f "/tmp/${HEADER_NAME}.tmp" "/dev/${ACTIVE_FLAGS_MTD}" 2>/dev/null
VALUE=$?

# exit if return code is not zero
if [ $VALUE -ne 0 ]; then
    exit $VALUE
fi

# truncate file to only contain header
dd if=/tmp/${HEADER_NAME}.tmp of=/tmp/${HEADER_NAME}.bin ibs=1 count=${HEADER_SIZE} 2>/dev/null
VALUE=$?

# exit if return code is not zero
if [ $VALUE -ne 0 ]; then
    exit $VALUE
fi

# clean up
rm -f /tmp/${HEADER_NAME}.tmp 2>/dev/null
VALUE=$?

# exit if return code is not zero
if [ $VALUE -ne 0 ]; then
    exit $VALUE
fi

# success
exit 0
