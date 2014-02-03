#SIGVerseヘッダファイルの場所指定
SIG_SRC  = $(SIGVERSE_PATH)/include/sigverse

#オブジェクトファイルの指定
OBJS     = OculusRiftCtrl.so

all: $(OBJS)

#コンパイルを行います
./%.so: ./%.cpp
	g++ -DCONTROLLER -DNDEBUG -DUSE_ODE -DdDOUBLE -I$(SIG_SRC) -I$(SIG_SRC)/comm/controller  -fPIC -shared -o $@   $<

clean:
	rm $(OBJS)
