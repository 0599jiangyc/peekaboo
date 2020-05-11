#! /bin/sh
#
# run.sh
# Copyright (C) 2020 jiangyc <0599jiangyc@gmail.com>
#
# Distributed under terms of the MIT license.
#


echo "run.sh [command]"
peekaboo_dir=`pwd`
dynamorio_dir=/tmp/DynamoRIO-Linux-7.91.18342
[ ! -d "./tracefiles" ] && mkdir tracefiles
cd tracefiles && $dynamorio_dir/bin64/drrun -c $peekaboo_dir/peekaboo_dr/build/libpeekaboo_dr.so -- $1
