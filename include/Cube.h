/**
 * Cube.h
 * Purpose: defines Class Cube
 *
 * @author Wall.Of.Death
 * @version 1.0
 * @since 20151120
 */

#ifndef CUBE_H
#define CUBE_H

#include <string>
#include <iostream>"
#include <list>
#include <math.h>
#include "Types.h"
#include "MoleculeGroup.h"


using namespace std;

/**
 * Class Cube
 *
 * @brief represents a unit within the area class and contains a number of moleculeGroup Objects
 *
 * A cube object is a specific volume within the area class.
 * As a volume it of course has to have attributes, which tell about the size and location of
 * the cube. These attributes are constant during a simulation - process.
 *
 * The following attributes stay constant during one simulation-process :
 *  - height, length, width : size attributes in meter
 *  - volume                : volume attributes in meter^3
 *  - geoCoords             : location referenced to a global map (in decimal degree)
 *  - coordsInArea          : location within the vector of cubes (of the area class)
 *  - maxCoordsInArea       : size of vector of cubes (of the area class)
 *
 * see the following image for to understand the meaning of height, length, width, x-,y- and z-axis
 *
 *      e-------f
 *     /|      /|
 *    / |     / |
 *   a--|----b  |
 *   |  g----|--h
 *   | /     | /
 *   |/      |/
 *   c-------d
 *
 * x-axis --> width  = c-->d
 * y-axis --> length = d-->h
 * z-axis --> height = d-->b
 *
 *
 * The following attributes change their values during a simulation-process :
 *
 * moleculesCount        : the total amount of all molecules inside the cube
 *                          -> is initially set by Area.loadBalancedAreaStructure()
 *                          -> is calculated during simulation by sum of moleculeGroupsPerCube * moleculesPerMoleculeGroup
 * moleculeGroupsPerCube : the number of all moleculeGroups stored in this cube-object
 * temperature           : in Kelvin
 *                          -> changes during simulateMoleculesFlow()
 *                          -> changes during simulateTemperaturFlow() TODO
 * pressure              : in hPa
 *                          -> changes during simulateMoleculesFlow()
 *                          -> changes during simulateTemperaturFlow() TODO
 * mass                  : in kg
 *                          -> changes c
 *                          -> changes during simulateTemperaturFlow() TODO
 *
 * force                 : in Newton
 *                          -> changes during simulation -> is set by calculateForces() which is called by Area.simulateMoleculesFlow()
 * moleculeGroups        : list of moleculeGroup Objects, that are currently in the cube
 *                          -> changes simulateMoleculesFlow()
 *                           -> if a moleculeGroup leaves the cube it is deleted from the list
 *                           -> if a moleculeGroup from another cube wants get into this cube, it is added to the list
 * moleculeGroupsWhichAreLeavingTheCube : list of moleculeGroups which want to leave the cube after a simulation - time - step
 *                          -> this list is filled by simulateMoleculesFlow()
 *                          -> the area object takes this list during Area.simulateMoleculesFlow() and hands moleculeGroups over
 *                             to their new cubes.
 *
 */

class Cube
{
    public:
        /* --- constructor and destructor --- */
        Cube();
        virtual ~Cube();

        /* --- miscellaneous --- */
        // temperature
        /**
         * Cube::modifyTemperature(string s_)
         *
         * @brief increments or decrements the temperature of the cube by
         * @param timeStepInSeconds_ the length in seconds of the simulated timeStep
         * @return coords : returns the new positionInCube, after the simulated timestep
         *
         * executes simulateMoleculesFlow() for to calculate a new positionInCube of
         * the moleculeGroup
         *
         */
        void modifyTemperature(string s_);

        // pressure
        void calcPressure();


        /* --- simulation --- */
        // simulation
        void initSimulation(int moleculeGroupsPerCube_);
        void simulateTimeStep(float timeStepInSeconds_);
        void simulateMoleculesFlow(float timeStepInSeconds_);
        void addMoleculeGroup(MoleculeGroup moleculeGroup_);

        // calculation of forces
        void clearForce();
        void addForce(vector3 force_);
        vector3 getForce();


        /* --- getters and setters --- */
        // setters
        void setMoleculesCount(float val_)          {moleculesCount = val_;            };
        void setmoleculeGroupsPerCube(float val_)   {moleculeGroupsPerCube = val_;     };
        void setTemperature(float val_)             {temperature = val_;               };
        void setPressure(float val_)                {pressure = val_;                  };
        void setCoordsInArea(coords val_)           {coordsInArea = val_;              };
        void setMaxCoordsInArea(coords val_)        {maxCoordsInArea = val_;           };
        void setHeight(float val_)                  {height = val_;                    };
        void setLength(float val_)                  {length = val_;                    };
        void setWidth (float val_)                  {width  = val_;                    };
        void setVolume(float val_)                  {volume = val_;                    };
        void setVolume()                            {
            float tempHeight = getHeight();
            float tempLength = getLength();
            float tempWidth  = getWidth();
            volume = tempHeight * tempLength * tempWidth;
        };
        void setMoleculeGroupsWhichAreLeavingTheCube(list<MoleculeGroup> val_) {
            moleculeGroupsWhichAreLeavingTheCube = val_;
        };

        // getters
        float getMoleculesCount()          {return moleculesCount;          };
        float getMoleculeGroupsPerCube()   {return moleculeGroupsPerCube;   };
        float getTemperature()             {return temperature;             };
        float getPressure()                {calcPressure(); return pressure;};
        GeoCoords getGeoCoords()           {return geoCoords;               };
        coords getCoordsInArea()           {return coordsInArea;            };
        coords getMaxCoordsInArea()        {return maxCoordsInArea;         };
        float getHeight() {return height;};
        float getLength() {return length;};
        float getWidth () {return width ;};
        float getVolume() {return volume;};
        float getAndSetMass() ;
        list<MoleculeGroup> getMoleculeGroupsWhichAreLeavingTheCube() {return moleculeGroupsWhichAreLeavingTheCube;};


    protected:

    private:
        // constant properties
        float height;  // in m
        float length;  // in m
        float width;   // in m
        float volume;  // in m^3
        GeoCoords geoCoords;
        coords coordsInArea;
        coords maxCoordsInArea;

        // changing properties
        float moleculesCount;
        float moleculeGroupsPerCube;
        float temperature;     // in K
        float pressure;        // in hPa

        float mass;    // in kg
        vector3 force; // in N
        list<MoleculeGroup> moleculeGroups;
        list<MoleculeGroup> moleculeGroupsWhichAreLeavingTheCube;

};

#endif // CUBE_H
