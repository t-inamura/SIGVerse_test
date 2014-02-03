#include "CX3DTextureTransformNode.h"
#include "CX3DParserUtil.h"
#include "CX3DParser.h"
#include "CJNIUtil.h"

/////////////////////////////////////////////////////////
//
//  CX3DTextureTransformNode

CX3DTextureTransformNode::CX3DTextureTransformNode(jobject vrmlNode)
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

		if (nodeName && strcmp(nodeName, "TextureTransform")==0)
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
						else if (strcmp(fieldName.c_str(), "translation") == 0)
						{
							m_translation.setValue(data);
						}

						delete data;
					}
				}
			}
		}
		delete tmpNode;
	}
}

void CX3DTextureTransformNode::print(int indent)
{
	FILE *fp = CX3DParser::getDebugLogFp();

	char *nodeName = getNodeName();
	if (nodeName)
	{
		float x, y;

		CX3DParser::printIndent(indent);
		fprintf(fp, "%s (%s)\n", nodeName, CX3DNode::getNodeTypeString(getNodeType()));

		getCenter()->getValue(x, y);
		CX3DParser::printIndent(indent+1);
		fprintf(fp, "center : (%f %f)\n", x, y);

		CX3DParser::printIndent(indent+1);
		fprintf(fp, "rotation : %f\n", getRotation()->getValue());

		getScale()->getValue(x, y);
		CX3DParser::printIndent(indent+1);
		fprintf(fp, "scale : (%f %f)\n", x, y);

		getTranslation()->getValue(x, y);
		CX3DParser::printIndent(indent+1);
		fprintf(fp, "translation : (%f %f)\n", x, y);
	}
}

