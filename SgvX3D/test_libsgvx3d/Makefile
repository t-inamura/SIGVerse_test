RM       = rm -f
CXX      = g++
CXXFLAGS = -g -Wall
LDFLAGS = -L$(JVMDIR) -ljvm

X3DPARSERDIR = ../X3DParser

JVMDIR = /usr/lib/jvm/java-6-openjdk-amd64/jre/lib/amd64/server

INCLUDE = -I$(X3DPARSERDIR)

.SUFFIXES: .cpp .o

.cpp.o:
	$(CXX) $(CXXFLAGS) $(INCLUDE) -c -o $@ $<
#$(CXX) $(CXXFLAGS) $(DEFINE) $(INCLUDE) -c -o $@ $<

SRCS = Main.cpp \
	Test_BoxNode.cpp \
	Test_CalcSimplifiedShape.cpp \
	Test_GetChildrenOfRootNode.cpp \
	Test_GetDefNames.cpp \
	Test_GetDefNode.cpp \
	Test_GetNodeName.cpp \
	Test_LoopFieldOfNode.cpp \
	Test_OpenHRP.cpp \
	Test_SimpleParse.cpp \
	Test_TransformNode.cpp

TESTNAME = xj3dtest

OBJS = $(SRCS:.cpp=.o)

# fix(2009/4/10)
# LIBS = $(X3DPARSERDIR)/libx3d-c++.a
LIBS = $(X3DPARSERDIR)/libx3dc++.a

all: $(OBJS)
	$(CXX) -o $(TESTNAME) $(OBJS) $(LIBS) $(LDFLAGS)

clean:
	$(RM) *.o $(TESTNAME)
