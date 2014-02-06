JC      = javac
JA      = jar
#JFLAGS = -g
JFLAGS  = 
JAFLAGS = cvf
JINC    = -cp .:/usr/local/Xj3D/jars/*

JARTGF  = X3DParser.jar

.SUFFIXES: .java .class

SRCS = MyWorldRoot.java \
       MyX3DUtil.java \
       X3DParseEventHandler.java \
       X3DParser.java \
       X3DParserTest.java

OBJS = $(SRCS:.java=.class)

.java.class:
	$(JC) $(JFLAGS) $(JINC) $<

default: libs

libs: $(OBJS)
	$(JA) $(JAFLAGS) $(JARTGF) $(OBJS)

clean:
	$(RM) ./*.class ./X3DParser.jar
