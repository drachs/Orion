OBS = gen-cpp/Universe_constants.o gen-cpp/Universe.o gen-cpp/Universe_types.o
# Work around thrift compile bug
CPPFLAGS = -DHAVE_NETINET_IN_H -DHAVE_INTTYPES_H

all : OrionServer OrionClient_Console

OrionServer : gen-cpp ${OBS} OrionServer.o
	g++ -o OrionServer OrionServer.o ${OBS} -lthrift

OrionClient_Console : gen-cpp ${OBS} OrionClient_Console.o
	g++ -o OrionClient_Console OrionClient_Console.o ${OBS} -lthrift

gen-cpp : Universe.thrift
	thrift --gen cpp Universe.thrift

clean:
	-rm -r *.o gen-cpp OrionServer OrionClient_Console *~

.PHONY: all clean
