/**
    Cube.cpp
    Purpose: Implements Class Cube

    @author Wall.Of.Death
    @version 1.0 20151120
*/

#include "../include/Cube.h"
#include "MoleculeGroup.h"

/* --- constructor and destructor --- */

/**
 * Constructor of class Cube
 *
 * @brief creates a object of class Cube
 *
 */
Cube::Cube() { };

/**
 * Destructor of class Cube
 *
 * @brief destroys a object of class Cube
 *
 */
Cube::~Cube() { };

/* --- miscellaneous --- */
// temperature

/**
 * Cube::modifyTemperature(string s_)
 *
 * @brief increments or decrements the temperature of the cube by MODIFY_TEMPERATURE_DELTA
 * @param s_ enter '+' for increment, enter '-' for decrement
 *
 *
 * if s == '+' the current temperature of the cube gets incremented by MODIFY_TEMPERATURE_DELTA
 * if s == '-' the current temperature of the cube gets decremented by MODIFY_TEMPERATURE_DELTA
 * executes calcPressure after setting new temperature
 *
 */
void Cube::modifyTemperature(string s_){
    float temp = getTemperature();
    if (s_[0] == '+') {
        temp += MODIFY_TEMPERATURE_DELTA;
    }
    if (s_[0] == '-') {
        temp -= MODIFY_TEMPERATURE_DELTA;
    }
    setTemperature(temp);

    calcPressure();
};


/* --- simulation --- */

// simulation
/**
 * Cube::initSimulation(int moleculeGroupsPerCube_)
 *
 * @brief creates and initializes moleculeGroups
 * @param moleculeGroupsPerCube has to be a square number e.g. 100, 1024, 10000
 * @todo later versions : z coordinate is not used
 *
 *
 * --> sets moleculeGroupsPerCube
 * --> creates and initializes moleculeGroups
 *      --> sets attributes
 *      --> for example : calculate position of every moleculeGroup within the cube
 *      --> executes moleculeGroup.initSimulation() for ever moleculeGroup
 *
 * the moleculeGroups are distributed linear in the cube
 * for example see the following pictures
 *
 *  +------------+  +-----------+
 +  |            |  | M   M   M |
 *  |   M    M   |  |           |
 *  |            |  | M   M   M |
 *  |   M    M   |  |           |
 *  |            |  | M   M   M |
 *  +------------+  +-----------+
 *
 * ATTENTION : NOTE THAT THE PARAMETER moleculeGroupsPerCube HAS TO BE A SQUARE NUMBER
 *
 *
 */
void Cube::initSimulation(int moleculeGroupsPerCube_) {
    MoleculeGroup tempMoleculeGroup;
    setMoleculeGroupsPerCube(moleculeGroupsPerCube_);

    // get temperature of this cube
    float tempTemperature = getTemperature();

    // calculate moleculesPerMoleculeGroup
    float tempMoleculesCount = getMoleculesCount();
    float tempMoleculesPerMoleculeGroup = tempMoleculesCount / moleculeGroupsPerCube_;
    setMoleculesPerMoleculeGroup(tempMoleculesPerMoleculeGroup);

    // calculate mass per moleculeGroup
    float tempMass = calcMass();
    float massPerMoleculeGroup = tempMass / moleculeGroupsPerCube_;

    // calculations before loop
    int moleculesInXDirection = round(sqrt(moleculeGroupsPerCube_));
    int moleculesInYDirection = moleculesInXDirection;
    float tempWidth = getWidth();
    float tempLength = getLength();
    float distanceMoleculesInXDirection = (tempWidth / moleculesInXDirection);
    float distanceMoleculesInYDirection = (tempLength / moleculesInYDirection);


    float tempX = 0, tempY = 0;
    vector3 tempPosition;

    // calculate positions of moleculeGroups in cube and add them to
    // moleculeGroups-list of cube
    tempX = 0 - (distanceMoleculesInXDirection / 2);
    for (float x = 0; x < moleculesInXDirection; x++) {
        tempX += distanceMoleculesInXDirection;
        tempY = 0 - (distanceMoleculesInYDirection / 2);
        for (float y = 0; y < moleculesInYDirection; y++) {
            tempY += distanceMoleculesInYDirection;
            tempPosition.x = tempX;
            tempPosition.y = tempY;
            tempPosition.z = 0;
            tempMoleculeGroup.setTemperature(tempTemperature);
            tempMoleculeGroup.setPositionInCube(tempPosition);
            tempMoleculeGroup.setMass(massPerMoleculeGroup);
            tempMoleculeGroup.setMoleculesCount(tempMoleculesPerMoleculeGroup);
            coords tempCoordsInArea = getCoordsInArea();
            tempMoleculeGroup.setCoordsOfCube(tempCoordsInArea);
            tempMoleculeGroup.initSimulation();
            moleculeGroups.push_front(tempMoleculeGroup);
        }
    }
    cout << "created " << moleculeGroups.size() << " molecules." << endl;
}

/**
 * Cube::simulateTimeStep(float timeStepInSeconds_)
 *
 * @brief executes simulateMoleculesFlow() and simulateTemperatureFlow()
 * @param timeStepInSeconds_ the length in seconds of the simulated timeStep
 *
 * executes the functions simulateMoleculesFlow() and simulateTemperatureFlow()
 *
 */
void Cube::simulateTimeStep(float timeStepInSeconds_) {
    simulateMoleculesFlow(timeStepInSeconds_);
    //simulateTemperatureFlow(timeStepInSeconds_);
}

/**
 * Cube::simulateMoleculesFlow(float timeStepInSeconds_)
 *
 * @brief simulates the moving of all moleculeGroups within this cube
 * @param timeStepInSeconds_ the length in seconds of the simulated timeStep
 *
 * simulates the moving of all moleculeGroups within this cube depending on
 * the force attribute of the class.
 *
 *
 *
     // iterate list of moleculegroups
      // --> getForce
      // --> simulateTimeStep
      // --> check if newPositionInCube is out of cube
      //      --> check if the moleculeGroup may be handed over to new cube
      //           --> hand moleculeGroup over to different cube
      //      --> or if
 *
 *
 *
   e-------f
  /|      /|
 / |     / |
a--|----b  |
|  g----|--h
| /     | /
|/      |/
c-------d

x-axis --> width  = c-->d
y-axis --> length = d-->h
z-axis --> height = d-->b

*/
void Cube::simulateMoleculesFlow(float timeStepInSeconds_) {

    list<MoleculeGroup> tempMoleculeGroupsWhichAreLeavingTheCube;
    vector3 tempForce = getForce();
    for(auto iterateMoleculeGroups = moleculeGroups.begin(); iterateMoleculeGroups != moleculeGroups.end(); iterateMoleculeGroups++) {
        iterateMoleculeGroups->setForce(tempForce);
        vector3 newPositionInCube = iterateMoleculeGroups->simulateTimeStep(timeStepInSeconds_);

        // check if newPositionInCube is out of Cube
        float tempHeight = getHeight();
        float tempWidth  = getWidth();
        float tempLength = getLength();
        bool isOutOfCube = false;
        coords tempCoordsOfCube = iterateMoleculeGroups->getCoordsOfCube();
        coords tempMaxCoordsInArea = getMaxCoordsInArea();


        if (newPositionInCube.x < 0) {                             // out in the minus direction of x-axis
            tempCoordsOfCube.x = tempCoordsOfCube.x - 1;           // calculate x coordinate of new cube
            if (tempCoordsOfCube.x >= 0) {                             // if new cube is still in area
                isOutOfCube = true;                                       // allow leaving cube
                newPositionInCube.x = tempWidth + newPositionInCube.x;    // calculate new x coordinate of moleculeGroup
            } else {                                                   // else do not allow leaving cube
                tempCoordsOfCube.x = 0;                                   // moleculeGroup stays in cube
                newPositionInCube.x = 0;                                  //          --||--
            }
        } else if (newPositionInCube.x > tempWidth) {              // out in the plus direction of x-axis
            tempCoordsOfCube.x = tempCoordsOfCube.x + 1;           // calculate x coordinate of new cube
            if (tempCoordsOfCube.x < tempMaxCoordsInArea.x) {         // if new cube is still in area
                isOutOfCube = true;                                       // allow leaving cube
                newPositionInCube.x = newPositionInCube.x - tempWidth;    // calculate new x coordinate of moleculeGroup
            } else {                                                   // else do not allow leaving cube
                tempCoordsOfCube.x = tempMaxCoordsInArea.x - 1;           // moleculeGroup stays in cube
                newPositionInCube.x = tempWidth;                          //          --||--
            }
        }

        if (newPositionInCube.y < 0) {                             // out in the minus direction of y-axis
            tempCoordsOfCube.y = tempCoordsOfCube.y - 1;           // calculate y coordinate of new cube
            if (tempCoordsOfCube.y >= 0) {                              // if new cube is still in area
                isOutOfCube = true;                                       // allow leaving cube
                newPositionInCube.y = tempLength + newPositionInCube.y;   // calculate new y coordinate of moleculeGroup
            } else {                                                   // else do not allow leaving cube
                tempCoordsOfCube.y = 0;                                   // moleculeGroup stays in cube
                newPositionInCube.y = 0;                                  //          --||--
            }
        } else if (newPositionInCube.y > tempLength) {             // out in the plus direction of y-axis
            tempCoordsOfCube.y = tempCoordsOfCube.y + 1;           // calculate y coordinate of new cube
            if (tempCoordsOfCube.y < tempMaxCoordsInArea.y) {          // if new cube is still in area
                isOutOfCube = true;                                       // allow leaving cube
                newPositionInCube.y = newPositionInCube.y - tempLength;   // calculate new y coordinate of moleculeGroup
            } else {                                                   // else do not allow leaving cube
                tempCoordsOfCube.y = tempMaxCoordsInArea.y - 1;          // moleculeGroup stays in cube
                newPositionInCube.y = tempLength;                        //          --||--
            }
        }

        /*@todo later versions z - axis
        if (newPositionInCube.z < 0) {                             // out in the minus direction of z-axis
            tempCoordsOfCube.z = tempCoordsOfCube.z - 1;           // calculate z coordinate of new cube
            if (tempCoordsOfCube.z >= 0) {                             // if new cube is still in area
                isOutOfCube = true;                                       // allow leaving cube
                newPositionInCube.z = tempHeight + newPositionInCube.z;   // calculate new z coordinate of moleculeGroup
            } else {                                                   // else do not allow leaving cube
                tempCoordsOfCube.z = 0;                                   // moleculeGroup stays in cubes
                newPositionInCube.z = 0;                                  //          --||--
            }
        } else if (newPositionInCube.z > tempHeight) {             // out in the plus direction of z-axis
            tempCoordsOfCube.z = tempCoordsOfCube.z + 1;           // calculate z coordinate of new cube
            if (tempCoordsOfCube.z < tempMaxCoordsInArea.z) {          // if new cube is still in area
                isOutOfCube = true;                                       // allow leaving cube
                newPositionInCube.z = newPositionInCube.z - tempHeight;   // calculate new z coordinate of moleculeGroup
            } else {                                                   // else do not allow leaving cube
                tempCoordsOfCube.z = tempMaxCoordsInArea.z - 1;           // moleculeGroup stays in cubes.
                newPositionInCube.z = tempHeight;
            }
        }
        */

        iterateMoleculeGroups->setPositionInCube(newPositionInCube);
        if (isOutOfCube) {
            iterateMoleculeGroups->setCoordsOfCube(tempCoordsOfCube);
            tempMoleculeGroupsWhichAreLeavingTheCube.push_front(*iterateMoleculeGroups);
            moleculeGroups.erase(iterateMoleculeGroups--);
            iterateMoleculeGroups++;

        }

    }

    setMoleculeGroupsWhichAreLeavingTheCube(tempMoleculeGroupsWhichAreLeavingTheCube);

};

void Cube::addMoleculeGroup(MoleculeGroup moleculeGroup_) {
    moleculeGroups.push_front(moleculeGroup_);
    setMoleculeGroupsPerCube(moleculeGroups.size());
};

// calculation of forces

void Cube::clearForce() {
    force.x = 0;
    force.y = 0;
    force.z = 0;
};

void Cube::addForce(vector3 force_) {
    force.x += force_.x;
    force.y += force_.y;
    force.z += force_.z;
};

vector3 Cube::getForce() {
    return force;
};

// calculation of attributes
/**
 * Cube::recalculateAttributes(changeType changeType_)
 *
 * @brief recalculates the attributes during simulation
 * @param changeType_ says wether moleculeGroupsPerCube or temperature has changed
 *
 * if moleculeGroupsPerCube has changed the following attributes have to be recalculated
 *  - moleculesCount
 *  - temperature
 *  - pressure
 *  - mass
 *
 *
 */
void Cube::recalculateAttributes(changeType changeType_) {
    if (changeType_ == changeType::MOLECULE_FLOW) {
        calcMoleculeGroupsPerCube();
        calcMoleculesCount();
        calcTemperature();
        calcMass();
        calcPressure();
    } else if (changeType_ == changeType::TEMPERATUR_FLOW) {
        //TODO
    }

};

// pressure
/**
 * Cube::calcPressure()
 *
 * @brief calculates the current pressure within the cube
 * @return returns the current pressure within the cube
 *
 * calculates the current pressure within the cube by
 *  --> pressure = (moleculesCount * BOLTZMANN_CONST * temperature) / volume
 *
 */
float Cube::calcPressure() {
    float tempMoleculesCount = getMoleculesCount();
    float tempTemperature    = getTemperature();
    float tempVolume         = getVolume();
    float tempPressure = (tempMoleculesCount * BOLTZMANN_CONST * tempTemperature) / tempVolume;
    setPressure(tempPressure);
    return tempPressure;
};

// mass
/**
 * Cube::calcMass()
 *
 * @brief calculates the current mass of all the air within the cube
 * @return returns the current mass of all the air within the cube
 *
 * calculates the current mass of all the air within the cube by
 *  --> mass = density / volume
 *
 */
float Cube::calcMass() {
    float tempPressure    = calcPressure();
    float tempTemperature = getTemperature();
    float tempDensity = tempPressure / (INDIVIDUAL_GAS_CONST * tempTemperature);
    float tempMass = tempDensity * getVolume();
    setMass(tempMass);
    return tempMass;
};

/**
@brief calculates a new temperature for this cube and for all moleculeGroups within this cube.
       the new temperature is the average temperature of the temperatures of all moleculeGroups within this cube.
       this simple calculation can be used, because every moleculegroup represents the same number of molecules and
       though has the same mass and volume within this cube
*/
float Cube::calcTemperature() {
    float sum = 0;
    for(auto iterateMoleculeGroups = moleculeGroups.begin(); iterateMoleculeGroups != moleculeGroups.end(); iterateMoleculeGroups++) {
        MoleculeGroup tempMoleculeGroup = *iterateMoleculeGroups;
        sum += tempMoleculeGroup.getTemperature();
    }
    float tempMoleculeGroupsPerCube = getMoleculeGroupsPerCube();
    float averageTemperature = sum / tempMoleculeGroupsPerCube;
    setTemperature(averageTemperature); // this automatically sets the temperature of all moleculeGroups in this cube
    return averageTemperature;
}

float Cube::calcMoleculeGroupsPerCube() {
    float tempMoleculeGroupsPerCube = moleculeGroups.size();
    setMoleculeGroupsPerCube(tempMoleculeGroupsPerCube);
    return tempMoleculeGroupsPerCube;
};

float Cube::calcMoleculesCount() {
    float tempMoleculeGroupsPerCube = getMoleculeGroupsPerCube();
    float tempMoleculesPerMoleculeGroup = getMoleculesPerMoleculeGroup();
    float tempMoleculesCount = tempMoleculeGroupsPerCube * tempMoleculesPerMoleculeGroup;
    setMoleculesCount(tempMoleculesCount);
    return tempMoleculesCount;
};
