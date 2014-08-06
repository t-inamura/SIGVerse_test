#include <stdio.h>
#include "CX3DParser.h"
#include "CX3DParserUtil.h"
#include "CX3DField.h"
#include "CJNIUtil.h"
#include "Test.h"
#include <string>

int main(int argc, char *argv[])
{
	int testNo = 0;
	char *vrmlFile = NULL;
	int nMFMax = 0;
	char *logfile = NULL;
	int iRestArgsTop = 0;
	int nRestArgs = 0;

	if (argc < 3)
	{
#ifdef WIN32
#include "MainHelp_win.cpp"
#else
#include "MainHelp_linux.cpp"
#endif
		return 1;
	}
	else
	{
		testNo = atoi(argv[1]);
		vrmlFile = argv[2];	

		iRestArgsTop = 3;
		nRestArgs = 0;

		for (int i=3; i<argc; i++)
		{
			if ((strcmp(argv[i], "-MFMax") == 0) && ((i+1) < argc))
			{
				nMFMax = atoi(argv[i+1]);
				i++;
			}
			else if ((strcmp(argv[i], "-log") == 0) && ((i+1) < argc))
			{
				logfile = argv[i+1];
				CX3DParser::openDebugLog(logfile);
				i++;
			}
			else
			{
				nRestArgs++;
			}
		}
	}

	fprintf(stderr, "testNo : %d\n", testNo);
	fprintf(stderr, "nMFMax : %d\n", nMFMax);
	fprintf(stderr, "logfile : %s\n", logfile ? logfile : "stderr");

	// begin(add)(2009/3/27)
	if (!CJNIUtil::init("X3DParser.cfg"))
	{
		CX3DParser::printLog("*** Failed to initialize Java VM ***");
		exit(1);
	}
	// end(add)

	CX3DParser parser;

	parser.setMaxPrintElemsForMFField(nMFMax);

	switch (testNo)
	{
	case 0:
		Test_printVRML(parser, vrmlFile);
		break;

	case 1:
		Test_printChildrenOfRoot(parser, vrmlFile);
		break;

	case 2:
		Test_printNodeName(parser, vrmlFile);
		break;

	case 3:
		Test_printFieldOfNode(parser, vrmlFile);
		break;

	case 4:
		Test_printFieldsOfBoxNode(parser, vrmlFile);
		break;

	case 5:
		Test_printFieldsOfTransformNode(parser, vrmlFile);
		break;

	case 6:
		Test_calcSimplifiedShape(parser, vrmlFile);
		break;

	case 7:
		printOpenHRPStructure(parser, vrmlFile);
		break;

	case 8:
		Test_printDefNames(parser, vrmlFile);
		break;

	case 9:
		Test_printDefNode(parser, vrmlFile, argv[iRestArgsTop]);
		break;
	}

	return 0;
}

