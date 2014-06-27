SRC=PSMoveAPIService.cpp
OBJ=$(SRC:.cpp=.o)
EXE=PSMoveAPIService.sig


SIGVERSEPATH=/home/babouchot/workspace/SIGService_src_v2-2-0/SIGService


SIGVERSELIB=/home/babouchot/workspace/SIGService_src_v2-2-0/SIGService


INCLUDE=-I$(SIGVERSEPATH)
LIBSPATH=-L$(SIGVERSELIB)


LIBS=-lSIGService -lpsmoveapi -lpsmoveapi_tracker -lopencv_core -lopencv_highgui

CC=g++
CFLAGS=-Wall -g
# LDFLAGS=-mwindows

%.o: %.cpp
	$(CC) $(CFLAGS) $(INCLUDE) -o $@ -c $< 


all: $(EXE)


$(EXE): $(OBJ)
	$(CC) $(LDFLAGS) -o $@ $(LIBSPATH) $(OBJ) $(LIBS)

