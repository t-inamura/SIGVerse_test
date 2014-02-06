#!/bin/sh
if [ $# -ne 2 ] ;then
    echo "[USAGE] Number of arguments must be two." 1>&2
    echo "[USAGE] ./exec_test.sh testsubject testfile.x3d(.wrl)" 1>&2
    echo "[USAGE] (ex.) $ ./exec_test.sh 0 ../../../shapes/VRML/apple.wrl" 1>&2
    exit 0
fi
#
#if [ ! -d $2 ]; then
#    echo "ERROR: The specified directory does not exist." 1>&2
#    exit 0
#fi

LD_LIBRARY_PATH=/usr/lib/jvm/java-6-openjdk-amd64/jre/lib/amd64/server:$LD_LIBRARY_PATH ./xj3dtest $1 $2
