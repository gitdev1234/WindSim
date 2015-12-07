#include "MoleculeGroup.h"

MoleculeGroup::MoleculeGroup() {
    //ctor
}

MoleculeGroup::~MoleculeGroup() {
    //dtor
}

/* --- simulation --- */

// simulation
void MoleculeGroup::startSimulation() {
    speed.x = 0;
    speed.y = 0;
    speed.z = 0;
};


coords MoleculeGroup::simulateTimeStep(float timeStepInSeconds_){
    coords tempCoords = simulateMoleculesFlow(timeStepInSeconds_);
    //simulateTemperatureFlow(timeStepInSeconds_);

    return tempCoords;
};

coords MoleculeGroup::simulateMoleculesFlow(float timeStepInSeconds_) {
    calculateAcceleration();
    calculateSpeed(timeStepInSeconds_);
    // TODO molecule exchange and new coordinates
    coords tempCoords;
    tempCoords.x = 0;
    tempCoords.y = 0;
    return tempCoords;
};

void MoleculeGroup::calculateAcceleration() {
    vector3 tempAcceleration;
    vector3 tempForce = getForce();
    float tempMass = getMass();
    tempAcceleration.x = tempForce.x / tempMass;
    tempAcceleration.y = tempForce.y / tempMass;
    tempAcceleration.z = tempForce.z / tempMass;
    acceleration = tempAcceleration;
};

void MoleculeGroup::calculateSpeed(float timeStepInSeconds_) {
    vector3 tempSpeed;
    vector3 tempAcceleration = getAcceleration();
    tempSpeed.x += (tempAcceleration.x * timeStepInSeconds_);
    tempSpeed.y += (tempAcceleration.y * timeStepInSeconds_);
    tempSpeed.z += (tempAcceleration.z * timeStepInSeconds_);
    speed = tempSpeed;
};
