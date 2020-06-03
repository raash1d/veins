//-----------------------------------------------------------
// @author Raashid Ansari
// @date   06-29-2018
// @brief  This class contains commands to generate vehicles
//     in SUMO. It also provides functions to get details
//     about the scenario where a simulation is running
//-----------------------------------------------------------

package org.car2x.veins.mps.traffic;

simple TrafficManager {
    parameters:
        int  nCars          = default(0);
        bool laneRandomize  = default(false);
        bool routeRandomize = default(false);
        double arrivalTime  = default(2.0s) @unit("s");
        
        @class(TrafficManager);
}
