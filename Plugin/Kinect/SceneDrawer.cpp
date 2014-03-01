/****************************************************************************
*                                                                           *
*  OpenNI 1.x Alpha                                                         *
*  Copyright (C) 2011 PrimeSense Ltd.                                       *
*                                                                           *
*  This file is part of OpenNI.                                             *
*                                                                           *
*  OpenNI is free software: you can redistribute it and/or modify           *
*  it under the terms of the GNU Lesser General Public License as published *
*  by the Free Software Foundation, either version 3 of the License, or     *
*  (at your option) any later version.                                      *
*                                                                           *
*  OpenNI is distributed in the hope that it will be useful,                *
*  but WITHOUT ANY WARRANTY; without even the implied warranty of           *
*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the             *
*  GNU Lesser General Public License for more details.                      *
*                                                                           *
*  You should have received a copy of the GNU Lesser General Public License *
*  along with OpenNI. If not, see <http://www.gnu.org/licenses/>.           *
*                                   






                                        *
****************************************************************************/
//---------------------------------------------------------------------------
// Includes
//---------------------------------------------------------------------------
#include "SceneDrawer.h"

//for sigverse
#include "SIGService.h"
#include <iostream>
#include <map>
#include <math.h>
#include <tchar.h>

using namespace std;
//using namespace boost;


enum SigVec
{
	HIP = 0,
	HTOTOR,
	WAIST,
	RSHOULDER,
	LSHOULDER,
	RELBOW,
	LELBOW,
	LEG
};

struct Quaternion {
	float qw;
	float qx;
	float qy;
	float qz;
};

typedef map<string, Quaternion> QMap;

#ifndef USE_GLES
#if (XN_PLATFORM == XN_PLATFORM_MACOSX)
	#include <GLUT/glut.h>
#else
	#include <GL/glut.h>
#endif
#else
	#include "opengles.h"
#endif

extern xn::UserGenerator g_UserGenerator;
extern xn::DepthGenerator g_DepthGenerator;

extern XnBool g_bDrawBackground;
extern XnBool g_bDrawPixels;
extern XnBool g_bDrawSkeleton;
extern XnBool g_bPrintID;
extern XnBool g_bPrintState;

#include <map>



std::map<XnUInt32, std::pair<XnCalibrationStatus, XnPoseDetectionStatus> > m_Errors;
void XN_CALLBACK_TYPE MyCalibrationInProgress(xn::SkeletonCapability& capability, XnUserID id, XnCalibrationStatus calibrationError, void* pCookie)
{
	m_Errors[id].first = calibrationError;
}
void XN_CALLBACK_TYPE MyPoseInProgress(xn::PoseDetectionCapability& capability, const XnChar* strPose, XnUserID id, XnPoseDetectionStatus poseError, void* pCookie)
{
	m_Errors[id].second = poseError;
}

#define MAX_DEPTH 10000
float g_pDepthHist[MAX_DEPTH];
unsigned int getClosestPowerOfTwo(unsigned int n)
{
	unsigned int m = 2;
	while(m < n) m<<=1;

	return m;
}


GLuint initTexture(void** buf, int& width, int& height)
{
	GLuint texID = 0;
	glGenTextures(1,&texID);

	width = getClosestPowerOfTwo(width);
	height = getClosestPowerOfTwo(height); 
	*buf = new unsigned char[width*height*4];
	glBindTexture(GL_TEXTURE_2D,texID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	return texID;
}


GLfloat texcoords[8];
void DrawRectangle(float topLeftX, float topLeftY, float bottomRightX, float bottomRightY)
{
	GLfloat verts[8] = {
		topLeftX,     topLeftY,
		topLeftX,     bottomRightY,
		bottomRightX, bottomRightY,
		bottomRightX, topLeftY
	};
	glVertexPointer(2, GL_FLOAT, 0, verts);
	glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

	//TODO: Maybe glFinish needed here instead - if there's some bad graphics crap
	glFlush();
}


void DrawTexture(float topLeftX, float topLeftY, float bottomRightX, float bottomRightY)
{
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glTexCoordPointer(2, GL_FLOAT, 0, texcoords);

	DrawRectangle(topLeftX, topLeftY, bottomRightX, bottomRightY);

	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
}


XnFloat Colors[][3] =
{
	{0,1,1},
	{0,0,1},
	{0,1,0},
	{1,1,0},
	{1,0,0},
	{1,.5,0},
	{.5,1,0},
	{0,.5,1},
	{.5,0,1},
	{1,1,.5},
	{1,1,1}
};
XnUInt32 nColors = 10;
#ifndef USE_GLES
void glPrintString(void *font, char *str)
{
	int i,l = strlen(str);

	for(i=0; i<l; i++)
	{
		glutBitmapCharacter(font,*str++);
	}
}
#endif
void DrawLimb(XnUserID player, XnSkeletonJoint eJoint1, XnSkeletonJoint eJoint2)
{
	if (!g_UserGenerator.GetSkeletonCap().IsTracking(player))
	{
		printf("not tracked!\n");
		return;
	}

	XnSkeletonJointPosition joint1, joint2;
	g_UserGenerator.GetSkeletonCap().GetSkeletonJointPosition(player, eJoint1, joint1);
	g_UserGenerator.GetSkeletonCap().GetSkeletonJointPosition(player, eJoint2, joint2);

	if (joint1.fConfidence < 0.5 || joint2.fConfidence < 0.5)
	{
		return;
	}

	XnPoint3D pt[2];
	pt[0] = joint1.position;
	pt[1] = joint2.position;

	g_DepthGenerator.ConvertRealWorldToProjective(2, pt, pt);
#ifndef USE_GLES
	glVertex3i(pt[0].X, pt[0].Y, 0);
	glVertex3i(pt[1].X, pt[1].Y, 0);
#else
	GLfloat verts[4] = {pt[0].X, pt[0].Y, pt[1].X, pt[1].Y};
	glVertexPointer(2, GL_FLOAT, 0, verts);
	glDrawArrays(GL_LINES, 0, 2);
	glFlush();
#endif
}

bool DiffVec(XnPoint3D &rvec, XnSkeletonJointPosition jvec, XnSkeletonJointPosition kvec)
{
	if(jvec.fConfidence < 0.5 || kvec.fConfidence < 0.5)
	{
		return false;
	}

	rvec.X = -(kvec.position.X - jvec.position.X);
	rvec.Y = kvec.position.Y - jvec.position.Y;
	rvec.Z = -(kvec.position.Z - jvec.position.Z);

	double length = sqrt(rvec.X*rvec.X + rvec.Y*rvec.Y + rvec.Z*rvec.Z);
	rvec.X = rvec.X/length;
	rvec.Y = rvec.Y/length;
	rvec.Z = rvec.Z/length;
	return true;
}

Quaternion CalcQuaternion(XnPoint3D kvec, XnPoint3D svec)
{
	Quaternion q;
	if(kvec.X == svec.X && kvec.Y == svec.Y && kvec.Z == svec.Z)
	{
		q.qw = 1; q.qx = 0; q.qy = 0; q.qz = 0;
		return q;
	}

	double x = kvec.Y*svec.Z - kvec.Z*svec.Y;
	double y = kvec.Z*svec.X - kvec.X*svec.Z;
	double z = kvec.X*svec.Y - kvec.Y*svec.X;

	double sum = sqrt(x*x+y*y+z*z);
	x = x/sum;
	y = y/sum;
	z = z/sum;

	double angle = acos(kvec.X*svec.X+kvec.Y*svec.Y+kvec.Z*svec.Z);
	q.qw = cos(angle/2);
	q.qx = x*sin(angle/2);
	q.qy = y*sin(angle/2);
	q.qz = z*sin(angle/2);

	return q;
}

void RotVec(XnPoint3D &v, Quaternion q)
{
	double rx = v.X *  q.qw + v.Y * -q.qz + v.Z *  q.qy; 
	double ry = v.X *  q.qz + v.Y *  q.qw + v.Z * -q.qx;
	double rz = v.X * -q.qy + v.Y *  q.qx + v.Z *  q.qw;
	double rw = v.X *  q.qx + v.Y *  q.qy + v.Z *  q.qz;

	v.X = q.qx *  rw + q.qy *  rz + q.qz * -ry + q.qw * rx;
	v.Y = q.qx * -rz + q.qy *  rw + q.qz *  rx + q.qw * ry;
	v.Z = q.qx *  ry + q.qy * -rx + q.qz *  rw + q.qw * rz;
}

XnPoint3D GetSigVec(int sigvec)
{
	XnPoint3D p;
	if(sigvec == WAIST || sigvec == HTOTOR)
	{
		p.X = 0;
		p.Y = 1;
		p.Z = 0;
	}
	else if(sigvec == RSHOULDER || sigvec == RELBOW)
	{
		p.X = -1;
		p.Y =  0;
		p.Z =  0;
	}
	else if(sigvec == HIP || sigvec == LSHOULDER || sigvec == LELBOW)
	{
		p.X =  1;
		p.Y =  0;
		p.Z =  0;
	}
	else if(sigvec == LEG)
	{
		p.X =  0;
		p.Y = -1;
		p.Z =  0;
	}
	return p;
}

string FloatToString(float x)
{
	char tmp[32];
	sprintf(tmp,"%f",x);
	string str;
	str = string(tmp);
	return str;
}

string GetStringFromQuaternion( string jname, Quaternion q)
{
	string qw = FloatToString(q.qw);
	string qx = FloatToString(q.qx);
	string qy = FloatToString(q.qy);
	string qz = FloatToString(q.qz);
	string s = jname + ":" + qw + "," + qx + "," + qy + "," + qz;

	return s;
}

Quaternion MultiQuaternion(Quaternion p, Quaternion q)
{
	Quaternion r;
    r.qw = p.qw * q.qw - p.qx * q.qx - p.qy * q.qy - p.qz * q.qz;
    r.qx = p.qw * q.qx + p.qx * q.qw + p.qy * q.qz - p.qz * q.qy;
    r.qy = p.qw * q.qy - p.qx * q.qz + p.qy * q.qw + p.qz * q.qx;
    r.qz = p.qw * q.qz + p.qx * q.qy - p.qy * q.qx + p.qz * q.qw;

	return r;
}

const XnChar* GetCalibrationErrorString(XnCalibrationStatus error)
{
	switch (error)
	{
	case XN_CALIBRATION_STATUS_OK:
		return "OK";
	case XN_CALIBRATION_STATUS_NO_USER:
		return "NoUser";
	case XN_CALIBRATION_STATUS_ARM:
		return "Arm";
	case XN_CALIBRATION_STATUS_LEG:
		return "Leg";
	case XN_CALIBRATION_STATUS_HEAD:
		return "Head";
	case XN_CALIBRATION_STATUS_TORSO:
		return "Torso";
	case XN_CALIBRATION_STATUS_TOP_FOV:
		return "Top FOV";
	case XN_CALIBRATION_STATUS_SIDE_FOV:
		return "Side FOV";
	case XN_CALIBRATION_STATUS_POSE:
		return "Pose";
	default:
		return "Unknown";
	}
}
const XnChar* GetPoseErrorString(XnPoseDetectionStatus error)
{
	switch (error)
	{
	case XN_POSE_DETECTION_STATUS_OK:
		return "OK";
	case XN_POSE_DETECTION_STATUS_NO_USER:
		return "NoUser";
	case XN_POSE_DETECTION_STATUS_TOP_FOV:
		return "Top FOV";
	case XN_POSE_DETECTION_STATUS_SIDE_FOV:
		return "Side FOV";
	case XN_POSE_DETECTION_STATUS_ERROR:
		return "General error";
	default:
		return "Unknown";
	}
}


void DrawDepthMap(const xn::DepthMetaData& dmd, const xn::SceneMetaData& smd, sigverse::SIGService *srv)
{
	static bool bInitialized = false;	
	static GLuint depthTexID;
	static unsigned char* pDepthTexBuf;
	static int texWidth, texHeight;

	//added for sigverse
	static char saddr[128];
	static int port;
	static int speed;
	static double move_speed;
	static int cnt;
	static bool start;
	static XnPoint3D startpos;

	float topLeftX;
	float topLeftY;
	float bottomRightY;
	float bottomRightX;
	float texXpos;
	float texYpos;

	if(!bInitialized)
	{
		start = false;

		TCHAR spd[8];
		GetPrivateProfileString(_T("SETTING"),_T("SEND_SPEED"),'\0', spd, 256, _T("./SIGNiUserTracker.ini"));
		if(spd[0] == '\0') {
			speed = 1;
		}
		else {
			speed = atoi((char*)spd);
		}

		GetPrivateProfileString(_T("SETTING"),_T("MOVE_SPEED"),'\0', spd, 256, _T("./SIGNiUserTracker.ini"));
		if(spd[0] == '\0') {
			move_speed = 0.1;
		}
		else {
			move_speed = atof((char*)spd);
		}

		texWidth =  getClosestPowerOfTwo(dmd.XRes());
		texHeight = getClosestPowerOfTwo(dmd.YRes());

//		printf("Initializing depth texture: width = %d, height = %d\n", texWidth, texHeight);
		depthTexID = initTexture((void**)&pDepthTexBuf,texWidth, texHeight) ;

//		printf("Initialized depth texture: width = %d, height = %d\n", texWidth, texHeight);
		bInitialized = true;

		topLeftX = dmd.XRes();
		topLeftY = 0;
		bottomRightY = dmd.YRes();
		bottomRightX = 0;
		texXpos =(float)dmd.XRes()/texWidth;
		texYpos  =(float)dmd.YRes()/texHeight;

		memset(texcoords, 0, 8*sizeof(float)); //TODO: Magic number
		texcoords[0] = texXpos, texcoords[1] = texYpos, texcoords[2] = texXpos, texcoords[7] = texYpos;
	}

	unsigned int nValue = 0;
	unsigned int nHistValue = 0;
	unsigned int nIndex = 0;
	unsigned int nX = 0;
	unsigned int nY = 0;
	unsigned int nNumberOfPoints = 0;
	XnUInt16 g_nXRes = dmd.XRes();
	XnUInt16 g_nYRes = dmd.YRes();

	unsigned char* pDestImage = pDepthTexBuf;

	const XnDepthPixel* pDepth = dmd.Data();
	const XnLabel* pLabels = smd.Data();

	// Calculate the accumulative histogram
	memset(g_pDepthHist, 0, MAX_DEPTH*sizeof(float));
	for (nY=0; nY<g_nYRes; nY++)
	{
		for (nX=0; nX<g_nXRes; nX++)
		{
			nValue = *pDepth;

			if (nValue != 0)
			{
				g_pDepthHist[nValue]++;
				nNumberOfPoints++;
			}

			pDepth++;
		}
	}

	for (nIndex=1; nIndex<MAX_DEPTH; nIndex++)
	{
		g_pDepthHist[nIndex] += g_pDepthHist[nIndex-1];
	}
	if (nNumberOfPoints)
	{
		for (nIndex=1; nIndex<MAX_DEPTH; nIndex++)
		{
			g_pDepthHist[nIndex] = (unsigned int)(256 * (1.0f - (g_pDepthHist[nIndex] / nNumberOfPoints)));
		}
	}

	pDepth = dmd.Data();
	if (g_bDrawPixels)
	{
		XnUInt32 nIndex = 0;
		// Prepare the texture map
		for (nY=0; nY<g_nYRes; nY++)
		{
			for (nX=0; nX < g_nXRes; nX++, nIndex++)
			{

				pDestImage[0] = 0;
				pDestImage[1] = 0;
				pDestImage[2] = 0;
				if (g_bDrawBackground || *pLabels != 0)
				{
					nValue = *pDepth;
					XnLabel label = *pLabels;
					XnUInt32 nColorID = label % nColors;
					if (label == 0)
					{
						nColorID = nColors;
					}

					if (nValue != 0)
					{
						nHistValue = g_pDepthHist[nValue];

						pDestImage[0] = nHistValue * Colors[nColorID][0]; 
						pDestImage[1] = nHistValue * Colors[nColorID][1];
						pDestImage[2] = nHistValue * Colors[nColorID][2];
					}
				}

				pDepth++;
				pLabels++;
				pDestImage+=3;
			}

			pDestImage += (texWidth - g_nXRes) *3;
		}
	}
	else
	{
		xnOSMemSet(pDepthTexBuf, 0, 3*2*g_nXRes*g_nYRes);
	}

	glBindTexture(GL_TEXTURE_2D, depthTexID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texWidth, texHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, pDepthTexBuf);

	// Display the OpenGL texture map
	glColor4f(0.75,0.75,0.75,1);

	glEnable(GL_TEXTURE_2D);
	DrawTexture(dmd.XRes(),dmd.YRes(),0,0);	
	glDisable(GL_TEXTURE_2D);

	char strLabel[50] = "";
	XnUserID aUsers[15];
	XnUInt16 nUsers = 15;
	g_UserGenerator.GetUsers(aUsers, nUsers);

	srv->checkRecvData(1);
	for (int i = 0; i < nUsers; ++i){
#ifndef USE_GLES
		if (g_bPrintID)
		{

			XnPoint3D com;
			g_UserGenerator.GetCoM(aUsers[i], com);
			g_DepthGenerator.ConvertRealWorldToProjective(1, &com, &com);

			xnOSMemSet(strLabel, 0, sizeof(strLabel));
			if (!g_bPrintState)
			{
				// Tracking
				sprintf(strLabel, "%d", aUsers[i]);
			}
			else if (g_UserGenerator.GetSkeletonCap().IsTracking(aUsers[i]))
			{
				// Tracking
				sprintf(strLabel, "%d - Tracking", aUsers[i]);
			}
			else if (g_UserGenerator.GetSkeletonCap().IsCalibrating(aUsers[i]))
			{
				// Calibrating
				sprintf(strLabel, "%d - Calibrating [%s]", aUsers[i], GetCalibrationErrorString(m_Errors[aUsers[i]].first));
			}
			else
			{
				// Nothing
				sprintf(strLabel, "%d - Looking for pose [%s]", aUsers[i], GetPoseErrorString(m_Errors[aUsers[i]].second));
			}


			glColor4f(1-Colors[i%nColors][0], 1-Colors[i%nColors][1], 1-Colors[i%nColors][2], 1);

			glRasterPos2i(com.X, com.Y);
			glPrintString(GLUT_BITMAP_HELVETICA_18, strLabel);
		}

#endif

		if (g_bDrawSkeleton && g_UserGenerator.GetSkeletonCap().IsTracking(aUsers[i])){
#ifndef USE_GLES
			glBegin(GL_LINES);
#endif

			glColor4f(1-Colors[aUsers[i]%nColors][0], 1-Colors[aUsers[i]%nColors][1], 1-Colors[aUsers[i]%nColors][2], 1);
			DrawLimb(aUsers[i], XN_SKEL_HEAD,           XN_SKEL_NECK);

			DrawLimb(aUsers[i], XN_SKEL_NECK,           XN_SKEL_LEFT_SHOULDER);
			DrawLimb(aUsers[i], XN_SKEL_LEFT_SHOULDER,  XN_SKEL_LEFT_ELBOW);
			DrawLimb(aUsers[i], XN_SKEL_LEFT_ELBOW,     XN_SKEL_LEFT_HAND);

			DrawLimb(aUsers[i], XN_SKEL_NECK,           XN_SKEL_RIGHT_SHOULDER);
			DrawLimb(aUsers[i], XN_SKEL_RIGHT_SHOULDER, XN_SKEL_RIGHT_ELBOW);
			DrawLimb(aUsers[i], XN_SKEL_RIGHT_ELBOW,    XN_SKEL_RIGHT_HAND);

			DrawLimb(aUsers[i], XN_SKEL_LEFT_SHOULDER,  XN_SKEL_TORSO);
			DrawLimb(aUsers[i], XN_SKEL_RIGHT_SHOULDER, XN_SKEL_TORSO);

			DrawLimb(aUsers[i], XN_SKEL_TORSO,          XN_SKEL_LEFT_HIP);
			DrawLimb(aUsers[i], XN_SKEL_LEFT_HIP,       XN_SKEL_LEFT_KNEE);
			DrawLimb(aUsers[i], XN_SKEL_LEFT_KNEE,      XN_SKEL_LEFT_FOOT);

			DrawLimb(aUsers[i], XN_SKEL_TORSO,          XN_SKEL_RIGHT_HIP);
			DrawLimb(aUsers[i], XN_SKEL_RIGHT_HIP,      XN_SKEL_RIGHT_KNEE);
			DrawLimb(aUsers[i], XN_SKEL_RIGHT_KNEE,     XN_SKEL_RIGHT_FOOT);

			DrawLimb(aUsers[i], XN_SKEL_LEFT_HIP,       XN_SKEL_RIGHT_HIP);


			if(i == 0){
				if(cnt < 0) cnt = speed;
				if(cnt == 0){
					XnSkeletonJointPosition torso, lhip, rhip, neck, lshoul,
						rshoul, lelb, relb, lknee, rknee, lhand, rhand, lfoot, rfoot, head;
					g_UserGenerator.GetSkeletonCap().GetSkeletonJointPosition(aUsers[0], XN_SKEL_TORSO, torso);
					g_UserGenerator.GetSkeletonCap().GetSkeletonJointPosition(aUsers[0], XN_SKEL_LEFT_HIP, lhip);
					g_UserGenerator.GetSkeletonCap().GetSkeletonJointPosition(aUsers[0], XN_SKEL_RIGHT_HIP, rhip);
					g_UserGenerator.GetSkeletonCap().GetSkeletonJointPosition(aUsers[0], XN_SKEL_NECK, neck);
					g_UserGenerator.GetSkeletonCap().GetSkeletonJointPosition(aUsers[0], XN_SKEL_HEAD, head);
					g_UserGenerator.GetSkeletonCap().GetSkeletonJointPosition(aUsers[0], XN_SKEL_LEFT_SHOULDER, lshoul);
					g_UserGenerator.GetSkeletonCap().GetSkeletonJointPosition(aUsers[0], XN_SKEL_RIGHT_SHOULDER, rshoul);
					g_UserGenerator.GetSkeletonCap().GetSkeletonJointPosition(aUsers[0], XN_SKEL_LEFT_ELBOW, lelb);
					g_UserGenerator.GetSkeletonCap().GetSkeletonJointPosition(aUsers[0], XN_SKEL_RIGHT_ELBOW, relb);
					g_UserGenerator.GetSkeletonCap().GetSkeletonJointPosition(aUsers[0], XN_SKEL_LEFT_HAND, lhand);
					g_UserGenerator.GetSkeletonCap().GetSkeletonJointPosition(aUsers[0], XN_SKEL_RIGHT_HAND, rhand);
					g_UserGenerator.GetSkeletonCap().GetSkeletonJointPosition(aUsers[0], XN_SKEL_LEFT_KNEE, lknee);
					g_UserGenerator.GetSkeletonCap().GetSkeletonJointPosition(aUsers[0], XN_SKEL_RIGHT_KNEE, rknee);
					g_UserGenerator.GetSkeletonCap().GetSkeletonJointPosition(aUsers[0], XN_SKEL_LEFT_FOOT, lfoot);
					g_UserGenerator.GetSkeletonCap().GetSkeletonJointPosition(aUsers[0], XN_SKEL_RIGHT_FOOT, rfoot);

					if(start == false)
					{
						startpos.X = torso.position.X;
						startpos.Y = torso.position.Y;
						startpos.Z = torso.position.Z;
						start = true;
					}

					XnPoint3D pos;
					pos.X = -(torso.position.X - startpos.X);
					pos.Y = torso.position.Y - startpos.Y;
					pos.Z = -(torso.position.Z - startpos.Z);

					QMap mq; 

					XnPoint3D khip_vec;
					if(DiffVec(khip_vec, rhip, lhip)){
						mq.insert(QMap::value_type("WAIST",CalcQuaternion(GetSigVec(HIP),khip_vec)));
						Quaternion rrootq = CalcQuaternion(khip_vec, GetSigVec(HIP));


						XnPoint3D kneck_vec;
						if(DiffVec(kneck_vec, torso, neck))
						{
							RotVec(kneck_vec,rrootq);

							XnPoint3D sneck_vec = GetSigVec(WAIST);
							mq.insert(QMap::value_type("WAIST_JOINT1",CalcQuaternion(sneck_vec,kneck_vec)));
							Quaternion rwaist= CalcQuaternion(kneck_vec, sneck_vec); 

							XnPoint3D khead_vec;
							if(DiffVec(khead_vec, neck,head))
							{
								RotVec(khead_vec, rrootq);
								RotVec(khead_vec, rwaist);

								mq.insert(QMap::value_type("HEAD_JOINT1",CalcQuaternion(sneck_vec,khead_vec)));
							}


							XnPoint3D krsh_vec;
							if(DiffVec(krsh_vec, rshoul,relb))
							{
								XnPoint3D srsh_vec = GetSigVec(RSHOULDER);

								RotVec(krsh_vec, rrootq);
								RotVec(krsh_vec, rwaist);

								mq.insert(QMap::value_type("RARM_JOINT2",CalcQuaternion(srsh_vec,krsh_vec)));
								Quaternion rrsh = CalcQuaternion(krsh_vec, srsh_vec); 

								XnPoint3D krel_vec;
								if(DiffVec(krel_vec, relb, rhand))
								{
									RotVec(krel_vec, rrootq);
									RotVec(krel_vec, rwaist);
									RotVec(krel_vec, rrsh);

									mq.insert(QMap::value_type("RARM_JOINT3",CalcQuaternion(srsh_vec, krel_vec)));
								}
							}

							XnPoint3D klsh_vec;
							if(DiffVec(klsh_vec, lshoul, lelb))
							{
								XnPoint3D slsh_vec = GetSigVec(LSHOULDER);

								RotVec(klsh_vec, rrootq);
								RotVec(klsh_vec, rwaist);

								mq.insert(QMap::value_type("LARM_JOINT2",CalcQuaternion(slsh_vec, klsh_vec)));
								Quaternion rlsh = CalcQuaternion(klsh_vec, slsh_vec); 
								//////////////////////////////////////////////


								XnPoint3D klel_vec; 
								if(DiffVec(klel_vec, lelb, lhand))
								{

									RotVec(klel_vec, rrootq);
									RotVec(klel_vec, rwaist);
									RotVec(klel_vec, rlsh);

									mq.insert(QMap::value_type("LARM_JOINT3",CalcQuaternion(slsh_vec, klel_vec)));
								}
							}
						}

						XnPoint3D sleg_vec = GetSigVec(LEG);
						XnPoint3D krhip_vec;
						if(DiffVec(krhip_vec, rhip, rknee))
						{

							RotVec(krhip_vec, rrootq);

							mq.insert(QMap::value_type("RLEG_JOINT2",CalcQuaternion(sleg_vec, krhip_vec)));
							Quaternion rrhp = CalcQuaternion(krhip_vec, sleg_vec);

							XnPoint3D krknee_vec;
							if(DiffVec(krknee_vec, rknee, rfoot))
							{

								RotVec(krknee_vec, rrootq);
								RotVec(krknee_vec, rrhp);

								mq.insert(QMap::value_type("RLEG_JOINT4",CalcQuaternion(sleg_vec, krknee_vec)));
							}
						}

						XnPoint3D klhip_vec;
						if(DiffVec(klhip_vec, lhip, lknee)){

							RotVec(klhip_vec, rrootq);

							mq.insert(QMap::value_type("LLEG_JOINT2",CalcQuaternion(sleg_vec, klhip_vec)));
							Quaternion rlhp = CalcQuaternion(klhip_vec, sleg_vec); 

							XnPoint3D klknee_vec; 
							if(DiffVec(klknee_vec, lknee, lfoot))
							{

								RotVec(klknee_vec, rrootq);
								RotVec(klknee_vec, rlhp);

								mq.insert(QMap::value_type("LLEG_JOINT4",CalcQuaternion(sleg_vec, klknee_vec)));
							}
						}
						int connectedNum = srv->getConnectedControllerNum();

						if(connectedNum > 0){

							string posx = FloatToString(pos.X*move_speed);
							string posy = FloatToString(pos.Y*move_speed);
							string posz = FloatToString(pos.Z*move_speed);
							string pos_ = "POSITION";
							pos_ += ":" + posx + "," + posy + "," + posz;


							string st = "KINECT_DATA ";
							st += pos_;
							QMap::iterator it = mq.begin();

							while(it != mq.end())
							{
								st += " ";
								st += (GetStringFromQuaternion((*it).first.c_str(),(*it).second));
								it++;
							}
							st += " END:";

							std::vector<std::string> names = srv->getAllConnectedEntitiesName();
							for(int i = 0; i < connectedNum; i++){
								srv->sendMsgToCtr(names[i].c_str(), st);
							}
						}

					} // if(DiffVec(
					cnt = speed;
			   } //if(cnt == 0)
			   cnt--;
		  }
#ifndef USE_GLES
glEnd();
#endif
	   }
   }
}

