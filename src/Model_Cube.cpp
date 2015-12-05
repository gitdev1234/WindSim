/**
    Model_Cube.cpp
    Purpose: Implements Class Model_Cube

    @author Wall.Of.Death
    @version 1.0 20151120
*/

#include "../include/Model_Cube.h"


Model_Cube::Model_Cube() {
    //ctor
}

Model_Cube::~Model_Cube() {
    //dtor
}

/* --- miscellaneous --- */
void Model_Cube::ModifyTemperature(string s){
    float temp = getTemperature();
    if (s[0] == '+') {
        temp += ModifyTemperatureDelta;
    }
    if (s[0] == '-') {
        temp -= ModifyTemperatureDelta;
    }
    setTemperature(temp);

    CalcPressure();
};

void Model_Cube::ModifyMolecules_count(float delta){
    float temp = getMolecules_Count();
    temp += delta;
    setMolecules_Count(temp);
    CalcPressure();
};

void Model_Cube::CalcPressure() {
    float boltzmann_const = 1.38 * pow(10,-23);
    float molecules_count_ = getMolecules_Count();
    float temperature_     = getTemperature();
    float volume_         = getVolume();
    float pressure_ = (molecules_count_ * boltzmann_const * temperature_) / volume_;
    setPressure(pressure_);
};


float Model_Cube::getAndSetMass() {
    float R_ = 287.05;// individual gas-constant [J / (Kg * K)]
    float pressure_ = getPressure();
    float temperature_ = getTemperature();
    float thickness = pressure_ / (R_ * temperature_);
    mass = thickness * getVolume();

    return mass;
};


/* --- simulation --- */

// simulation
void Model_Cube::simulateTimeStep(float timeStepInSeconds) {
};

// calculation of forces

void Model_Cube::clearForce() {
    force.x = 0;
    force.y = 0;
    force.z = 0;
};

void Model_Cube::AddForce(vector3 force_) {
    force.x += force_.x;
    force.y += force_.y;
    force.z += force_.z;
};

vector3 Model_Cube::getForce() {
    return force;
};
