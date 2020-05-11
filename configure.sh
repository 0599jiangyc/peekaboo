#! /bin/sh
#
# configure.sh
# Copyright (C) 2020 jiangyc <0599jiangyc@gmail.com>
#
# Distributed under terms of the MIT license.
#

peekaboo_dir=`pwd`
cd $peekaboo_dir/libpeekaboo & make
cd /tmp && wget https://github.com/DynamoRIO/dynamorio/releases/download/release_7.91.18342/DynamoRIO-Linux-7.91.18342.tar.gz && tar -xvf DynamoRIO-Linux-7.91.18342.tar.gz && cd DynamoRIO-Linux-7.91.18342 && dynamorio_dir=`pwd`
cd $peekaboo_dir/peekaboo_dr && mkdir build && cd build && DynamoRIO_DIR=$dynamorio_dir/bin64 cmake .. && make
