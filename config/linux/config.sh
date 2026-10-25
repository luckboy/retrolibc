#!/bin/sh
#
# Copyright (c) 2026 Łukasz Szpakowski
#
# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at https://mozilla.org/MPL/2.0/.
#
TYPE="$1"
KERNEL_VERSION="$2"

if [ x"$KERNEL_VERSION" = x"" ]; then
    KERNEL_VERSION="`uname -r | cut -d - -f 1`"
fi

KERNEL_VERSION_MAJOR="`echo "$KERNEL_VERSION" | cut -d . -f 1`"
KERNEL_VERSION_PATCHLEVEL="`echo "$KERNEL_VERSION" | cut -d . -f 2`"
KERNEL_VERSION_SUBLEVEL="`echo "$KERNEL_VERSION" | cut -d . -f 3`"
if [ "$KERNEL_VERSION_SUBLEVEL" -gt 255 ]; then
    KERNEL_VERSION_SUBLEVEL=255
fi

if [ x"$TYPE" = xtarget ]; then
	echo "#ifndef _BITS_CONFIG_H"
	echo "#define _BITS_CONFIG_H"
    echo "#define __LINUX_VERSION_CODE `expr $KERNEL_VERSION_MAJOR \* 65536 + $KERNEL_VERSION_PATCHLEVEL \* 256 + $KERNEL_VERSION_SUBLEVEL`"
    echo "#define __KERNEL_VERSION(a, b, c) (((a) << 16) + ((b) << 8) + ((c) > 255 ? 255 : (c)))"
    echo "#endif"
else
	echo "#ifndef _RETROLIBC_CONFIG_H"
	echo "#define _RETROLIBC_CONFIG_H"
    echo "#define RETROLIBC_LINUX_VERSION_CODE `expr $KERNEL_VERSION_MAJOR \* 65536 + $KERNEL_VERSION_PATCHLEVEL \* 256 + $KERNEL_VERSION_SUBLEVEL`"
    echo "#define RETROLIBC_KERNEL_VERSION(a, b, c) (((a) << 16) + ((b) << 8) + ((c) > 255 ? 255 : (c)))"
    echo "#endif"
fi
