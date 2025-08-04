// BaseStation.ned
package hcran;

simple BaseStation
{
    parameters:
        string type;          // "Macro", "Micro", or "Pico" (cell type)
        int userLoad = default(0);       // current user load (can be updated during simulation)
        int initialChannels = default(0); // initial channel allocation (for reference)
    gates:
        input in;   // receives messages (e.g., load requests) from RRH
        output out; // sends messages (e.g., load replies) to RRH
}
