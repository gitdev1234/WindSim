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
 * TODO v1.0
 */
void Cube::initSimulation() {

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
 * @param changeType_ says wether air or temperature has exchanged
 *
 * if air has exchanged the following attributes have to be recalculated
 *  - moleculesCount (is already set during simulateAirExchange())
 *  - temperature
 *  - density
 *  - pressure
 *  - mass
 *
 */
void Cube::recalculateAttributes(changeType changeType_) {
    if (changeType_ == changeType::MOLECULE_FLOW) {
        calcTemperature();
        calcDensity();
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

// density
/**
 * Cube::calcDensity()
 *
 * @brief calculates the current density of the air within the cube
 * @return returns the current density of all the air within the cube
 *
 * calculates the current density of all the air within the cube by
 *  --> density = pressure / (individual_gas_const * temperature)
 *
 */
float Cube::calcDensity() {
    float tempTemperature = getTemperature();
    float tempPressure = calcPressure();
    float tempDensity = tempPressure / (INDIVIDUAL_GAS_CONST * tempTemperature);
    setDensity(tempDensity);
    return tempDensity;
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
    float tempDensity = calcDensity();
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
    // TODO v1.0
    float averageTemperature = 0;
    return averageTemperature;
}


float Cube::calcMoleculesCount() {
    //todo v1.0
    return 0;
};
