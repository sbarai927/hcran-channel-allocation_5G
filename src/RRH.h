// RRH.h
#ifndef HCRAN_RRH_H
#define HCRAN_RRH_H

#include <omnetpp.h>
using namespace omnetpp;

/**
 * RRH module: dynamically allocates channels between cells based on load.
 * Periodically polls all connected BaseStations for their load and redistributes channels.
 */
class RRH : public cSimpleModule
{
  private:
    // Parameters
    int totalChannels;
    bool dynamicAllocation;
    double requestInterval;
    // State
    int numMacro; 
    int numMicro;
    int numPico;
    int totalCells;
    int *channels;    // array of current channel allocation per cell
    cMessage *pollTimer;  // self-message for triggering periodic polls
    int repliesExpected;  // count of replies awaited in the current round

  protected:
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;
    virtual void finish() override;
};

#endif
