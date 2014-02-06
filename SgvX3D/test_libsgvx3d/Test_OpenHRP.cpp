#include "CX3DParser.h"
#include "Test.h"

// ===============================================================
// ===============================================================

void printOpenHRPStructure(CX3DParser& parser, char *vrmlFile)
{
	// ---------------------------------------------
	// ---------------------------------------------
	if (!parser.parse(vrmlFile))
	{
		fprintf(stderr, "%s parse failed\n", vrmlFile);
		return;
	}

	CX3DParser::printLog("**** OpenHRP field value of (%s) ****\n", vrmlFile);

	// -----------------------------------------
	//
	// -----------------------------------------
	MFNode *pHumanoidNodes = parser.searchNodesFromAllChildrenOfRoot("Humanoid");
	if (pHumanoidNodes)
	{
		for (int i=0; i<pHumanoidNodes->count(); i++)
		{
			CX3DOpenHRPHumanoidNode *pHumanoid = (CX3DOpenHRPHumanoidNode *)(pHumanoidNodes->getNode(i));

			if (pHumanoid)
			{
				// -----------------------------------------
				// -----------------------------------------
				MFNode *humanoidBody = pHumanoid->getHumanoidBody();

				// -----------------------------------------
				// -----------------------------------------
				int nBody = humanoidBody->count();

				for (int i=0; i<nBody; i++)
				{
					// -----------------------------------------
					// -----------------------------------------
					CX3DNode *pNode = humanoidBody->getNode(i);
					if (pNode)
					{
						// -----------------------------------------
						// -----------------------------------------
						switch (pNode->getNodeType())
						{
						case OPENHRP_JOINT_NODE:
							{
								// -----------------------------------------
								// -----------------------------------------
								CX3DOpenHRPJointNode *pJointNode = (CX3DOpenHRPJointNode *)pNode;
								printOpenHRPJointNode(pJointNode, 0);
							}
							break;
						case OPENHRP_SEGMENT_NODE:
							{
								// -----------------------------------------
								// -----------------------------------------
								CX3DOpenHRPSegmentNode *pSegNode = (CX3DOpenHRPSegmentNode *)pNode;
								printOpenHRPSegmentNode(pSegNode, 0);
							}
							break;
						}
					}
				}
			}
		}

		delete pHumanoidNodes;
		pHumanoidNodes = NULL;
	}
}

// ===============================================================
// ===============================================================
void printOpenHRPJointNode(CX3DOpenHRPJointNode *pJointNode, int indent)
{
	// -----------------------------------------
	// -----------------------------------------
	SFString *name = pJointNode->getName();

	// -----------------------------------------
	// -----------------------------------------
	SFVec3f *trans = pJointNode->getTranslation();

	// -----------------------------------------
	// -----------------------------------------
	SFRotation *rot = pJointNode->getRotation();

	// -----------------------------------------
	// -----------------------------------------
	SFString *jointType = pJointNode->getJointType();

	// -----------------------------------------
	//
	// -----------------------------------------
	MFNode *children = pJointNode->getChildren();

	CX3DParser::printIndent(indent);
	CX3DParser::printLog("name: (%s)\n", name->getValue());

	CX3DParser::printIndent(indent);
	CX3DParser::printLog("type: (%s)\n", jointType->getValue());

	CX3DParser::printIndent(indent);
	CX3DParser::printLog("translation: (%f, %f, %f)\n", trans->x(), trans->y(), trans->z());

	CX3DParser::printIndent(indent);
	CX3DParser::printLog("rotation:    (%f, %f, %f, %f)\n", rot->x(), rot->y(), rot->z(), rot->rot());

	// -------------------------------------------
	// -------------------------------------------
	int n = children->count();

	for (int i=0; i<n; i++)
	{
		// -------------------------------------------
		// -------------------------------------------
		CX3DNode *pNode = children->getNode(i);

		// -------------------------------------------
		// -------------------------------------------
		switch (pNode->getNodeType())
		{
		case OPENHRP_JOINT_NODE:
			{
				// -------------------------------------------
				// -------------------------------------------
				CX3DOpenHRPJointNode *pJointNode = (CX3DOpenHRPJointNode *)pNode;
				printOpenHRPJointNode(pJointNode, indent+1);
			}
			break;

		case OPENHRP_SEGMENT_NODE:
			{
				// -------------------------------------------
				// -------------------------------------------
				CX3DOpenHRPSegmentNode *pSegNode = (CX3DOpenHRPSegmentNode *)pNode;
				printOpenHRPSegmentNode(pSegNode, indent+1);
			}
			break;
		}
	}
}

// ===============================================================
// ===============================================================
void printOpenHRPSegmentNode(CX3DOpenHRPSegmentNode *pSegmentNode, int indent)
{
	// -----------------------------------------
	// -----------------------------------------
	SFString *name = pSegmentNode->getName();

	CX3DParser::printIndent(indent);
	CX3DParser::printLog("name: (%s)\n", name->getValue());

	// -----------------------------------------
	//
	//
	// -----------------------------------------
	MFNode *children = pSegmentNode->getChildren();

	int n = children->count();
	for (int i=0; i<n; i++)
	{
		CX3DNode *pNode = children->getNode(i);
		switch (pNode->getNodeType())
		{
		case SHAPE_NODE:
			{
				CX3DShapeNode *pShape = (CX3DShapeNode *)pNode;
				pShape->print(indent);
			}
			break;
		}
	}
}

