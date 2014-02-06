@echo off
rem ----------------------------------------------
rem  X3DParser (java) test script
rem ----------------------------------------------
if "%1" == "" (
    echo usage: %0 x3dfile
    echo ^(ex^.^)
    echo %0 VRML/apple.wrl
    echo %0 X3DSamples/list2-1.x3d
) else (
    java -Djava.class.path=.\X3DParser.jar;"C:\Program Files\Xj3D\jars\xj3d-all.jar" X3DParserTest 0 ..\shapes\%1
)

pause
