OculusRiftSensor - A demonstration of Oculus Rift in SIGVerse
=============================================================

Overview
========
This package describes how to use the sensor implemented on Oculus
Rift in SIGVerse.  It consists of two modules: a SIGService module
which can receive signals from Oculus Rift and an agent controller to
get messages from SIGViewer.

How to build OculusRiftSensor
=============================
* SIGService module
0. Requirement: SIGService.
1. Open ViewerService/OculusRiftSensor.sln by VS2010.
2. Just click Build -> Build Solution.

Then you can find the OculusRiftSensor.sig file in ViewerService/Debug
or ViewerService/Release.

* Agent controller
0. Requirement: SIGServer.
1. Locate AgentController directory to where you can operate
   SIGServer.
2. Execute make command in the directory.

Then you can find the OculusRiftCtrl.so file.

How to use OculusRiftSensor
===========================
0. Make sure your Oculus Rift can normally be operated by using
   well-examined programs e.g. SDK samples, demo programs distributed
   by Oculus VR, inc.
1. Login to the server ready to operate SIGServer.
2. Execute command: sigserver.sh -w ./OculusRiftCtrl.xml
3. If you confirmed startnig SIGServer, switch to the Windows PC and
   start the SIGViewer.
4. Add SIGService OculusRiftSensor.sig made by the above instruction
   to running SIGViewer by selecting the Service -> Add pop-up menu.
5. Click connect button on SIGViewer.
6. Click Service -> Start -> OculusRiftSensor.sig
7. Click START button to start simulation

