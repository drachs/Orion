// This autogenerated skeleton file illustrates how to build a server.
// You should copy it to another filename to avoid overwriting it.

#include <stdint.h>
#include "gen-cpp/Universe.h"
#include <thrift/protocol/TBinaryProtocol.h>
#include <thrift/transport/TServerSocket.h>
#include <thrift/transport/TBufferTransports.h>
#include <thrift/concurrency/ThreadManager.h>
#include <thrift/concurrency/PosixThreadFactory.h>
#include <thrift/server/TThreadedServer.h>
#include "boost/thread/tss.hpp"
#include "thread.h"
#include <math.h>
#include <string.h>

using namespace ::apache::thrift;
using namespace ::apache::thrift::protocol;
using namespace ::apache::thrift::transport;
using namespace ::apache::thrift::server;

using boost::shared_ptr;

using namespace  ::Orion::Universe;
using namespace std;

class UniverseHandler : virtual public UniverseIf
{
public:
    UniverseHandler()
    : ClientNumber(0)
    {
        UniverseData["Star_Sol"].id = "Star_Sol";
        UniverseData["Star_Sol"].__isset.Position = true;
        UniverseData["Star_Sol"].Position.Position.x = 0;
        UniverseData["Star_Sol"].Position.Position.y = 0;
        UniverseData["Star_Sol"].Position.Position.z = 0;

        UniverseData["Star_Sol"].__isset.GravityWell = true;
        UniverseData["Star_Sol"].GravityWell.Mass = 1.989 * pow(10,30);

        UniverseData["Planet_Earth"].id = "Planet_Earth";
        UniverseData["Planet_Earth"].__isset.Position = true;
        UniverseData["Planet_Earth"].Position.Position.x = 0;
        UniverseData["Planet_Earth"].Position.Position.y = 92960000;
        UniverseData["Planet_Earth"].Position.Position.z = 0;

        UniverseData["Planet_Earth"].__isset.GravityWell = true;
        UniverseData["Planet_Earth"].GravityWell.Mass = 5.972 * pow(10,24);

        LocalData["Asteroid_AA01"].id = "Asteroid_AA01";
        LocalData["Asteroid_AA01"].__isset.Position = true;
        LocalData["Asteroid_AA01"].Position.Position.x = 23123300;
        LocalData["Asteroid_AA01"].Position.Position.y = 52960000;
        LocalData["Asteroid_AA01"].Position.Position.z = 32322440;

        LocalData["Asteroid_AA01"].__isset.GravityWell = true;
        LocalData["Asteroid_AA01"].GravityWell.Mass = 4.251 * pow(10,20);

        LocalData["Entity_Box"].id = "Entity_Box";
        LocalData["Entity_Box"].__isset.Position = true;
        LocalData["Entity_Box"].Position.Position.x = 23123300;
        LocalData["Entity_Box"].Position.Position.y = 52960000;
        LocalData["Entity_Box"].Position.Position.z = 32322440;
    }

    void LongRangeScan(std::map<std::string, netAnomaly> & _return)
    {
        // Your implementation goes here
        cout << *StationDescription << ": LongRangeScan" << endl;
        _return = UniverseData;
    }

    void ShortRangeScan(std::map<std::string, netAnomaly> & _return)
    {
        // Your implementation goes here
        cout << *StationDescription << ": ShortRangeScan" << endl;
        _return = LocalData;
    }

    void SetEngineVector(const Orion::Universe::netVector& Vector, double Magnitude)
    {
        cout << *StationDescription << ": SetEngineVector: " << Vector.x << ", " << Vector.y << ", " << Vector.z << " " << Magnitude << endl;
    }

    bool JoinShip(const string &id, const string &StationDescription_in)
    {
    ServerLock.lock();
    int myClientNumber = ClientNumber++;
    ServerLock.unlock();


    StationDescription.reset(new string(StationDescription_in+" "+to_string(myClientNumber)));
    }

    void Ping(void)
    {
        // Your implementation goes here
        cout << "Ping" << endl;
    }

	void LongWait(void)
	{
		sleep(60);
	}

private:
	std::map<std::string, netAnomaly> UniverseData;
	std::map<std::string, netAnomaly> LocalData;

	boost::thread_specific_ptr<string> StationDescription;

    mutex ServerLock;

    unsigned int ClientNumber;
};

int main(int argc, char **argv)
{
    boost::shared_ptr<UniverseHandler> handler(new UniverseHandler());
    boost::shared_ptr<TProcessor> processor(new UniverseProcessor(handler));

    boost::shared_ptr<TServerTransport> serverTransport (new TServerSocket(9090));

    boost::shared_ptr<TTransportFactory> transportFactory(new TBufferedTransportFactory());

    boost::shared_ptr<TProtocolFactory> protocolFactory(new TBinaryProtocolFactory());

    //shared_ptr<ThreadManager> threadManager = ThreadManager::newSimpleThreadManager(15);
    //shared_ptr<PosixThreadFactory> threadFactory = shared_ptr<PosixThreadFactory>(new PosixThreadFactory());
    //threadManager->threadFactory(threadFactory);
    //threadManager->start();

    TThreadedServer server(processor,
                           serverTransport,
                           transportFactory,
                           protocolFactory);

    server.serve();
    return 0;
}

