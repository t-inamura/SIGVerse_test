#include "CX3DParser.h"
#include "Test.h"

// ==========================================
// ==========================================

void Test_printFieldsOfBoxNode(CX3DParser& parser, char *vrmlFile)
{
	// ---------------------------------------------
	// ---------------------------------------------
	if (!parser.parse(vrmlFile))
	{
		fprintf(stderr, "%s parse failed\n", vrmlFile);
		return;
	}

	CX3DParser::printLog("**** Fields of BoxNode ****\n");

	// ---------------------------------------------
	// ---------------------------------------------
	MFNode *pShapeNodes = parser.searchNodesFromAllChildrenOfRoot("Shape");
	if (pShapeNodes)
	{
		for (int i=0; i<pShapeNodes->count(); i++)
		{
			CX3DShapeNode *pShape = (CX3DShapeNode *)(pShapeNodes->getNode(i));

			if (pShape)
			{
				// --------------------------------------------------------
				//
				// --------------------------------------------------------
				SFNode *geometry = pShape->getGeometry();
				CX3DNode *pNode = geometry->getNode();

				// ---------------------------------------------
				// ---------------------------------------------
				if (pNode && pNode->getNodeType() == BOX_NODE)
				{
					CX3DBoxNode *pBoxNode = (CX3DBoxNode *)pNode;

					// ---------------------------------------------
					// ---------------------------------------------

					SFVec3f *sz = pBoxNode->getSize();
					printf("size : (%f, %f, %f)\n", sz->x(), sz->y(), sz->z());

					// solid
					SFBool *solid = pBoxNode->getSolid();
					printf("solid : %s\n", solid->getValue() ? "TRUE" : "FALSE");
				}
			}
		}

		delete pShapeNodes;
		pShapeNodes = NULL;
	}
}

