#ifndef CLIENTNET_H_INCLUDED
#define CLIENTNET_H_INCLUDED

#include "thread.h"
#include "gen-cpp/Universe.h"

/**
 * Handles com with the server
 */
class ClientNet : public threads
{
public:
    ClientNet();
    ~ClientNet();
    void task(void);
    ::Orion::Universe::UniverseClient *OrionClient;
};

#endif // CLIENTNET_H_INCLUDED
