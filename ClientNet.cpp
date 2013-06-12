#include "ClientNet.h"

#include <iostream>
#include <sys/time.h>

#include <thrift/transport/TSocket.h>
#include <thrift/transport/TBufferTransports.h>
#include <thrift/protocol/TBinaryProtocol.h>

using namespace apache::thrift;
using namespace apache::thrift::protocol;
using namespace apache::thrift::transport;

using namespace  ::Orion::Universe;

using namespace std;

#define MS_BETWEEN_SERVER_UPDATES 100

/**
 * Handles com with the server
 */
ClientNet::ClientNet()
{
    printf("ClientNet\n");

    boost::shared_ptr<TSocket> socket(new TSocket("localhost", 9090));
    boost::shared_ptr<TTransport> transport(new TBufferedTransport(socket));
    boost::shared_ptr<TProtocol> protocol(new TBinaryProtocol(transport));

    OrionClient = new UniverseClient(protocol);

    transport->open();

    printf("Ping: ");
    fflush(stdout);
    OrionClient->Ping();
    printf("Done\n");

    startThread();
}

ClientNet::~ClientNet()
{
    //transport->close();
}

void ClientNet::task(void)
{
    std::map<std::string, Anomaly> LongRangeData;
    std::map<std::string, Anomaly> ShortRangeData;
    struct timeval  tv;

    while(1)
    {
        gettimeofday(&tv, NULL);
        double start_time = (tv.tv_sec) * 1000 + (tv.tv_usec) / 1000 ;

        OrionClient->send_LongRangeScan();
        OrionClient->send_ShortRangeScan();

        OrionClient->recv_LongRangeScan(LongRangeData);
        OrionClient->recv_ShortRangeScan(ShortRangeData);

        gettimeofday(&tv, NULL);
        double end_time = (tv.tv_sec) * 1000 + (tv.tv_usec) / 1000 ;

        double difference = (MS_BETWEEN_SERVER_UPDATES - (end_time - start_time));
        if (difference > 0)
            usleep(difference*1000);
		else
			printf("Error, data transfer taking too long\n");
    }
}

