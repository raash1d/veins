//-----------------------------------------------------------
// @author Raashid Ansari
// @date   06-29-2018
// @brief  This class contains commands to generate vehicles
//     in SUMO. It also provides functions to get details
//     about the scenario where a simulation is running
//-----------------------------------------------------------

#pragma once

#include <map>
#include <list>
#include <ctime>
#include <string>
#include <vector>
#include <stdint.h>

#include <omnetpp/simtime_t.h>
#include <omnetpp/csimplemodule.h>

#include <veins/modules/mobility/traci/TraCIScenarioManager.h>
#include <veins/modules/mobility/traci/TraCICommandInterface.h>

/**
 * Class to create and control vehicles in a scenario
 */
class TrafficManager : public omnetpp::cSimpleModule {
public:
    TrafficManager();
    ~TrafficManager();
    virtual void initialize(int stage);
    virtual void finish();

protected:
    void createVehicle(const int vehicleIdx);
    virtual void handleMessage(cMessage* msg);

protected:
    long nCars_;
    cMessage* addCar_;
    enum CarCreationOrder {
        ATTACKER_CAR_FIRST = -1,
        GENUINE_CAR_FIRST = 0,
        RANDOM_SELECTION = 1,
    };

    //    typedef std::list<std::string> StringList;
    typedef std::vector<std::string> StringVector_t;
    typedef std::map<std::string, StringVector_t> StringStringVectorMap_t;

    std::map<std::string, int> roadIds_; // key=road_id, value=number_of_lanes
    StringVector_t laneIds_;
    StringVector_t junctionIds_;
    StringVector_t routeIds_;
    StringVector_t vehTypeIds_;
    StringStringVectorMap_t roadIdOfLane_;
    StringStringVectorMap_t routeStartLaneIds_;
    veins::TraCIScenarioManager* traciScenarioManager_;
    veins::TraCICommandInterface* traciCmdInterface_;
    bool initScenario_;
    bool laneRandomize_;
    bool routeRandomize_;

private:
    void loadTrafficInfo();

private:
    double arrivalTime_;
};
