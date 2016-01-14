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
    double temp = getTemperature();
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
list<airDelta> Cube::calcLeavingAirDeltas(double timeStepInSeconds_) {
    airDelta tempAirDelta;
    tempAirDelta.temperature = getTemperature();

    coords tempCoordsInArea = getCoordsInArea();
    vector3 tempSpeed = getSpeed();
    double tempDensity = calcDensity();
    double tempWidth   = getWidth();
    double tempLength  = getLength();
    bool up = (tempSpeed.y < 0);
    bool left = (tempSpeed.x < 0);
    double tempPhi; // exchange air in kg/s

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

double Cube::calcAirDeltaMoleculesCount(double phi_, double timeStepInSeconds_) {
    double deltaMass = phi_ * timeStepInSeconds_;    // in [kg/s * s] = [kg]
    double deltaMoleculesCount;
    double tempMass = calcMass();
    if (deltaMass > tempMass) { // TODO
        deltaMass = tempMass;
    }
    if (tempMass > 0) {
        double tempMoleculesPerMass = getMoleculesCount() / tempMass;
        deltaMoleculesCount = deltaMass * tempMoleculesPerMass;
    } else {
        deltaMoleculesCount = 0;
    }
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
    double oldMoleculesCount = getMoleculesCount();
    double newMoleculesCount = oldMoleculesCount;
    double newTemperature = getTemperature();

    // subtract outAirDeltas
    for(auto iterateOutAirDeltas = outAirDeltas.begin(); iterateOutAirDeltas != outAirDeltas.end(); iterateOutAirDeltas++) {
        airDelta tempAirDelta = *iterateOutAirDeltas;
        newMoleculesCount -= tempAirDelta.moleculesCount;
    }

    // add inAirDeltas
    for(auto iterateInAirDeltas = inAirDeltas.begin(); iterateInAirDeltas != inAirDeltas.end(); iterateInAirDeltas++) {
        airDelta tempAirDelta = *iterateInAirDeltas;
        double proportionDeltaToCube = tempAirDelta.moleculesCount / newMoleculesCount;
        double oldTemperature = getTemperature();
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

// calculation of acceleration and speed (depending on current forces)

void Cube::calcForce(std::vector<std::vector<Cube> >& Cubes_) {
    coords tempCoordsInArea = getCoordsInArea();
    Cube c = Cubes_[coordsInArea.y][coordsInArea.x];
    double tempLength = Cubes_[tempCoordsInArea.y][tempCoordsInArea.x].getLength();
};

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
    double tempMass = getMass();
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
void Cube::calcSpeed(double timeStepInSeconds_){
    vector3 tempSpeed = getSpeed();
    vector3 tempAcceleration = getAcceleration();
    tempSpeed.x += (tempAcceleration.x * timeStepInSeconds_ * TODO_LIMITOR_FACTOR);
    tempSpeed.y += (tempAcceleration.y * timeStepInSeconds_ * TODO_LIMITOR_FACTOR);
    tempSpeed.z += (tempAcceleration.z * timeStepInSeconds_ * TODO_LIMITOR_FACTOR);
    if (SHOW_IN_DETAIL) {
        cout << "[" << coordsInArea.y << "," << coordsInArea.x << "]:" << tempSpeed.x << "," << tempSpeed.y << "," << tempSpeed.z << endl;
    }
    setSpeed(tempSpeed);

    /*
    //Runge-Kutta
    vector3 oldSpeed = getSpeed();
    vector3 tempAcceleration = getAcceleration();
    tempSpeed.x = oldSpeed.x + timeStepInSeconds_ * ca
    tempSpeed.x = 0.5 * (oldSpeed.x + (timeStepInSeconds_ * calcAcceleration(oldSpeed)));
    tempSpeed.y = 0.5 * (oldSpeed.y + (timeStepInSeconds_ * calcAcceleration(oldSpeed));
    tempSpeed.z = 0.5 * (oldSpeed.z + (timeStepInSeconds_ * calcAcceleration(oldSpeed));
    setSpeed(tempSpeed);
    */
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
double Cube::calcPressure() {
    double tempMoleculesCount = getMoleculesCount();
    double tempTemperature    = getTemperature();
    double tempVolume         = getVolume();
    double tempPressure = (tempMoleculesCount * BOLTZMANN_CONST * tempTemperature) / tempVolume;
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
double Cube::calcDensity() {
    double tempTemperature = getTemperature();
    double tempPressure = calcPressure();
    double tempDensity = tempPressure / (INDIVIDUAL_GAS_CONST * tempTemperature);
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
double Cube::calcMass() {
    double tempDensity = calcDensity();
    double tempMass = tempDensity * getVolume();
    setMass(tempMass);
    return tempMass;
};

/**
@brief calculates a new temperature for this cube and for all moleculeGroups within this cube.
       the new temperature is the average temperature of the temperatures of all moleculeGroups within this cube.
       this simple calculation can be used, because every moleculegroup represents the same number of molecules and
       though has the same mass and volume within this cube
*/
double Cube::calcTemperature() {
    // TODO v1.0
    double averageTemperature = 0;
    return averageTemperature;
}


double Cube::calcMoleculesCount() {
    //todo v1.0
    return 0;
};
