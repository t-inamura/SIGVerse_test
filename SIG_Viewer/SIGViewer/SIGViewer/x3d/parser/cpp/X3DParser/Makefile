
RM = rm -f
AR = ar -cr
#AR = g++ -shared -o
#CXX = g++ -g
CXX = g++

SRCS = CJNIUtil.cpp \
	CSimplifiedShape.cpp \
	CVRMLFieldData.cpp \
	CX3DAppearanceNode.cpp \
	CX3DBoxNode.cpp \
	CX3DColorNode.cpp \
	CX3DConeNode.cpp \
	CX3DCoordinateNode.cpp \
	CX3DCylinderNode.cpp \
	CX3DDirectionalLightNode.cpp \
	CX3DField.cpp \
	CX3DGroupNode.cpp \
	CX3DHAnimDisplacerNode.cpp \
	CX3DHAnimHumanoidNode.cpp \
	CX3DHAnimJointNode.cpp \
	CX3DHAnimSegmentNode.cpp \
	CX3DHAnimSiteNode.cpp \
	CX3DImageTextureNode.cpp \
	CX3DIndexedFaceSetNode.cpp \
	CX3DIndexedLineSetNode.cpp \
	CX3DMaterialNode.cpp \
	CX3DNode.cpp \
	CX3DNormalNode.cpp \
	CX3DOpenHRPHumanoidNode.cpp \
	CX3DOpenHRPJointNode.cpp \
	CX3DOpenHRPSegmentNode.cpp \
	CX3DParser.cpp \
	CX3DParserUtil.cpp \
	CX3DPointLightNode.cpp \
	CX3DShapeNode.cpp \
	CX3DSphereNode.cpp \
	CX3DSpotLightNode.cpp \
	CX3DTextureCoordinateNode.cpp \
	CX3DTextureTransformNode.cpp \
	CX3DTransformNode.cpp \
	CX3DViewpointNode.cpp \
	SgvConfigFile.cpp



OBJS = $(SRCS:.cpp=.o)

LIBNAME = libx3dc++.a

.SUFFIXES: .cpp .o

.cpp.o:
	$(CXX) $(CXXFLAGS) $(DEFINE) $(INCLUDE) -c -o $@ $<

all: lib

lib: $(OBJS)
	$(AR) $(LIBNAME) $(OBJS)

clean:
	$(RM) $(LIBNAME) *.o

install:

uninstall: