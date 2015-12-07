#ifndef MODEL_MOLECULEGROUP_H
#define MODEL_MOLECULEGROUP_H

#include "Types.h"

class Model_MoleculeGroup
{
    public:
        Model_MoleculeGroup();
        virtual ~Model_MoleculeGroup();

        /* --- simulation --- */
        // simulation
        void startSimulation();
        coords simulateTimeStep(float timeStepInSeconds);
        void calculateAcceleration();
        void calculateSpeed(float timeStepInSeconds);

        /* --- getters and setters --- */
        // setters
        void setMoleculesCount(float val)   {moleculesCount = val;};
        void setMass(float val)             {mass = val;          };
        void setForce(vector3 val)          {force = val;         };
        void setPositionInCube(vector3 val) {positionInCube = val;};

        // getters
        float   getMoleculesCount()  {return moleculesCount;};
        float   getMass()            {return mass;          };
        vector3 getForce()           {return force;         };
        vector3 getAcceleration()    {return acceleration;  };
        vector3 getSpeed()           {return speed;         };
        vector3 getPositionInCube()  {return positionInCube;};

    protected:

    private:
        float moleculesCount;
        float mass;
        vector3 force;
        vector3 acceleration;
        vector3 speed;
        vector3 positionInCube;

};

#endif // MODEL_MOLECULEGROUP_H
