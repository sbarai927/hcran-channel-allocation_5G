// BaseStation.h
#ifndef HCRAN_BASESTATION_H
#define HCRAN_BASESTATION_H

#include <omnetpp.h>
using namespace omnetpp;

/**
 * BaseStation module (Macro/Micro/Pico cell). 
 * Responds to load requests from the RRH by sending back its current user load.
 */
class BaseStation : public cSimpleModule
{
  protected:
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;
};

#endif
