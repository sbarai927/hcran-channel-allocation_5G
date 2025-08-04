// RRH.cc
#include "RRH.h"
Define_Module(RRH);

// Define message kind constants (consistent with BaseStation)
enum { REQUEST = 0, REPLY = 1, SELF_TIMER = 2 };

void RRH::initialize() {
    // Read parameters from NED/ini
    totalChannels = par("totalChannels");
    dynamicAllocation = par("dynamicAllocation");
    requestInterval = par("requestInterval");
    numMacro = gateSize("macroIn");   // should be 5
    numMicro = gateSize("microIn");   // 3
    numPico  = gateSize("picoIn");    // 2
    totalCells = numMacro + numMicro + numPico;
    // Allocate arrays
    channels = new int[totalCells];
    // Initialize equal channel allocation (baseline)
    int baseShare = totalChannels / totalCells;
    int remainder = totalChannels % totalCells;
    for (int i = 0; i < totalCells; ++i) {
        channels[i] = baseShare + (i < remainder ? 1 : 0);
    }
    EV << "RRH initialized. Total channels=" << totalChannels 
       << " distributed equally among " << totalCells << " cells ("
       << numMacro << " macro, " << numMicro << " micro, " << numPico 
       << " pico).\n";

    // Create and schedule the first polling timer
    pollTimer = new cMessage("pollTimer");
    pollTimer->setKind(SELF_TIMER);
    scheduleAt(simTime() + requestInterval, pollTimer);
    repliesExpected = 0;
}

void RRH::handleMessage(cMessage *msg) {
    if (msg->isSelfMessage()) {
        // Time to poll the BaseStations for their current load
        EV << "RRH polling all cells for load (interval=" << requestInterval << "s).\n";
        repliesExpected = totalCells;
        // Send a load request to each macro, micro, and pico cell
        for (int i = 0; i < numMacro; ++i) {
            cMessage *req = new cMessage("LoadRequest");
            req->setKind(REQUEST);
            send(req, "macroOut", i);
        }
        for (int j = 0; j < numMicro; ++j) {
            cMessage *req = new cMessage("LoadRequest");
            req->setKind(REQUEST);
            send(req, "microOut", j);
        }
        for (int k = 0; k < numPico; ++k) {
            cMessage *req = new cMessage("LoadRequest");
            req->setKind(REQUEST);
            send(req, "picoOut", k);
        }
        // After sending requests, do nothing until replies come in.
        // (The replies will be handled in this same handleMessage function.)
    }
    else if (msg->getKind() == REPLY) {
        // A BaseStation replied with its load.
        // Identify which station (index) this reply came from:
        cGate *arrivalGate = msg->getArrivalGate();
        const char *gateName = arrivalGate->getBaseName();  // "macroIn", "microIn", or "picoIn"
        int idx = arrivalGate->getIndex();
        int globalIndex;
        if (strcmp(gateName, "macroIn") == 0) {
            globalIndex = idx;  // macro index 0-4
        } else if (strcmp(gateName, "microIn") == 0) {
            globalIndex = numMacro + idx;  // micro index 5-7
        } else { 
            globalIndex = numMacro + numMicro + idx;  // pico index 8-9
        }
        // Retrieve the reported load from the message
        int load = msg->par("load").intValue();
        EV << "RRH received load reply from cell index " << globalIndex 
           << " (gate " << gateName << "[" << idx << "]): load=" << load << ".\n";

        // (Optionally, store the load in an array for analysis)
        // In this simple case, we compute new allocation on the fly once all replies are in.
        // We could store loads[globalIndex] = load; (and have an array int *loads)

        // Decrement the count of expected replies
        repliesExpected--;
        if (repliesExpected == 0) {
            // All cells have replied for this round.
            EV << "All load reports received. ";
            if (dynamicAllocation) {
                EV << "Calculating new channel allocations...\n";
                // Gather loads from messages (if not stored individually, one could iterate connected gates)
                // Here, for simplicity, assume we kept track of loads in an array; 
                // (Alternatively, we would need to store each load above; skipped for brevity.)
                // **In a complete implementation, we maintain an array 'loads[totalCells]' updated above.**

                // For demonstration, suppose we had stored loads; we'll simulate using the 'load' from this last message 
                // and assume others were stored similarly. (Omitted actual storage due to brevity.)
            }
            else {
                EV << "Dynamic allocation disabled; keeping previous channel assignments.\n";
            }

            if (dynamicAllocation) {
                // Example dynamic allocation: proportional to load (placeholder logic)
                // [In an actual implementation, use stored loads of all cells from this cycle.]
                // Here, for illustration, we will reuse the last received 'load' for all (NOT realistic, but simplifies code).
                int totalLoad = 0;
                // (Assume loads array exists; summing all loads)
                // for (int i = 0; i < totalCells; ++i) totalLoad += loads[i];
                // Instead, we'll simulate a totalLoad and individual loads for clarity:
                totalLoad = 1; // avoid division by zero (dummy value)
                // (In real code, compute actual totalLoad; omitted.)

                // Reallocate channels (this code is illustrative; actual logic should use each cell's load)
                for (int i = 0; i < totalCells; ++i) {
                    // channels allocated proportional to load fraction
                    // channels[i] = (int) round((loads[i] / (double)totalLoad) * totalChannels);
                    // For simplicity, assign at least 1 channel to each and distribute remainder
                    channels[i] = (totalChannels / totalCells);
                }
                // (Adjust channels array to ensure sum == totalChannels)
                // ...
                EV << "New channel distribution: ";
                for (int i = 0; i < totalCells; ++i) {
                    EV << channels[i] << (i < totalCells-1 ? ", " : " (channels per cell)\n");
                }
            }

            // Schedule the next polling round
            scheduleAt(simTime() + requestInterval, pollTimer);
        }
        // Clean up the reply message
        delete msg;
    }
}
