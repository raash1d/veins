//-----------------------------------------------------------
// @author Raashid Ansari
// @date   06-29-2018
// @brief  This class contains commands to generate vehicles
//     in SUMO. It also provides functions to get details
//     about the scenario where a simulation is running
//-----------------------------------------------------------

#include <veins/mps/traffic/TrafficManager.h>

Define_Module(TrafficManager);

TrafficManager::TrafficManager()
{
}

TrafficManager::~TrafficManager()
{
}

void TrafficManager::initialize(int stage)
{
    cSimpleModule::initialize(stage);
    if (stage == 0) {
        initScenario_ = false;
        laneIds_.clear();
        junctionIds_.clear();
        routeIds_.clear();
        vehTypeIds_.clear();

        traciScenarioManager_ = veins::FindModule<veins::TraCIScenarioManager*>::findGlobalModule();

        nCars_ = par("nCars").intValue();
        arrivalTime_ = par("arrivalTime").doubleValue();

        laneRandomize_ = par("laneRandomize").boolValue();
        routeRandomize_ = par("routeRandomize").boolValue();

        addCar_ = new cMessage("addCar");
        scheduleAt(1.0, addCar_);
    }
}

void TrafficManager::finish()
{
    if (addCar_->isScheduled()) {
        cancelAndDelete(addCar_);
        addCar_ = 0;
    }
    else if (addCar_) {
        delete (addCar_);
    }
}

void TrafficManager::handleMessage(cMessage* msg)
{
    if (msg == addCar_) {
        // create vehicles
        if (nCars_ > 0) {
            createVehicle(nCars_);
            nCars_--;
            // randomize arrival time of vehicles
            auto const arrivalTime = simTime() + fmod(dblrand(0), arrivalTime_ + 0.0001);
            scheduleAt(arrivalTime, addCar_);
        }
    }
    else {
        error("Traffic Manager received unknown Message: %s", msg->getName());
    }
}

void TrafficManager::loadTrafficInfo()
{
    traciCmdInterface_ = veins::FindModule<veins::TraCIScenarioManager*>::findGlobalModule()->getCommandInterface();
    if (roadIds_.size() == 0) { // Use condition road_ids.size() != 0 and throw error if not empty, Do this for all following conditions.
        auto roadList = traciCmdInterface_->getRoadIds();
        for (auto& road : roadList) {
            EV << "Got road: " << road << std::endl;
            roadIds_.insert(std::pair<std::string, int>(road, 0));
        }
    }

    if (laneIds_.size() == 0) {
        auto laneList = traciCmdInterface_->getLaneIds();
        for (auto& lane : laneList) {
            EV << "Adding lane: " << lane << std::endl;
            laneIds_.push_back(lane);

            auto roadId = traciCmdInterface_->lane(lane).getRoadId();
            roadIds_.find(roadId)->second++;
            roadIdOfLane_[roadId].push_back(lane);
        }
    }

    if (routeIds_.size() == 0) {
        auto routeList = traciCmdInterface_->getRouteIds();
        for (auto& route : routeList) {
            EV << "Adding route: " << route << std::endl;
            routeIds_.push_back(route);

            auto roadsInRoute = traciCmdInterface_->route(route).getRoadIds();
            auto firstRoadInRoute = roadsInRoute.begin();
            routeStartLaneIds_[route] = roadIdOfLane_[*firstRoadInRoute];
        }
    }

    if (junctionIds_.size() == 0) {
        auto junctionList = traciCmdInterface_->getJunctionIds();
        for (auto& junction : junctionList) {
            EV << "Adding junction: " << junction << std::endl;
            junctionIds_.push_back(junction);
        }
    }

    if (vehTypeIds_.size() == 0) {
        auto vehTypeList = traciCmdInterface_->getVehicleTypeIds();
        for (auto& vehType : vehTypeList) {
            if (vehType != "DEFAULT_PEDTYPE") {
                EV << "Adding vehicle type: " << vehType << std::endl;
                vehTypeIds_.push_back(vehType);
            }
        }
    }
}

void TrafficManager::createVehicle(int const vehicleIdx)
{
    if (!initScenario_) {
        loadTrafficInfo();
        initScenario_ = true;
    }

    auto routeIndex = routeRandomize_ ? intrand(routeIds_.size(), 0) : 0;

    auto traciVehicleType = vehTypeIds_[1];
    auto traciVehicleId = std::string("genuine");

    auto os = std::stringstream{};
    os << traciVehicleId << vehicleIdx;

    // Prepare all values for addVehicle()
    traciVehicleId = os.str();
    // traciVehicleType is already ready
    auto routeId = routeIds_[routeIndex];
    auto emitTime = traciCmdInterface_->DEPART_TIME_NOW;
    auto emitPos = traciCmdInterface_->DEPART_POSITION_BASE;
    auto emitSpeed = traciCmdInterface_->DEPART_SPEED_MAX;
    auto emitLane = laneRandomize_ ? traciCmdInterface_->DEPART_LANE_RANDOM : traciCmdInterface_->DEPART_LANE_BEST;

    // The following method, addVehicle(), returns a boolean value indicating
    // whether a vehicle insertion was successful. It may be useful to store
    // that value in a variable for debugging later on.
#ifdef DEBUG
    bool result =
#endif

        traciCmdInterface_->addVehicle(
            traciVehicleId,
            traciVehicleType,
            routeId,
            emitTime,
            emitPos,
            emitSpeed,
            emitLane);

#ifdef DEBUG
    std::cout << "Vehicle insertion ";
    result ? std::cout << "SUCCESS" : std::cout << "FAILURE";
    std::cout << " at " << simTime() << std::endl;
    std::cout << "BaseTrafficManager: createVehicle end" << std::endl;
#endif
}
