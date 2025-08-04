// RRH.ned
package hcran;

simple RRH
{
    parameters:
        int totalChannels;         // total number of channel units to distribute among cells
        bool dynamicAllocation;    // whether dynamic reallocation is enabled (proposed vs existing)
        double requestInterval;    // interval (in simulation seconds) between load sensing rounds
    gates:
        input macroIn[5];   output macroOut[5];   // connections to 5 Macro cells
        input microIn[3];   output microOut[3];   // connections to 3 Micro cells
        input picoIn[2];    output picoOut[2];    // connections to 2 Pico cells
}
