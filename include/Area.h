/**
    Area.h
    Purpose: Defines Class Area

    @author Wall.Of.Death
    @version 1.0
    @since 2015-11-20
*/

#ifndef AREA_H
#define AREA_H

#include <vector>
#include <stack>
#include <string>
#include "Cube.h"
#include "sstream"

using namespace std;

class Area
{
    public:
        Area();
        //Area(int CubesCountWidth, int CubesCountLength, float heightCube, float widthCube, float lengthCube);
        virtual ~Area();

        // creating Areas
        void createStandardArea();
        void createArea(int CubesCountWidth_, int CubesCountLength_, float heightArea_, float widthArea_, float lengthArea_, GeoCoords UpperLeftCube_);
        void DestroyArea();

        // loading area pressure and temperature structure
        void LoadBalancedAreaStructure();
        void LoadAreaStructureTemplate(string path);

        // ui-implementation
        void PrintCubes(string properties);


        // modifying temperature values
        void ModifyTemperature(int x, int y, string s);
        void AffectSurroundingCubes(int x, int y);
        void AffectSurroundingCubes(coords leftUpperCorner, coords leftLowerCorner, coords rightUpperCorner, coords rightLowerCorner,
                                    stack <coords> left, stack <coords> right, stack <coords> up, stack <coords> down);
        float MixTemperatures(Cube Cube1, Cube Cube2);
        float MixTemperatures(float Temp1, float Mass1, float Volume1, float Temp2, float Mass2, float Volume2);
        Cube MixTemperaturesC(Cube Cube1, Cube Cube2);
        bool  CheckCoordsStillInArea(coords c);

        /* --- simulation --- */
        // simulation
        void initSimulation();
        void simulate(float timeStepInSeconds_, float simulationSpeedInSeconds_);
        void simulateTimeStep(float timeStepInSeconds_);
        void simulateAirExchange(float timeStepInSeconds_);
        void simulateTemperatureExchange(float timeStepInSeconds_);

        // calculating forces
        void calculateForces(coords c);
        vector3 calculateGradientForce(coords c);
        vector3 calculateGradientForce(coords fromCube_, coords toCube_);
        vector3 calculateCoriolisForce(coords c);
        vector3 calculateFrictionForce(coords c);

        // miscellaneous
        string getANSIRGBScaleColor(float min_, float max_, float value_);
        string getANSIEndCode();
        float GetMinMaxValue(string properties, bool max_) ;
        uint64 GetTimeMs64();
        vector3 sumVector3(vector3 a, vector3 b);

        // setters
        void SetCubesCountWidth(int val)  { CubesCountWidth = val; }
        void SetCubesCountLength(int val) { CubesCountLength = val; }
        void Setheight(float val)         { height = val; }
        void Setlength(float val)         { length = val; }
        void Setwidth(float val)          { width = val; }

        // getters
        int GetCubesCountWidth()  { return CubesCountWidth; }
        int GetCubesCountLength() { return CubesCountLength; }
        float Getheight()         { return height; }
        float Getlength()         { return length; }
        float Getwidth()          { return width; }

    protected:

    private:
        int CubesCountWidth;
        int CubesCountLength;
        float height;
        float length;
        float width;
        GeoCoords geoCoordsUpperLeftCube;
        std::vector<std::vector<Cube> > Cubes; // 2-dimensional array of Cube objects

};

#endif // AREA_H
