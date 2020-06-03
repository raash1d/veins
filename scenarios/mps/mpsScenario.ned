//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/.
// 

import org.car2x.veins.nodes.Scenario;
import org.car2x.veins.modules.mobility.traci.TraCIScenario*;
import org.car2x.veins.modules.obstacle.VehicleObstacleControl;
import org.car2x.veins.base.connectionManager.ConnectionManager;
import org.car2x.veins.mps.traffic.TrafficManager;


network mpsScenario extends Scenario {
    submodules:
        vehicleObstacles: VehicleObstacleControl {
            @display("p=30,138");
        }
        manager: TraCIScenarioManagerForker {
            parameters:
                @display("p=115,60");
        }
        trafficManager: TrafficManager {
            @display("p=115,0");
        }
}
