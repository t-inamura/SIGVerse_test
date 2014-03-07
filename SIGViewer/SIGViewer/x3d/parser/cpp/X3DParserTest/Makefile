
RM = rm -f
AR = ar -cr
CXX = g++ -g

X3DPARSERDIR = ../X3DParser

JVMDIR = /opt/sun-jdk-1.6.0.06/jre/lib/amd64/server

INCLUDE = -I$(X3DPARSERDIR)

.SUFFIXES: .cpp .o

.cpp.o:
	$(CXX) $(CXXFLAGS) $(DEFINE) $(INCLUDE) -c -o $@ $<


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

LDFLAGS = -L $(JVMDIR) -ljvm

all: lib $(OBJS)
	$(CXX) -o $(TESTNAME) $(OBJS) $(LIBS) $(LDFLAGS)


lib:
	make -C $(X3DPARSERDIR)

clean:
	make -C $(X3DPARSERDIR) clean
	$(RM) *.o $(TESTNAME)
