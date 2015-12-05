/**
    Model_Area.h
    Purpose: Defines Class Model_Area

    @author Wall.Of.Death
    @version 1.0 20151120
*/

#ifndef MODEL_AREA_H
#define MODEL_AREA_H

#include <vector>
#include <stack>
#include <string>
#include "Model_Cube.h"
#include "sstream"

using namespace std;

class Model_Area
{
    public:
        Model_Area();
        //Model_Area(int CubesCountWidth, int CubesCountLength, float heightCube, float widthCube, float lengthCube);
        virtual ~Model_Area();

        // creating Areas
        void createStandardArea();
        void createArea(int CubesCountWidth_, int CubesCountLength_, float heightArea_, float widthArea_, float lengthArea_, GeoCoords UpperLeftCube_);
        void DestroyArea();

        // loading area pressure and temperature structure
        void LoadBalancedAreaStructure();
        void LoadAreaStructureTemplate(string path);

        // ui-implementation
        void PrintCubes(string properties);


        // modifying values
        void ModifyTemperature(int x, int y, string s);
        void AffectSurroundingCubes(int x, int y);
        void AffectSurroundingCubes(coords leftUpperCorner, coords leftLowerCorner, coords rightUpperCorner, coords rightLowerCorner,
                                    stack <coords> left, stack <coords> right, stack <coords> up, stack <coords> down);
        float MixTemperatures(Model_Cube Cube1, Model_Cube Cube2);
        float MixTemperatures(float Temp1, float Mass1, float Volume1, float Temp2, float Mass2, float Volume2);
        Model_Cube MixTemperaturesC(Model_Cube Cube1, Model_Cube Cube2);
        bool  CheckCoordsStillInArea(coords c);
        float GetMinMaxValue(string properties, bool max_) ;

        //miscellanous
        string getANSIRGBScaleColor(float min_, float max_, float value_);
        string getANSIEndCode();

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
        std::vector<std::vector<Model_Cube> > Cubes; // 2-dimensional array of model_cube objects
        std::stack<coords> temperatureModifications;
};

#endif // MODEL_AREA_H
