#include "CX3DParserUtil.h"
#include "CX3DParser.h"
#include <stdio.h>

std::string CX3DParserUtil::extractFileBaseName(const char *pathName)
{
	char buf[1024];
	int len;
	int i, iLastSlash, iPeriod;

	if (!pathName) return "";

	len = strlen(pathName);
	iLastSlash = -1;
	iPeriod = len;
	for (i=len-1; i>=0; i--)
	{
		char c = pathName[i];
		if ((iLastSlash == -1) && (c == '/')) iLastSlash = i;
		if ((iPeriod == len) && (c == '.')) iPeriod = i;
	}

	int nCopy = iPeriod - iLastSlash - 1;
	strncpy(buf, pathName+iLastSlash+1, nCopy);
	buf[nCopy] = '\0';

	return buf;
}

