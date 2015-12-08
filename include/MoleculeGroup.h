/**
  * MoleculeGroup.h
  *
  * Purpose: Defines Class MoleculeGroup
  *
  *
  * @author Wall.Of.Death
  * @version 1.0
  * @since 20151120
  */

#ifndef MOLECULEGROUP_H
#define MOLECULEGROUP_H

#include "Types.h"

/**
 * Class MoleculeGroup
 *
 * @brief represents a group of air molecules
 *
 * A MoleculeGroup - object represents a group of air molecules.
 * The Model_MoleculesGroup - objects are stored in Model_Cube - objects.
 * This means every cube contains a number of MoleculeGroups.
 * The cubes are stored in the Area - object.
 * The Area - object calculates in every simulationTimeStep the current forces for every cube.
 * The cubes hand these forces over to their MoleculeGroup - objects.
 * The MoleculeGroup - objects then calculate for every simulationTimeStep, the new attributes.
 * Depending on the positionInCube attribute of the MoleculeGroup - objects the Area - object
 * can react on a MoleculeGroup that is leaving a cube. In such a case the MoleculeGroup is handed over from
 * the old cube into the new.
 *
 *
 * The following attributes stay constant for during one simulation-process :
 *  - moleculesCount : the number of molecules the group is representing
 *  - mass           : the mass (calculated by mass of one molecule * moleculesCount)
 *
 * The following attributes change their values during a simulation-process :
 *  - temperature    : changes during simulateTemperaturFlow()
 *  - force          : force is the sum of all current forces, which are calculated and set by Model_Cubes - class
 *  - acceleration   : changes during simulateMoleculesFlow() by calculating acceleration = Force/mass
 *  - speed          : changes during simulateMoleculesFlow() by calculating speed += (acceleration*simulation-time)
 *  - positionInCube : changes during simulateMoleculesFlow() by calculating positionInCube += (speed*simulation-time)
 * every molecule-group has a mass, a temperature
 */
class MoleculeGroup
{
    public:
        MoleculeGroup();
        virtual ~MoleculeGroup();

        /* --- simulation --- */
        // simulation
        void startSimulation();
        coords simulateTimeStep(float timeStepInSeconds_);
        coords simulateMoleculesFlow(float timeStepInSeconds_);
        void calculateAcceleration();
        void calculateSpeed(float timeStepInSeconds);

        /* --- getters and setters --- */
        // setters
        void setMoleculesCount(float val)   {moleculesCount = val;};
        void setMass(float val)             {mass = val;          };
        void setTemperature(float val_)     {temperature = val_;  };
        void setForce(vector3 val)          {force = val;         };
        void setPositionInCube(vector3 val) {positionInCube = val;};

        // getters
        float   getMoleculesCount()  {return moleculesCount;};
        float   getMass()            {return mass;          };
        float   getTemperature()     {return temperature;   };
        vector3 getForce()           {return force;         };
        vector3 getAcceleration()    {return acceleration;  };
        vector3 getSpeed()           {return speed;         };
        vector3 getPositionInCube()  {return positionInCube;};


    protected:

    private:
        float moleculesCount;
        float mass;
        float temperature;
        vector3 force;
        vector3 acceleration;
        vector3 speed;
        vector3 positionInCube;

};

#endif // MOLECULEGROUP_H
