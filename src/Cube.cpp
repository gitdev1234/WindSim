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

bool  Cube::checkCoordsStillInArea(coords c_) {
    bool temp = true;
    coords tempMaxCoordsInArea = getMaxCoordsInArea();
    if ( (c_.x < 0) || (c_.x > tempMaxCoordsInArea.x)) {
        temp = false;
    }
    if ( (c_.y < 0) || (c_.y > tempMaxCoordsInArea.y)) {
        temp = false;
    }
    return temp;
};


/* --- simulation --- */

// simulation
/**
 * Cube::initSimulation(int moleculeGroupsPerCube_)
 *
 * @brief sets speed to 0;
 */
void Cube::initSimulation() {
    vector3 tempSpeed;
    tempSpeed.x = 0;
    tempSpeed.y = 0;
    tempSpeed.z = 0;
    setSpeed(tempSpeed);

}

/**
 *
 *
 *                    x=0,y<0,z=0 up
 *                   |
 *  x<0,y<0,z=0  ^   ^   ^  x>0,y<0,z=0 rightUp
 *   leftUp       \  |  /
 *                 \ | /
 *                  \|/
 *  x<0,y=0,z=0 <----|----> x>0,y=0,z=0 right
 *   left           /|\
 *                 / | \
 *                /  |  \
 *  x<0,y>0,z=0  v   v   v  x>0,y>0,z=0 rightDown
 *   leftDown        |
 *                    x=0,y>0,z=0 down
 *
 *
 *
 *
 *  -------------------- --------------------
 * | Cube_1.1      ->   | Cube_1.2           |
 * |              v_1.1 |                    |
 * |                \   |                    |
 * |       ->      ( \  |                    |
 * |      v_1.1   (   \ |A_1.1               |
 * |         \   (alpha\|                    |
 * |       (  \  -------\                    |
 * |      (    \        |\                   |
 * |     (alpha \       | v                  |
 *  -------------\------ --------------------
 * |      A_2.1   \     |                    |
 * |               v    |                    |
 * |                    |                    |
 * |                    |                    |
 * |                    |                    |
 * |                    |                    |
 * |                    |                    |
 * |                    |                    |
 * | Cube_2.1           | Cube_2.2           |
 *  -------------------- --------------------
 *                                    -->
 * phi_Cube_1.2 = density * A_1.1 * |v_1.1| * cos(alpha)
 *                                   -->
 * phi_Cube_1.2 = density * A_1.1 * v_1.1.x
 *
 *                                    -->
 * phi_Cube_2.1 = density * A_2.1 * |v_1.1| * sin(alpha)
 *                                   -->
 * phi_Cube_2.1 = density * A_2.1 * v_1.1.y
 *
 * [kg * m^2 * m]   [kg]
 * [------------] = [--]
 * [m^3 * s     ]   [s ]
 *
 */
list<airDelta> Cube::calcLeavingAirDeltas(float timeStepInSeconds_) {
    airDelta tempAirDelta;
    tempAirDelta.temperature = getTemperature();

    coords tempCoordsInArea = getCoordsInArea();
    vector3 tempSpeed = getSpeed();
    float tempDensity = calcDensity();
    float tempWidth   = getWidth();
    float tempLength  = getLength();
    bool up = (tempSpeed.y < 0);
    bool left = (tempSpeed.x < 0);
    float tempPhi; // exchange air in kg/s

    if (tempSpeed.y != 0) {
        if (up) {
            tempAirDelta.newCoords.x = tempCoordsInArea.x;
            tempAirDelta.newCoords.y = tempCoordsInArea.y - 1;

        } else { // down
            tempAirDelta.newCoords.x = tempCoordsInArea.x;
            tempAirDelta.newCoords.y = tempCoordsInArea.y + 1;
        }
        if (checkCoordsStillInArea(tempAirDelta.newCoords)) {
            if (tempSpeed.y < 0) {
                    tempSpeed.y *= -1;
            }
            tempPhi = tempDensity * tempWidth * tempSpeed.y; // in [kg/s]
            tempAirDelta.moleculesCount = calcAirDeltaMoleculesCount(tempPhi,timeStepInSeconds_);
            outAirDeltas.push_front(tempAirDelta);
        }
    }

    if (tempSpeed.x != 0) {
        if (left) {
            tempAirDelta.newCoords.x = tempCoordsInArea.x - 1;
            tempAirDelta.newCoords.y = tempCoordsInArea.y;
        } else { // right
            tempAirDelta.newCoords.x = tempCoordsInArea.x + 1;
            tempAirDelta.newCoords.y = tempCoordsInArea.y;
        }
        if (checkCoordsStillInArea(tempAirDelta.newCoords)) {
            if (tempSpeed.x < 0) {
                    tempSpeed.x *= -1;
            }
            tempPhi = tempDensity * tempLength * tempSpeed.x; // in [kg/s]
            tempAirDelta.moleculesCount = calcAirDeltaMoleculesCount(tempPhi,timeStepInSeconds_);
            outAirDeltas.push_front(tempAirDelta);
        }
    }

    return outAirDeltas;
}

float Cube::calcAirDeltaMoleculesCount(float phi_, float timeStepInSeconds_) {
    float deltaMass = phi_ * timeStepInSeconds_;    // in [kg/s * s] = [kg]
    float tempMass = calcMass();
    if (deltaMass > tempMass) { // TODO
        deltaMass = tempMass;
    }
    float tempMoleculesPerMass = getMoleculesCount() / tempMass;
    float deltaMoleculesCount = deltaMass * tempMoleculesPerMass;
    return deltaMoleculesCount;
}

void Cube::clearAirDeltas(){
    inAirDeltas.clear();
    outAirDeltas.clear();
};


// calculation of attributes
/**
 * Cube::recalculateAttributes()
 *
 * @brief recalculates the attributes during simulation
 *
 * adds the molecules within the inAirDelta-list
 * adds the temperature of the molecules by proportion
 * calculates mass, density and pressure
 *
 */
void Cube::recalculateAttributes() {
    float oldMoleculesCount = getMoleculesCount();
    float newMoleculesCount = oldMoleculesCount;
    float newTemperature = getTemperature();

    // subtract outAirDeltas
    for(auto iterateOutAirDeltas = outAirDeltas.begin(); iterateOutAirDeltas != outAirDeltas.end(); iterateOutAirDeltas++) {
        airDelta tempAirDelta = *iterateOutAirDeltas;
        newMoleculesCount -= tempAirDelta.moleculesCount;
    }

    // add inAirDeltas
    for(auto iterateInAirDeltas = inAirDeltas.begin(); iterateInAirDeltas != inAirDeltas.end(); iterateInAirDeltas++) {
        airDelta tempAirDelta = *iterateInAirDeltas;
        float proportionDeltaToCube = tempAirDelta.moleculesCount / newMoleculesCount;
        float oldTemperature = getTemperature();
        newTemperature = (proportionDeltaToCube * tempAirDelta.temperature) + ((1-proportionDeltaToCube) * oldTemperature);
        newMoleculesCount += tempAirDelta.moleculesCount;
    }

    // set new values
    setMoleculesCount(newMoleculesCount);
    setTemperature(newTemperature);
    calcMass();
    calcDensity();
    calcPressure();
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

// calculation of acceleration and speed (depending on current forces)

/**
 * Cube::calcAcceleration()
 *
 * @brief calculates the current acceleration of the air within the cube
 *
 * calculates the current acceleration of the air within the cube:
 *  --> acceleration = force / mass
 */
void Cube::calcAcceleration() {
    vector3 tempForce = getForce();
    float tempMass = getMass();
    vector3 tempAcceleration;
    tempAcceleration.x = tempForce.x / tempMass;
    tempAcceleration.y = tempForce.y / tempMass;
    tempAcceleration.z = tempForce.z / tempMass;
    setAcceleration(tempAcceleration);
};

/**
 * Cube::calcSpeed()
 *
 * @brief calculates the current speed of the air within the cube
 *
 * calculates the current speed of the air within the cube:
 *  --> newSpeed = speed_before_timeStep + (acceleration * time)
 *
 * !!! ATTENTION !!!
 * always call init-simulation at the beginning of every simulation
 * --> this sets the speed back to 0
 * always call calcAcceleration before calcSpeed
 * --> this ensures, that calcSpeed uses the right acceleration-value
 */
void Cube::calcSpeed(float timeStepInSeconds_){
    vector3 tempSpeed = getSpeed();
    vector3 tempAcceleration = getAcceleration();
    tempSpeed.x += (tempAcceleration.x * timeStepInSeconds_);
    tempSpeed.y += (tempAcceleration.y * timeStepInSeconds_);
    tempSpeed.z += (tempAcceleration.z * timeStepInSeconds_);
    setSpeed(tempSpeed);
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
