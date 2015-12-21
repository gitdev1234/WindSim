/**
    Cube.cpp
    Purpose: Implements Class Cube

    @author Wall.Of.Death
    @version 1.0 20151120
*/

#include "../include/Cube.h"

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

}

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
    // TODO
    float averageTemperature = 0;
    return averageTemperature;
}


float Cube::calcMoleculesCount() {
    float tempMoleculeGroupsPerCube = getMoleculeGroupsPerCube();
    float tempMoleculesPerMoleculeGroup = getMoleculesPerMoleculeGroup();
    float tempMoleculesCount = tempMoleculeGroupsPerCube * tempMoleculesPerMoleculeGroup;
    setMoleculesCount(tempMoleculesCount);
    return tempMoleculesCount;
};
