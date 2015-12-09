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

class Cube
{
    public:

        Cube();
        Cube(int molecules_count, float temperature, float pressure, GeoCoords geoCoords);
        virtual ~Cube();

        /* --- miscellaneous --- */
        // temperature
        void ModifyTemperature(string s);

        // molecules_count
        void ModifyMolecules_count(float delta);

        // pressure
        void CalcPressure();


        /* --- simulation --- */
        // simulation
        void initSimulation(int moleculeGroupsPerCube);
        list<MoleculeGroup> simulateTimeStep(float timeStepInSeconds_);
        void addMoleculeGroup(MoleculeGroup moleculeGroup_);

        // calculation of forces
        void clearForce();
        void addForce(vector3 force);
        vector3 getForce();


        /* --- getters and setters --- */
        // setters
        void setMolecules_Count(float val)         {molecules_count = val;           };
        void setmoleculeGroupsPerCube(float val)   {moleculeGroupsPerCube = val;     };
        void setTemperature(float val)             {temperature = val;               };
        void setPressure(float val)                {pressure = val;                  };
        void setCoordsInArea(coords val_)          {coordsInArea = val_;             };
        void setMaxCoordsInArea(coords val_)       {maxCoordsInArea = val_;          };
        void setHeight(float val)                  {height = val;                    };
        void setLength(float val)                  {length = val;                    };
        void setWidth (float val)                  {width  = val;                    };
        void setVolume(float val)                  {volume = val;                    };
        void setVolume()                           {volume = height * length * width;};

        // getters
        float getMolecules_Count()         {return molecules_count;         };
        float getmoleculeGroupsPerCube()   {return moleculeGroupsPerCube;   };
        float getTemperature()             {return temperature;             };
        float getPressure()                {CalcPressure(); return pressure;};
        GeoCoords getGeoCoords()           {return geoCoords;               };
        coords getCoordsInArea()           {return coordsInArea;            };
        coords getMaxCoordsInArea()        {return maxCoordsInArea;         };
        float getHeight() {return height;};
        float getLength() {return length;};
        float getWidth () {return width ;};
        float getVolume() {return volume;};
        float getAndSetMass() ;

    protected:

    private:
        // constant properties
        float height;  // in m
        float length;  // in m
        float width;   // in m
        float volume;  // in m
        GeoCoords geoCoords;
        coords coordsInArea;
        coords maxCoordsInArea;


        // changing properties
        float molecules_count;
        float moleculeGroupsPerCube;
        float temperature;     // in K
        float pressure;        // in hPa

        float mass;
        vector3 force; // in N
        list<MoleculeGroup> moleculeGroups;

};

#endif // CUBE_H
