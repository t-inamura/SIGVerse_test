TARGET=libSIGService.a libSIGService.so

SRC=SgvSocket.cpp ViewImage.cpp SIGService.cpp 
OBJ=$(SRC:.cpp=.o)


CC=g++
CFLAGS=-Wall -fPIC
AR=ar


%.o: %.cpp
	$(CC) $(CFLAGS) -o $@ -c $< $(INCLUDE)


all: $(TARGET)


libSIGService.a: $(OBJ)
	$(AR) rcs $@ $(OBJ)

libSIGService.so: $(OBJ)
	$(CC) $(CFLAGS) -shared -o $@ $(OBJ) 
