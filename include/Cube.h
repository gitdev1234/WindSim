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
        void simulateAirExchange(double timeStepInSeconds_);
        list<airDelta> calcLeavingAirDeltas(double timeStepInSeconds_);
        double calcAirDeltaMoleculesCount(double phi_, double timeStepInSeconds_);
        void clearAirDeltas();
        void simulateTemperatureExchange(double timeStepInSeconds_);
        void recalculateAttributes();

        // calculation of forces
        void clearForce();
        void addForce(vector3 force_);

        // calculation of acceleration and speed (depending on current forces)
        void calcAcceleration();
        void calcSpeed(double timeStepInSeconds_);

        // calculations of attributes
        double calcMoleculesCount();
        double calcPressure();
        double calcDensity();
        double calcMass();
        double calcTemperature();


        /* --- getters and setters --- */
        // setters
        void setMoleculesCount(double val_)            {moleculesCount = val_;            };
        void setTemperature(double val_)               {temperature = val_;               };
        void setPressure(double val_)                  {pressure = val_;                  };
        void setMass(double val_)                      {mass = val_;                      };
        void setCoordsInArea(coords val_)             {coordsInArea = val_;              };
        void setMaxCoordsInArea(coords val_)          {maxCoordsInArea = val_;           };
        void setHeight(double val_)                    {height = val_;                    };
        void setLength(double val_)                    {length = val_;                    };
        void setWidth (double val_)                    {width  = val_;                    };
        void setVolume(double val_)                    {volume = val_;                    };
        void setVolume()                              {
            double tempHeight = getHeight();
            double tempLength = getLength();
            double tempWidth  = getWidth();
            volume = tempHeight * tempLength * tempWidth;
        };
        void setSurfaceRoughness(surfaceRoughnessType val_) {surfaceRoughness = val_;};
        void setDensity(double val_)                   {density = val_;                   };
        void setAcceleration(vector3 val_)            {acceleration = val_;              };
        void setSpeed(vector3 val_)                   {speed = val_;                     };
        void setInAirDeltas(list<airDelta> val_)      {inAirDeltas = val_;               };
        void addInAirDelta(airDelta val_)             {inAirDeltas.push_front(val_);     };
        void setOutAirDeltas(list<airDelta> val_)     {outAirDeltas = val_;              };
        void setTemperatureDelta(double val_)         {temperatureDelta = val_;          };

        // getters
        double getMoleculesCount()           {return moleculesCount;            };
        double getTemperature()              {return temperature;               };
        GeoCoords getGeoCoords()             {return geoCoords;                 };
        coords getCoordsInArea()             {return coordsInArea;              };
        coords getMaxCoordsInArea()          {return maxCoordsInArea;           };
        double getHeight()  {return height; };
        double getLength()  {return length; };
        double getWidth ()  {return width ; };
        double getVolume()  {return volume; };
        double getDensity() {return density;};
        surfaceRoughnessType getSurfaceRoughness() {return surfaceRoughness;};
        double getMass()    {return mass;   };
        double getTemperatureDelta() {return temperatureDelta;};
        vector3 getSpeed() {return speed;  };
        vector3 getAcceleration()        {return acceleration; };
        vector3 getForce()               {return force;        };
        list<airDelta> getInAirDeltas()  {return inAirDeltas;  };
        list<airDelta> getOutAirDeltas() {return outAirDeltas; };

    protected:

    private:
        // constant properties
        double height;  // in m
        double length;  // in m
        double width;   // in m
        double volume;  // in m^3
        surfaceRoughnessType surfaceRoughness;
        GeoCoords geoCoords;
        coords coordsInArea;
        coords maxCoordsInArea;

        // changing properties
        double moleculesCount;
        double temperature;     // in K
        double pressure;        // in hPa
        double mass;            // in kg
        double density;         // in kg/m^3
        vector3 force;         // in N
        vector3 acceleration;  // in m/s^2
        vector3 speed;         // in m/s

        list<airDelta> inAirDeltas;
        list<airDelta> outAirDeltas;

        double temperatureDelta;
};

#endif // CUBE_H
