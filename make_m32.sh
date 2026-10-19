#!/bin/sh
make HOST_FLAGS=-m32 HOST=i386-linux-gnu TARGET_FLAGS=-m32 TARGET=i386-linux-gnu "$@"
