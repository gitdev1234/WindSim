#include "MoleculeGroup.h"

/**
 * Constructor of class MoleculeGroup
 *
 * @brief creates a object of class MoleculeGroup
 *
 */
MoleculeGroup::MoleculeGroup() { }

/**
 * Destructor of class MoleculeGroup
 *
 * @brief destroys a object of class MoleculeGroup
 *
 */
MoleculeGroup::~MoleculeGroup() { }

/* --- simulation --- */

// simulation
/**
 * MoleculeGroup::startSimulation()
 *
 * @brief startSimulation sets speed to zero
 *
 */
void MoleculeGroup::startSimulation() {
    vector3 tempSpeed;
    tempSpeed.x = 0;
    tempSpeed.y = 0;
    tempSpeed.z = 0;
    setSpeed(tempSpeed);
};

/**
 * MoleculeGroup::simulateTimeStep(float timeStepInSeconds_)
 *
 * @brief simulateTimeStep simulates the moving of the moleculeGroup
 * @param timeStepInSeconds_ the length in seconds of the simulated timeStep
 * @return coords : returns the new positionInCube, after the simulated timestep
 *
 * executes simulateMoleculesFlow() for to calculate a new positionInCube of
 * the moleculeGroup
 *
 */
vector3 MoleculeGroup::simulateTimeStep(float timeStepInSeconds_){
    vector3 newPositionInCube = simulateMoleculesFlow(timeStepInSeconds_);
    //simulateTemperatureFlow(timeStepInSeconds_);

    return newPositionInCube;
};

/**
 * MoleculeGroup::simulateMoleculesFlow(float timeStepInSeconds_)
 *
 * @brief simulateMoleculesFlow calculates the new positionInCube of a moleculeGroup after a simulated timeStep
 * @param timeStepInSeconds_ the length in seconds of the simulated timeStep
 * @return coords : returns the new positionInCube, after the simulated timestep
 *
 * calculates new positionInCube for MoleculeGroup, depending on
 *   - positionInCube before this timestep
 *   - speed before this timestep
 *   - current acceleration
 *   - length of current timestep
 *
 * new positionInCube is calculated by :
 *   --> newPositionInCube = oldPositionInCube + (oldSpeed + ((newAcceleration * timeStepInSeconds_) * timeStepInSeconds))
 */
vector3 MoleculeGroup::simulateMoleculesFlow(float timeStepInSeconds_) {
    calculateAcceleration();
    calculateSpeed(timeStepInSeconds_);
    // TODO molecule exchange and new coordinates
    vector3 newPositionInCube;
    newPositionInCube.x = 0;
    newPositionInCube.y = 0;
    return newPositionInCube;
};

/**
 * MoleculeGroup::calculateAcceleration()
 *
 * @brief calculates the current acceleration of the moleculeGroup
 *
 * calculates the current acceleration of the moleculeGroup by
 *   --> acceleration = force / mass
 */
void MoleculeGroup::calculateAcceleration() {
    vector3 tempAcceleration;
    vector3 tempForce = getForce();
    float tempMass = getMass();
    tempAcceleration.x = tempForce.x / tempMass;
    tempAcceleration.y = tempForce.y / tempMass;
    tempAcceleration.z = tempForce.z / tempMass;
    setAcceleration(tempAcceleration);
};

/**
 * MoleculeGroup::calculateSpeed(float timeStepInSeconds_)
 *
 * @brief calculates the current speed of the moleculeGroup
 * @param timeStepInSeconds_ the length in seconds of the simulated timeStep
 *
 * calculates the current speed of the moleculeGroup by
 *   --> speed = oldSpeed + (acceleration * timeStepInSeconds)
 */
void MoleculeGroup::calculateSpeed(float timeStepInSeconds_) {
    vector3 tempSpeed;
    vector3 tempAcceleration = getAcceleration();
    tempSpeed.x += (tempAcceleration.x * timeStepInSeconds_);
    tempSpeed.y += (tempAcceleration.y * timeStepInSeconds_);
    tempSpeed.z += (tempAcceleration.z * timeStepInSeconds_);
    setSpeed(tempSpeed);
};
