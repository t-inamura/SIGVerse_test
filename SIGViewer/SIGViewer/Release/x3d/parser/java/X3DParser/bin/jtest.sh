# ----------------------------------------------
#  X3DParser (java) test script
# ----------------------------------------------

if [ "$#" -ne 1 ]
then
    echo "usage: $0 x3dfile"
    echo "(ex)"
    echo "$0 VRML/apple.wrl"
    echo "$0 X3DSamples/list2-1.x3d"
    exit 1
fi

java -Djava.class.path=/prj/irwas/sekikawa/irwas-sim-090410/jar/X3DParser.jar:/prj/irwas/sekikawa/irwas-sim-090410/jar/xj3d-all.jar X3DParserTest 0 ../../../../shapes/$1
