#include "ClientNet.h"

#include <iostream>
#include <sys/time.h>

using namespace apache::thrift;
using namespace apache::thrift::protocol;
using namespace apache::thrift::transport;
using namespace std;

#define MS_BETWEEN_SERVER_UPDATES 100

/**
 * Handles com with the server
 */
ClientNet::ClientNet(boost::shared_ptr<apache::thrift::protocol::TProtocol> protocol)
    : UniverseClient(protocol), Clients()
{
    ComMutex.lock();

    PollRate = MS_BETWEEN_SERVER_UPDATES;

    printf("ClientNet\n");

    printf("Ping: ");
    fflush(stdout);
    this->Ping();
    printf("Done\n");

    startThread();
    ComMutex.unlock();
}

void ClientNet::RegisterCallbackObject(OrionClient *ptr)
{
    ComMutex.lock();
    Clients.push_back(ptr);
    ComMutex.unlock();
}

void ClientNet::UnregisterCallbackObject(OrionClient *ptr)
{
    ComMutex.lock();
    Clients.remove(ptr);
    ComMutex.unlock();
}

void ClientNet::task(void)
{
    struct timeval  tv;

    while(1)
    {
        gettimeofday(&tv, NULL);
        double start_time = (tv.tv_sec) * 1000 + (tv.tv_usec) / 1000 ;

        ComMutex.lock();
        // send
        for (	std::list<OrionClient *>::iterator i = Clients.begin();
                i != Clients.end();
                i++)
        {
            if ((*i)->BusyLock.trylock())
                {
                (*i)->Sending = true;
                (*i)->Send();
                }
            else
                (*i)->Sending = false;
        }


        // recv
        for (	std::list<OrionClient *>::iterator i = Clients.begin();
                i != Clients.end();
                i++)
        {
            if ((*i)->Sending)
            {
                (*i)->Recv();
                (*i)->Wakeup();
                (*i)->BusyLock.unlock();
            }
        }

        ComMutex.unlock();

        gettimeofday(&tv, NULL);
        double end_time = (tv.tv_sec) * 1000 + (tv.tv_usec) / 1000 ;

        double difference = (MS_BETWEEN_SERVER_UPDATES - (end_time - start_time));
        if (difference > 0)
            usleep(difference*1000);
        else
            printf("Error, not maintaining pace with networking heartbeat\n");
    }
}

OrionClient::OrionClient(ClientNet *ptr)
    : ClientNetwork(ptr)
{
    startThread();
}

OrionClient::~OrionClient(void)
{
    Stop();
}

void OrionClient::Wakeup(void)
{
    BusyLock.signal();
}

void OrionClient::task(void)
{
    BusyLock.lock();
    while(1)
        {
        BusyLock.wait(); // Releases the mutex while waiting, reaquires before return
        Process();
        }
}

void OrionClient::Start(void)
    {
    ClientNetwork->RegisterCallbackObject(this);
    }

void OrionClient::Stop(void)
    {
    ClientNetwork->UnregisterCallbackObject(this);
    }

