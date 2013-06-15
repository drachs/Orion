#ifndef CLIENTNET_H_INCLUDED
#define CLIENTNET_H_INCLUDED

#include <thrift/transport/TSocket.h>
#include <thrift/transport/TBufferTransports.h>
#include <thrift/protocol/TBinaryProtocol.h>

#include "thread.h"
#include "gen-cpp/Universe.h"

class OrionClient;

/**
 * Network Communication Object extending thrift.
 */
class ClientNet : public ::Orion::Universe::UniverseClient, threads
{
public:
    ClientNet(boost::shared_ptr<apache::thrift::protocol::TProtocol> protocol);

    /**
     * Only one thread may be sending requests at a time because responses must be read in the order they are requested.  Aquire
     * this lock before executing any services.  Receive all your responses before you let it go and you'll be fine.  This rule can
     * be bent.   See the rest of the docs.  Also used for manipulating callbacks.  Don't take the lock and try to de/register a callback.
     */
    mutex ComMutex;

    /// Number of milliseconds between network update pulses.  At the time of this comment, defaults to 100ms.
    unsigned int PollRate;

    /**
     * This callback functions allow you to register a callback that will be executed once for each network update tick from the Networking Thread.
     * When this happens, you can make a bunch of send_'s in a row without incuring additional round trip times.  You should request the bulk of your data
     * updates this way.  Do not execute any ClientNet functions other than send_ functions.   This is called with the ComMutex locked already, don't lock it again.
     */
    void RegisterCallbackObject(OrionClient *ptr);

    /**
     * Your callback will no longer be called
     */
    void UnregisterCallbackObject(OrionClient *ptr);


    /*
     * Caching Overrides.  These functions cache thrift call results to prevent common requests with beefy returns from needlessly crossing the wire many times.  These
     * will need to be updated if the service API gets updated.
     */

    /*    void LongRangeScan(std::map<std::string, Anomaly> & _return)
        void send_LongRangeScan();
        void recv_LongRangeScan(_return);

        void ShortRangeScan(std::map<std::string, Anomaly> & _return)
        void send_ShortRangeScan();
        void recv_ShortRangeScan(_return); */


private:
    /// Indicates which tick we're on.  Used by caching functions.
    unsigned int ComTick;
    /// Executes callbacks
    void task(void);

    // Callback lists
    std::list<OrionClient *> Clients;
};

/**
 * Orion Client
 */
class OrionClient: public threads
{
public:
    OrionClient(ClientNet *);
    ~OrionClient(void);

    friend class ClientNet;

    /**
     * Used to control the sleeping of the task and Process function.  This lock is held by Process() when it's running. Process cannot wakeup if it is held.  As long as this lock is held
     * this class won't receive calls to Send and Recv.
     */
    conditional_variable BusyLock;

    /// Register with the network handler and start requesting data
    void Start(void);
    /// Unregister and stop requesting data
    void Stop(void);

protected:
    ClientNet *ClientNetwork;

private:
    /// Override this function and execute all your send_ calls. Run in the networking thread, so don't take your time.  Don't try and aquire the ComMutex.
    virtual void Send(void) = 0;
    /// Override  this function and execute all your recv_ calls.  Run in the networking thread, so don't take your time. Don't try and aquire the ComMutex.
    virtual void Recv(void) = 0;
    /// Signal this class to run Process
    void Wakeup(void);

    /**
     * Override this function to do your work.  This thread is private to you so take as much time as you need.  Return when you're done, you'll be execute again next time there is data.  You must
     * aquire the ClientNet ComMutex before you call any networking functions.
     */
    virtual void Process(void) = 0;

    /// This function sleeps until Wakup has been called, then executes Process
    void task(void);

    /**
     *  If true, the system is handling Send requests for this class.  This is set to false when the class is too busy in process to handle them.  This variable is
     * managed by the ClientNet class and isn't thread safe for any other class to tinker with, including OrionClient.
     */
    bool Sending;
};

#endif // CLIENTNET_H_INCLUDED
