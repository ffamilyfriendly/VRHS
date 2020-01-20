SOURCES = \
	./src/common.cpp \
	./src/request.cpp \
	./src/response.cpp \
	./src/server.cpp

all:
	g++ -shared -fPIC ${SOURCES} -o ./bin/libVRHS.so