// BaseStation.cc
#include "BaseStation.h"

Define_Module(BaseStation);

// Define message kind constants (must match RRH's definitions)
enum { REQUEST = 0, REPLY = 1 };

void BaseStation::initialize() {
    // Optionally, record initial channel allocation or other setup. 
    int initCh = par("initialChannels");
    EV << getFullName() << " initialized with " << initCh 
       << " channels, initial load=" << par("userLoad").intValue() << ".\n";
}

void BaseStation::handleMessage(cMessage *msg) {
    if (msg->getKind() == REQUEST) {
        // Received a load request from RRH
        int currentLoad = par("userLoad").intValue();
        EV << getFullName() << " received load request, current load=" 
           << currentLoad << ". Sending reply.\n";

        // Create a reply message with the load information
        cMessage *reply = new cMessage("LoadReply");
        reply->setKind(REPLY);
        // Attach the load as a message parameter
        reply->addPar("load") = currentLoad;

        // Send reply back to RRH via the output gate
        send(reply, "out");

        // (Optional) Update the userLoad to simulate changes over time 
        // For example, increment load by a random small value to mimic new users joining
        int delta = intuniform(0, 5);  // small random increase
        par("userLoad").setIntValue(currentLoad + delta);

        // Delete the incoming request message to free memory
        delete msg;
    } else {
        // Unexpected message types can be handled here
        EV << getFullName() << " received an unknown message, kind=" << msg->getKind() << ".\n";
        delete msg;
    }
}
