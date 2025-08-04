// HcranNetwork.ned
package hcran;  // define a package if needed

import hcran.BaseStation;
import hcran.RRH;

network HcranNetwork
{
    parameters:
        // (No specific parameters at network level; see omnetpp.ini for global settings)
    submodules:
        // 5 Macro base stations
        macro: BaseStation[5] {
            parameters: type = "Macro";  // label the type (for potential use or visualization)
        }
        // 3 Micro base stations
        micro: BaseStation[3] {
            parameters: type = "Micro";
        }
        // 2 Pico base stations
        pico: BaseStation[2] {
            parameters: type = "Pico";
        }
        // One Remote Radio Head controller
        rrh: RRH {
            parameters:
                totalChannels = default(100);      // total available channel units to distribute
                dynamicAllocation = default(true); // enable/disable dynamic allocation (set per config)
                requestInterval = default(1s);     // polling interval for spectral sensing
        }
    connections:
        // Connect RRH to all Macro base stations (bidirectional)
        for i=0..4 {
            rrh.macroOut[i] --> macro[i].in;
            macro[i].out --> rrh.macroIn[i];
        }
        // Connect RRH to all Micro base stations
        for j=0..2 {
            rrh.microOut[j] --> micro[j].in;
            micro[j].out --> rrh.microIn[j];
        }
        // Connect RRH to all Pico base stations
        for k=0..1 {
            rrh.picoOut[k] --> pico[k].in;
            pico[k].out --> rrh.picoIn[k];
        }
}
