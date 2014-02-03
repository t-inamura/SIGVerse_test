#include "CX3DViewpointNode.h"
#include "CX3DParserUtil.h"
#include "CX3DParser.h"
#include "CJNIUtil.h"

/////////////////////////////////////////////////////////
//
//  CX3DViewpointNode

CX3DViewpointNode::CX3DViewpointNode(jobject vrmlNode)
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

		if (nodeName && strcmp(nodeName, "Viewpoint")==0)
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
						if (strcmp(fieldName.c_str(), "fieldOfView") == 0)
						{
							m_fieldOfView.setValue(data);
						}
						else if (strcmp(fieldName.c_str(), "jump") == 0)
						{
							m_jump.setValue(data);
						}
						else if (strcmp(fieldName.c_str(), "retainUserOffsets") == 0)
						{
							m_retainUserOffsets.setValue(data);
						}
						else if (strcmp(fieldName.c_str(), "orientation") == 0)
						{
							m_orientation.setValue(data);
						}
						else if (strcmp(fieldName.c_str(), "position") == 0)
						{
							m_position.setValue(data);
						}
						else if (strcmp(fieldName.c_str(), "centerOfRotation") == 0)
						{
							m_centerOfRotation.setValue(data);
						}
						else if (strcmp(fieldName.c_str(), "description") == 0)
						{
							m_description.setValue(data);
						}

						delete data;
					}
				}
			}
		}
		delete tmpNode;
	}
}

void CX3DViewpointNode::print(int indent)
{
	FILE *fp = CX3DParser::getDebugLogFp();

	char *nodeName = getNodeName();
	if (nodeName)
	{
		float x, y, z, rot;

		CX3DParser::printIndent(indent);
		fprintf(fp, "%s (%s)\n", nodeName, CX3DNode::getNodeTypeString(getNodeType()));

		CX3DParser::printIndent(indent+1);
		fprintf(fp, "fieldOfView : (%f)\n", getFieldOfView()->getValue());

		CX3DParser::printIndent(indent+1);
		fprintf(fp, "jump : %s\n", getJump()->getValue() ? "TRUE" : "FALSE");

		CX3DParser::printIndent(indent+1);
		fprintf(fp, "retainUserOffsets : %s\n", getRetainUserOffsets()->getValue() ? "TRUE" : "FALSE");

		getOrientation()->getValue(x, y, z, rot);
		CX3DParser::printIndent(indent+1);
		fprintf(fp, "orientation : (%f %f %f)(%f)\n", x, y, z, rot);

		getPosition()->getValue(x, y, z);
		CX3DParser::printIndent(indent+1);
		fprintf(fp, "position : (%f %f %f)\n", x, y, z);

		getCenterOfRotation()->getValue(x, y, z);
		CX3DParser::printIndent(indent+1);
		fprintf(fp, "centerOfRotation : (%f %f %f)\n", x, y, z);

		CX3DParser::printIndent(indent+1);
		fprintf(fp, "description : (%s)\n", getDescription()->getValue());
	}
}

