#include "Model_Molecule.h"

Model_Molecule::Model_Molecule()
{
    //ctor
}

Model_Molecule::~Model_Molecule()
{
    //dtor
}

/* --- simulation --- */

// simulation
void Model_Molecule::simulateTimeStep(float timeStepInSeconds_){
    calculateAcceleration();
    calculateSpeed(timeStepInSeconds_);
    // TODO molecule exchange
};

void Model_Molecule::calculateAcceleration() {
    vector3 tempAcceleration;
    vector3 tempForce = getForce();
    float tempMass = getMass();
    tempAcceleration.x = tempForce.x / tempMass;
    tempAcceleration.y = tempForce.y / tempMass;
    tempAcceleration.z = tempForce.z / tempMass;
    acceleration = tempAcceleration;
};

void Model_Molecule::calculateSpeed(float timeStepInSeconds_) {
    vector3 tempSpeed;
    vector3 tempAcceleration = getAcceleration();
    tempSpeed.x += (tempAcceleration.x * timeStepInSeconds_);
    tempSpeed.y += (tempAcceleration.y * timeStepInSeconds_);
    tempSpeed.z += (tempAcceleration.z * timeStepInSeconds_);
    speed = tempSpeed;
};
