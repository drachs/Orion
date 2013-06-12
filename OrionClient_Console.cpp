#include "gen-cpp/Universe.h"

#include <thrift/transport/TSocket.h>
#include <thrift/transport/TBufferTransports.h>
#include <thrift/protocol/TBinaryProtocol.h>

#include <iostream>

using namespace apache::thrift;
using namespace apache::thrift::protocol;
using namespace apache::thrift::transport;

using namespace  ::Orion::Universe;
using namespace std;

UniverseClient *OrionClient;

void LongRangeScan(void);

int main(int argc, char **argv)
{
    boost::shared_ptr<TSocket> socket(new TSocket("localhost", 9090));
    boost::shared_ptr<TTransport> transport(new TBufferedTransport(socket));
    boost::shared_ptr<TProtocol> protocol(new TBinaryProtocol(transport));

	OrionClient = new UniverseClient(protocol);

    transport->open();

	printf("Ping: ");
	fflush(stdout);
    OrionClient->Ping();
    printf("Done\n");

	LongRangeScan();

    transport->close();

    return 0;
}


void LongRangeScan(void)
	{
	std::map<std::string, Anomaly> UniverseData;

	OrionClient->LongRangeScan(UniverseData);

	for (	std::map<std::string, Anomaly>::iterator i = UniverseData.begin();
			i != UniverseData.end();
			i++)
		{
		Anomaly &Result = i->second;

		cout << "Detected object: " << Result.id << endl;
		if (Result.__isset.Position)
			{
			cout << "Position: " << Result.Position.Position.x << ", " << Result.Position.Position.y << ", " << Result.Position.Position.z << endl;
			}

		if (Result.__isset.Position)
			{
			cout << "Gravity Well of Mass: " << Result.GravityWell.Mass << endl;
			}

		cout << endl;
		}
	}
