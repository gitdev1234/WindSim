/**
    Cube.cpp
    Purpose: Implements Class Cube

    @author Wall.Of.Death
    @version 1.0 20151120
*/

#include "../include/Cube.h"
#include "MoleculeGroup.h"



Cube::Cube() {
    //ctor
}

Cube::~Cube() {
    //dtor
}

/* --- miscellaneous --- */
void Cube::ModifyTemperature(string s){
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

void Cube::ModifyMolecules_count(float delta){
    float temp = getMolecules_Count();
    temp += delta;
    setMolecules_Count(temp);
    CalcPressure();
};

void Cube::CalcPressure() {
    float boltzmann_const = 1.38 * pow(10,-23);
    float molecules_count_ = getMolecules_Count();
    float temperature_     = getTemperature();
    float volume_         = getVolume();
    float pressure_ = (molecules_count_ * boltzmann_const * temperature_) / volume_;
    setPressure(pressure_);
};


float Cube::getAndSetMass() {
    float R_ = 287.05;// individual gas-constant [J / (Kg * K)]
    float pressure_ = getPressure();
    float temperature_ = getTemperature();
    float density = pressure_ / (R_ * temperature_);
    mass = density * getVolume();

    return mass;
};


/* --- simulation --- */

// simulation
/**
 * @param moleculeGroupsPerCube has to be a square number e.g. 100, 1024, 10000
 * @todo later versions : z coordinate is not used
 */
void Cube::initSimulation(int moleculeGroupsPerCube_) {
    MoleculeGroup tempMoleculeGroup;
    setmoleculeGroupsPerCube(moleculeGroupsPerCube_);

    // calculate mass per moleculeGroup
    float tempMass = getAndSetMass();
    float massPerMoleculeGroup = tempMass / moleculeGroupsPerCube_;

    //
    int moleculesInXDirection = round(sqrt(moleculeGroupsPerCube_));
    int moleculesInYDirection = moleculesInXDirection;
    float tempWidth = getWidth();
    float tempLength = getLength();
    float distanceMoleculesInXDirection = (tempWidth / moleculesInXDirection);
    float distanceMoleculesInYDirection = (tempLength / moleculesInYDirection);


    float tempX = 0, tempY = 0;
    vector3 tempPosition;

    // calculating positions of moleculeGroups in cube and adding them to
    // moleculeGroups-list of cube
    tempX = 0-distanceMoleculesInXDirection;
    for (float x = 0; x < moleculesInXDirection; x++) {
        tempX += distanceMoleculesInXDirection;
        tempY = 0-distanceMoleculesInYDirection;
        for (float y = 0; y < moleculesInYDirection; y++) {
            tempY += distanceMoleculesInYDirection;
            tempPosition.x = tempX;
            tempPosition.y = tempY;
            tempPosition.z = 0;
            tempMoleculeGroup.setPositionInCube(tempPosition);
            tempMoleculeGroup.setMass(massPerMoleculeGroup);
            coords tempCoordsInArea = getCoordsInArea();
            tempMoleculeGroup.setCoordsOfCube(tempCoordsInArea);
            tempMoleculeGroup.initSimulation();
            moleculeGroups.push_front(tempMoleculeGroup);
        }
    }
    cout << "created " << moleculeGroups.size() << " molecules." << endl;
}

/**
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
list<MoleculeGroup> Cube::simulateTimeStep(float timeStepInSeconds_) {
    // iterate list of moleculegroups
      // --> setForce
      // --> simulateTimeStep
      // --> check if newPositionInCube is out of cube
      //      --> hand moleculeGroup over to different cube
    list<MoleculeGroup> moleculeGroupsWhichAreLeavingTheCube;
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


        if (newPositionInCube.x < 0) {                             // out in the minus direction of x-axis
            isOutOfCube = true;
            newPositionInCube.x = tempWidth + newPositionInCube.x; // calculate new x coordinate of moleculeGroup
            tempCoordsOfCube.x = tempCoordsOfCube.x - 1;           // calculate x coordinate of new cube

        } else if (newPositionInCube.x > tempWidth) {              // out in the plus direction of x-axis
            isOutOfCube = true;
            newPositionInCube.x = newPositionInCube.x - tempWidth; // calculate new x coordinate of moleculeGroup
            tempCoordsOfCube.x = tempCoordsOfCube.x - 1;           // calculate x coordinate of new cube
        }

        if (newPositionInCube.y < 0) {                             // out in the minus direction of y-axis
            isOutOfCube = true;
            newPositionInCube.y = tempLength + newPositionInCube.y;// calculate new y coordinate of moleculeGroup
            tempCoordsOfCube.y = tempCoordsOfCube.y - 1;           // calculate y coordinate of new cube
        } else if (newPositionInCube.y > tempLength) {             // out in the plus direction of y-axis
            isOutOfCube = true;
            newPositionInCube.y = newPositionInCube.y - tempLength;// calculate new y coordinate of moleculeGroup
            tempCoordsOfCube.y = tempCoordsOfCube.y + 1;             // calculate y coordinate of new cube
        }

        /* @todo later versions z - axis
        if (newPositionInCube.z < 0) {                 // out in the minus direction of z-axis
            isOutOfCube = true;
            newPositionInCube.z = tempHeight + newPositionInCube.z;// calculate new z coordinate of moleculeGroup
            tempCoordsOfCube.z = tempCoordsOfCube.z - 1;           // calculate z coordinate of new cube
        } else if (newPositionInCube.z > tempHeight) { // out in the plus direction of z-axis
            isOutOfCube = true;
            newPositionInCube.z = newPositionInCube.z - tempHeight;
            tempCoordsOfCube.z = tempCoordsOfCube.z + 1;
        }
        */

        iterateMoleculeGroups->setPositionInCube(newPositionInCube);
        if (isOutOfCube) {
            iterateMoleculeGroups->setCoordsOfCube(tempCoordsOfCube);
            moleculeGroupsWhichAreLeavingTheCube.push_front(*iterateMoleculeGroups);
            moleculeGroups.erase(iterateMoleculeGroups);

        }

    }
    return moleculeGroupsWhichAreLeavingTheCube;

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
