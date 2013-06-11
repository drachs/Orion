#include "gen-cpp/Universe.h"

#include <thrift/transport/TSocket.h>
#include <thrift/transport/TBufferTransports.h>
#include <thrift/protocol/TBinaryProtocol.h>

using namespace apache::thrift;
using namespace apache::thrift::protocol;
using namespace apache::thrift::transport;

using namespace  ::Orion::Universe;

int main(int argc, char **argv)
{
    boost::shared_ptr<TSocket> socket(new TSocket("localhost", 9090));
    boost::shared_ptr<TTransport> transport(new TBufferedTransport(socket));
    boost::shared_ptr<TProtocol> protocol(new TBinaryProtocol(transport));

    UniverseClient client(protocol);
    transport->open();

	printf("Ping: ");
	fflush(stdout);
    client.Ping();
    printf("Done\n");

	printf("LongWait: ");
	fflush(stdout);
    client.LongWait();
    printf("Done\n");

	printf("Ping: ");
	fflush(stdout);
    client.Ping();
    printf("Done\n");

    transport->close();

    return 0;
}
