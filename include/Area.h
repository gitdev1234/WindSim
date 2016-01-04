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

/**
 * Class Area
 *
 * @brief represents mXn-Matrix of Cubes
 *
 *
 * see the following image for to understand the meaning of CubesCountWidth, CubesCountLength, height, length, width; x-,y- and z-axis
 *
 *            A-------X-------X-------X-------X-------B    ----> A-->B = x-axis = width
 *           /|      /|      /|      /|      /|      /|                  the number of cubes from A to B is CubesCountWidth = 5
 *          / |     / |     / |     / |     / |     / |
 *         X-------X-------X-------X-------X-------X  |
 *        /|      /|      /|      /|      /|      /|--X
 *       / |     / |     / |     / |     / |     / | /
 *      X-------X-------X-------X-------X-------X  |/
 *     /|      /|      /|      /|      /|      /|--X
 *    / |     / |     / |     / |     / |     / | /
 *   C--|----X--|----X--|----X--|----X--|----D  |/   ^
 *   |  X----|--X----|--X----|--X----|--X----|--X    |
 *   | /     | /     | /     | /     | /     | /     | E-->D = z-axis = height
 *   |/      |/      |/      |/      |/      |/      |         the number of cubes from E to D, in this version of WindSim is always 1
 *   X-------X-------X-------X-------X-------E       |
 *
 *    /
 *   / A-->C = y-axis = length
 *  /          the number of cubes from A to C is CubesCountLength = 3
 * v
 *
 *
 * The matrix is called "Cubes" and represented by a vector of a vector of the class Cube
 * As the matrix is two-dimensional representation, one can understand the vector<vector>> "Cubes" as a top view.
 * Coordinates within this matrix are stored as [Y,X].
 * See the following image to understand how coordinates are calculated.
 *
 *   A-------X-------X-------X-------X-------B   ----> A-->B = x-axis
 *   |       |       |       |       |       |
 *   | [0,0] | [0,1] | [0,2] | [0,3] | [0,4] |
 *   |       |       |       |       |       |
 *   X-------X-------X-------X-------X-------X
 *   |       |       |       |       |       |
 *   | [1,0] | [1,1] | [1,2] | [1,3] | [1,4] |
 *   |       |       |       |       |       |
 *   X-------X-------X-------X-------X-------X
 *   |       |       |       |       |       |
 *   | [2,0] | [2,1] | [2,2] | [2,3] | [2,4] |
 *   |       |       |       |       |       |
 *   C-------X-------X-------X-------X-------D
 *
 *   |
 *   | A-->C = y-axis
 *   |
 *   v
 */

class Area {
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
