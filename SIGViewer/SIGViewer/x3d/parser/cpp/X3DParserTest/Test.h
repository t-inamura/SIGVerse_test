#ifndef _TEST_H_
#define _TEST_H_

void Test_printVRML(CX3DParser& parser, char *vrmlFile);

void Test_printChildrenOfRoot(CX3DParser& parser, char *vrmlFile);
void Test_printNodeName(CX3DParser& parser, char *vrmlFile);
void Test_printFieldOfNode(CX3DParser& parser, char *vrmlFile);
void Test_printFieldsOfBoxNode(CX3DParser& parser, char *vrmlFile);
void Test_printFieldsOfTransformNode(CX3DParser& parser, char *vrmlFile);

void Test_printDefNames(CX3DParser& parser, char *vrmlFile);
void Test_printDefNode(CX3DParser& parser, char *vrmlFile, char *defName);
void Test_calcSimplifiedShape(CX3DParser& parser, char *vrmlFile);

void printOpenHRPStructure(CX3DParser& parser, char *vrmlFile);
void printOpenHRPJointNode(CX3DOpenHRPJointNode *pJointNode, int indent);
void printOpenHRPSegmentNode(CX3DOpenHRPSegmentNode *pSegmentNode, int indent);

#endif
