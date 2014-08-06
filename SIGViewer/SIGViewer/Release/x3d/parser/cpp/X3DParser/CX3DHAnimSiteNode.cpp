#include "CX3DHAnimSiteNode.h"
#include "CX3DParserUtil.h"
#include "CX3DParser.h"
#include "CJNIUtil.h"

/////////////////////////////////////////////////////////
//
//  CX3DHAnimSiteNode

CX3DHAnimSiteNode::CX3DHAnimSiteNode(jobject vrmlNode)
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

		if (nodeName && strcmp(nodeName, "Site")==0)
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
						if (strcmp(fieldName.c_str(), "children") == 0)
						{
							m_children.setValue(data);
						}

						delete data;
					}
				}
			}
		}
		delete tmpNode;
	}
}

void CX3DHAnimSiteNode::print(int indent)
{
	FILE *fp = CX3DParser::getDebugLogFp();

	char *nodeName = getNodeName();
	if (nodeName)
	{
		float x, y, z, rot;
		int i, n;

		CX3DParser::printIndent(indent);
		fprintf(fp, "%s (%s)\n", nodeName, CX3DNode::getNodeTypeString(getNodeType()));

		CX3DParser::printIndent(indent+1);
		fprintf(fp, "children\n");
		MFNode *nodes = getChildren();
		n = nodes->count();
		for (i=0; i<n; i++)
		{
			CX3DNode *child = nodes->getNode(i);
			if (child)
			{
				child->print(indent+2);
			}
		}

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
	}
}

