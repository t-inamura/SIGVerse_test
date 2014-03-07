#include "CX3DHAnimHumanoidNode.h"
#include "CX3DParserUtil.h"
#include "CX3DParser.h"
#include "CJNIUtil.h"
#include <string.h>

/////////////////////////////////////////////////////////
//
//  CX3DHAnimHumanoidNode

CX3DHAnimHumanoidNode::CX3DHAnimHumanoidNode(jobject vrmlNode)
{
	if (!vrmlNode)
	{
		m_vrmlNode = NULL;
		return;
	}

	CJNIUtil *ju = CJNIUtil::getUtil();

	if (!ju->isInstanceOfVRMLNode(vrmlNode))
	{
		fprintf(stderr, "vrmlNode is not instance of VRMLNode [%s:%d]\n", __FILE__, __LINE__);
		exit(1);
	}

	m_vrmlNode = ju->env()->NewGlobalRef(vrmlNode);
	
	CX3DNode *tmpNode = new CX3DNode(vrmlNode);
	if (tmpNode)
	{
		char *nodeName = tmpNode->getNodeName();

		if (nodeName && strcmp(nodeName, "Humanoid")==0)
		{
			int n = tmpNode->countFields();
			for (int i=0; i<n; i++)
			{
				char *pFieldName = tmpNode->getFieldName(i);
				if (pFieldName)
				{
					std::string fieldName = pFieldName;

					CVRMLFieldData *data = tmpNode->createFieldValue((char *)(fieldName.c_str()));
					if (data)
					{
						if (strcmp(fieldName.c_str(), "center") == 0)
						{
							m_center.setValue(data);
						}
						else if (strcmp(fieldName.c_str(), "rotation") == 0)
						{
							m_rotation.setValue(data);
						}
						else if (strcmp(fieldName.c_str(), "scale") == 0)
						{
							m_scale.setValue(data);
						}
						else if (strcmp(fieldName.c_str(), "scaleOrientation") == 0)
						{
							m_scaleOrientation.setValue(data);
						}
						else if (strcmp(fieldName.c_str(), "translation") == 0)
						{
							m_translation.setValue(data);
						}
						else if (strcmp(fieldName.c_str(), "name") == 0)
						{
							m_name.setValue(data);
						}
						else if (strcmp(fieldName.c_str(), "info") == 0)
						{
							m_info.setValue(data);
						}
						else if (strcmp(fieldName.c_str(), "joints") == 0)
						{
							m_joints.setValue(data);
						}
						else if (strcmp(fieldName.c_str(), "segments") == 0)
						{
							m_segments.setValue(data);
						}
						else if (strcmp(fieldName.c_str(), "sites") == 0)
						{
							m_sites.setValue(data);
						}
						else if (strcmp(fieldName.c_str(), "skeleton") == 0)
						{
							m_skeleton.setValue(data);
						}
						else if (strcmp(fieldName.c_str(), "skin") == 0)
						{
							m_skin.setValue(data);
						}
						else if (strcmp(fieldName.c_str(), "skinCoord") == 0)
						{
							m_skinCoord.setValue(data);
						}
						else if (strcmp(fieldName.c_str(), "skinNormal") == 0)
						{
							m_skinNormal.setValue(data);
						}
						else if (strcmp(fieldName.c_str(), "version") == 0)
						{
							m_version.setValue(data);
						}
						else if (strcmp(fieldName.c_str(), "viewpoints") == 0)
						{
							m_viewpoints.setValue(data);
						}

						delete data;
					}
				}
			}
		}
		delete tmpNode;
	}
}

void CX3DHAnimHumanoidNode::print(int indent)
{
	FILE *fp = CX3DParser::getDebugLogFp();
	int nMax = CX3DParser::getMaxPrintElemsForMFField();
	bool bPartialPrint = false;

	char *nodeName = getNodeName();
	if (nodeName)
	{
		float x, y, z, rot;
		MFNode *nodes;
		int i, n;

		CX3DParser::printIndent(indent);
		fprintf(fp, "%s (%s)\n", nodeName, CX3DNode::getNodeTypeString(getNodeType()));

		CX3DParser::printIndent(indent+1);
		getCenter()->getValue(x, y, z);
		fprintf(fp, "center : (%f %f %f)\n", x, y, z);

		CX3DParser::printIndent(indent+1);
		getRotation()->getValue(x, y, z, rot);
		fprintf(fp, "rotation : (%f %f %f)(%f)\n", x, y, z, rot);

		CX3DParser::printIndent(indent+1);
		getScale()->getValue(x, y, z);
		fprintf(fp, "scale : (%f %f %f)\n", x, y, z);

		CX3DParser::printIndent(indent+1);
		getScaleOrientation()->getValue(x, y, z, rot);
		fprintf(fp, "scaleOrientation : (%f %f %f)(%f)\n", x, y, z, rot);

		CX3DParser::printIndent(indent+1);
		getTranslation()->getValue(x, y, z);
		fprintf(fp, "translation : (%f %f %f)\n", x, y, z);

		CX3DParser::printIndent(indent+1);
		fprintf(fp, "name (%s)\n", m_name.getValue());

		CX3DParser::printIndent(indent+1);
		n = m_info.count();
		fprintf(fp, "info [%d]\n", n);
		if ((nMax > 0) && (n > nMax)) { n = nMax; bPartialPrint = true; }
		else { bPartialPrint = false; }
		for (i=0; i<n; i++)
		{
			CX3DParser::printIndent(indent+2);
			fprintf(fp, "%s\n", m_info.getValue(i));
		}
		if (bPartialPrint)
		{
			CX3DParser::printIndent(indent+2);
			fprintf(fp, "...\n");
		}

		CX3DParser::printIndent(indent+1);
		nodes = getJoints();
		n = nodes->count();
		fprintf(fp, "joints [%d]\n", n);
		for (i=0; i<n; i++)
		{
			CX3DNode *child = nodes->getNode(i);
			if (child)
			{
				child->print(indent+2);
			}
		}

		CX3DParser::printIndent(indent+1);
		nodes = getSegments();
		n = nodes->count();
		fprintf(fp, "segments [%d]\n", n);
		for (i=0; i<n; i++)
		{
			CX3DNode *child = nodes->getNode(i);
			if (child)
			{
				child->print(indent+2);
			}
		}

		CX3DParser::printIndent(indent+1);
		nodes = getSites();
		n = nodes->count();
		fprintf(fp, "sites [%d]\n", n);
		for (i=0; i<n; i++)
		{
			CX3DNode *child = nodes->getNode(i);
			if (child)
			{
				child->print(indent+2);
			}
		}

		CX3DParser::printIndent(indent+1);
		nodes = getSkeleton();
		n = nodes->count();
		fprintf(fp, "skeleton [%d]\n", n);
		for (i=0; i<n; i++)
		{
			CX3DNode *child = nodes->getNode(i);
			if (child)
			{
				child->print(indent+2);
			}
		}

		CX3DParser::printIndent(indent+1);
		nodes = getSkin();
		n = nodes->count();
		fprintf(fp, "skin [%d]\n", n);
		for (i=0; i<n; i++)
		{
			CX3DNode *child = nodes->getNode(i);
			if (child)
			{
				child->print(indent+2);
			}
		}

		CX3DParser::printIndent(indent+1);
		fprintf(fp, "skinCoord\n");
		CX3DNode *pSkinCoord = getSkinCoord()->getNode();
		if (pSkinCoord) pSkinCoord->print(indent+2);

		CX3DParser::printIndent(indent+1);
		fprintf(fp, "skinNormal\n");
		CX3DNode *pSkinNormal = getSkinNormal()->getNode();
		if (pSkinNormal) pSkinNormal->print(indent+2);

		CX3DParser::printIndent(indent+1);
		fprintf(fp, "version (%s)\n", m_version.getValue());

		CX3DParser::printIndent(indent+1);
		nodes = getViewpoints();
		n = nodes->count();
		fprintf(fp, "viewpoints [%d]\n", n);
		for (i=0; i<n; i++)
		{
			CX3DNode *child = nodes->getNode(i);
			if (child)
			{
				child->print(indent+2);
			}
		}
	}
}

