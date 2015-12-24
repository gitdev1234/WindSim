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


using namespace std;

/**
 * Class Cube
 *
 * @brief represents a unit within the area class
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
 *                          -> is calculated during simulation within simulateAirExchange()
 *
 * temperature           : in Kelvin
 *                          -> changes during simulateAirExchange()
 *                          -> changes during simulateTemperatureExchange()
 *
 * pressure              : in hPa
 *                          -> changes during simulateAirExchange()
 *
 * mass                  : in kg
 *                          -> changes during simulateAirExchange()
 *
 * force                 : in Newton
 *                          -> changes during simulation -> is set by calculateForces() which is called by Area.simulateMoleculesFlow()
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
        void modifyTemperature(string s_);
        // coords
        bool checkCoordsStillInArea(coords c_);

        /* --- simulation --- */
        // simulation
        void initSimulation();
        void simulateAirExchange(float timeStepInSeconds_);
        list<airDelta> calcLeavingAirDeltas(float timeStepInSeconds_);
        float calcAirDeltaMoleculesCount(float phi_, float timeStepInSeconds_);
        void simulateTemperatureExchange(float timeStepInSeconds_);
        void recalculateAttributes();

        // calculation of forces
        void clearForce();
        void addForce(vector3 force_);
        vector3 getForce();

        // calculation of acceleration and speed (depending on current forces)
        void calcAcceleration();
        void calcSpeed(float timeStepInSeconds_);

        // calculations of attributes
        float calcMoleculesCount();
        float calcPressure();
        float calcDensity();
        float calcMass();
        float calcTemperature();


        /* --- getters and setters --- */
        // setters
        void setMoleculesCount(float val_)            {moleculesCount = val_;            };
        void setTemperature(float val_)               {temperature = val_;               };
        void setPressure(float val_)                  {pressure = val_;                  };
        void setMass(float val_)                      {mass = val_;                      };
        void setCoordsInArea(coords val_)             {coordsInArea = val_;              };
        void setMaxCoordsInArea(coords val_)          {maxCoordsInArea = val_;           };
        void setHeight(float val_)                    {height = val_;                    };
        void setLength(float val_)                    {length = val_;                    };
        void setWidth (float val_)                    {width  = val_;                    };
        void setVolume(float val_)                    {volume = val_;                    };
        void setVolume()                              {
            float tempHeight = getHeight();
            float tempLength = getLength();
            float tempWidth  = getWidth();
            volume = tempHeight * tempLength * tempWidth;
        };
        void setDensity(float val_)                   {density = val_;                   };
        void setAcceleration(vector3 val_)            {acceleration = val_;              };
        void setSpeed(vector3 val_)                   {speed = val_;                     };
        void setInAirDeltas(list<airDelta> val_)      {inAirDeltas = val_;               };
        void addInAirDelta(airDelta val_)             {inAirDeltas.push_front(val_);     };
        void setOutAirDeltas(list<airDelta> val_)     {outAirDeltas = val_;              };

        // getters
        float getMoleculesCount()            {return moleculesCount;            };
        float getTemperature()               {return temperature;               };
        GeoCoords getGeoCoords()             {return geoCoords;                 };
        coords getCoordsInArea()             {return coordsInArea;              };
        coords getMaxCoordsInArea()          {return maxCoordsInArea;           };
        float getHeight()  {return height; };
        float getLength()  {return length; };
        float getWidth ()  {return width ; };
        float getVolume()  {return volume; };
        float getDensity() {return density;};
        float getMass()    {return mass;   };
        vector3 getSpeed() {return speed;  };
        vector3 getAcceleration()        {return acceleration; };
        list<airDelta> getInAirDeltas()  {return inAirDeltas;  };
        list<airDelta> getOutAirDeltas() {return outAirDeltas; };

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
        float temperature;     // in K
        float pressure;        // in hPa
        float mass;            // in kg
        float density;         // in kg/m^3
        vector3 force;         // in N
        vector3 acceleration;  // in m/s^2
        vector3 speed;         // in m/s

        list<airDelta> inAirDeltas;
        list<airDelta> outAirDeltas;

};

#endif // CUBE_H
