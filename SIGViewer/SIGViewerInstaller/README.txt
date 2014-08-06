***Making of SIGViewerInstaller***

Download and install Inno Setup(isetup-5.5.4.exe).
http://www.jrsoftware.org/isdl.php

If you have installed Inno Setup,
double click SIGViewer.iss

AppVerName is application name and version number.
DefaultDirName is name of directory made.
In [Files] section, it is designation of copying files.

To correct directory name which will be made,
select [Tools] -> [Generate GUID],
so that AppId is generated at first line of editor.
Insert the ID to AppId.

To make setup.exe, select [Build] -> [Compile].

