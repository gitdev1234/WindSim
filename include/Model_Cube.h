/**
    Model_Cube.h
    Purpose: Defines Class Model_Cube

    @author Wall.Of.Death
    @version 1.0 20151120
*/

#ifndef MODEL_CUBE_H
#define MODEL_CUBE_H

#include <string>
#include <math.h>
#include "Types.h"


using namespace std;

class Model_Cube
{
    public:
        /* TODO STATIC
        static float   height;
        static float   length;
        static float   width;
        static float   volume;
        */

        Model_Cube();
        Model_Cube(int molecules_count, float temperature, float pressure, GeoCoords geoCoords);
        virtual ~Model_Cube();

        // temperature
        void ModifyTemperature(string s);

        // molecules_count
        void ModifyMolecules_count(float delta);

        // pressure
        void CalcPressure();

        // setters
        void setMolecules_Count(float val) {molecules_count = val;};
        void setTemperature(float val)     {temperature = val;};
        void setPressure(float val)        {pressure = val;};
        void setHeight(float val)          {height = val;};
        void setLength(float val)          {length = val;};
        void setWidth (float val)          {width  = val;};
        void setVolume(float val)          {volume = val;};
        void setVolume()                   {volume = height * length * width;};
        // getters
        float getMolecules_Count() {return molecules_count;};
        float getTemperature()     {return temperature;};
        float getPressure()        { CalcPressure(); return pressure;};
        GeoCoords getGeoCoords()   {return geoCoords;};
        float getHeight() {return height;};
        float getLength() {return length;};
        float getWidth () {return width ;};
        float getVolume() {return volume;};
        float getAndSetMass() ;

    protected:

    private:
        float molecules_count;
        float temperature;
        float pressure;
        GeoCoords geoCoords;

        float height;
        float length;
        float width;
        float volume;
        float mass;
};

#endif // MODEL_CUBE_H
