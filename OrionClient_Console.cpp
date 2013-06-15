#include <iostream>
#include "ClientNet.h"

#include <thrift/transport/TSocket.h>
#include <thrift/transport/TBufferTransports.h>
#include <thrift/protocol/TBinaryProtocol.h>

using namespace  ::Orion::Universe;

using namespace std;

void LongRangeScan(void);

// See ClientNet.h for docs on the OrionClient Class
class ExampleClient : public OrionClient {
public:
    ExampleClient(ClientNet *ptr) : OrionClient(ptr) {}

    void Send(void);
    void Recv(void);
    void Process(void);

    std::map<std::string, ::Orion::Universe::Anomaly> UniverseData;
    std::map<std::string, ::Orion::Universe::Anomaly> LocalData;

    void PrintScanData(std::map<std::string, ::Orion::Universe::Anomaly> &Data);
};

/**
 * This is called once per heartbeat if we're not busy running Process().  You should do all your send_'s in here and try to be quick about it.
 */
void ExampleClient::Send(void)
    {
    ClientNetwork->send_LongRangeScan();
	ClientNetwork->send_ShortRangeScan();
    }

/**
 * This is called when our data is has started comming back.  We do the recv_ half of our RPC calls here, and again we should try to be quick about it.
 */
void ExampleClient::Recv(void)
    {
    ClientNetwork->recv_LongRangeScan(UniverseData);
	ClientNetwork->recv_ShortRangeScan(LocalData);
    }

/**
 * This is called when we've successfully received our data in Recv.   This is run in our own personal thread so we can dilly dally in here as long as we want.  If we dilly
 * dally longer than the heartbeat interval, the networking subsystem will skip us.   Take as long as you need, and return when you're ready to consume more data from the server.
 * You should avoid direct server calls in here unless they're marked "oneway" (Oneway calls are free, go for it!).  You can do them if you need them, however you will tie up the com
 * channel for an entire round trip.   This can be avoided by using the Send and Recv system above.
 * This won't matter on the lan.  If you're going to do some anyway, "oneway" or not, you'll need to aquire the ClietnNetwork.ComMutex first to prevent conflict with the com engine itself, or
 * other client threads.
 */
void ExampleClient::Process(void)
    {
    printf("Heartbeat\n");

	cout << endl << "Long Range Scan Data:" << endl;
    PrintScanData(UniverseData);

    cout << endl << "Short Range Scan Data: " << endl;
    PrintScanData(LocalData);

    sleep(5);
    }

/// Print some space stuff
void ExampleClient::PrintScanData(std::map<std::string, ::Orion::Universe::Anomaly> &Data)
    {
	for (	std::map<std::string, Anomaly>::iterator i = Data.begin();
			i != Data.end();
			i++)
		{
		Anomaly &Result = i->second;

		cout << "Detected object: " << Result.id << endl;
		if (Result.__isset.Position)
			{
			cout << "Position: " << Result.Position.Position.x << ", " << Result.Position.Position.y << ", " << Result.Position.Position.z << endl;
			}

		if (Result.__isset.GravityWell)
			{
			cout << "Gravity Well of Mass: " << Result.GravityWell.Mass << endl;
			}

		cout << endl;
		}
    }

int main(int argc, char **argv)
{
    // Thrift Incantation
    boost::shared_ptr<apache::thrift::transport::TSocket> socket(new apache::thrift::transport::TSocket("localhost", 9090));
    boost::shared_ptr<apache::thrift::transport::TTransport> transport(new apache::thrift::transport::TBufferedTransport(socket));
    boost::shared_ptr<apache::thrift::protocol::TProtocol> protocol(new apache::thrift::protocol::TBinaryProtocol(transport));

    transport->open();

	ClientNet ClientNetwork(protocol);

	ExampleClient myExampleClient(&ClientNetwork);
	myExampleClient.Start();

    while(1)
        sleep(1000);
}

