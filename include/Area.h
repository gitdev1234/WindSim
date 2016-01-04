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
#include <iomanip>  // for cout << setprecision
#include <limits>   //    -- || --
#include <float.h>

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
        //Area(int CubesCountWidth, int CubesCountLength, double heightCube, double widthCube, double lengthCube);
        virtual ~Area();

        // creating Areas
        void createStandardArea();
        void createArea(int CubesCountWidth_, int CubesCountLength_, double heightArea_, double widthArea_, double lengthArea_, GeoCoords UpperLeftCube_);
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
        double MixTemperatures(Cube Cube1, Cube Cube2);
        double MixTemperatures(double Temp1, double Mass1, double Volume1, double Temp2, double Mass2, double Volume2);
        Cube MixTemperaturesC(Cube Cube1, Cube Cube2);
        bool  CheckCoordsStillInArea(coords c);

        /* --- simulation --- */
        // simulation
        void initSimulation();
        void simulate(double timeStepInSeconds_, double simulationSpeedInSeconds_);
        void simulateTimeStep(double timeStepInSeconds_);
        void simulateAirExchange(double timeStepInSeconds_);
        void simulateTemperatureExchange(double timeStepInSeconds_);

        // calculating forces
        void calculateForces(coords c);
        vector3 calculateGradientForce(coords c);
        vector3 calculateGradientForce(coords fromCube_, coords toCube_);
        vector3 calculateCoriolisForce(coords c);
        vector3 calculateFrictionForce(coords c);

        // miscellaneous
        string getANSIRGBScaleColor(double min_, double max_, double value_);
        string getANSIEndCode();
        double GetMinMaxValue(string properties, bool max_) ;
        uint64 GetTimeMs64();
        vector3 sumVector3(vector3 a, vector3 b);

        // setters
        void SetCubesCountWidth(int val)  { CubesCountWidth = val; }
        void SetCubesCountLength(int val) { CubesCountLength = val; }
        void Setheight(double val)         { height = val; }
        void Setlength(double val)         { length = val; }
        void Setwidth(double val)          { width = val; }

        // getters
        int GetCubesCountWidth()  { return CubesCountWidth; }
        int GetCubesCountLength() { return CubesCountLength; }
        double Getheight()         { return height; }
        double Getlength()         { return length; }
        double Getwidth()          { return width; }

    protected:

    private:
        int CubesCountWidth;
        int CubesCountLength;
        double height;
        double length;
        double width;
        GeoCoords geoCoordsUpperLeftCube;
        std::vector<std::vector<Cube> > Cubes; // 2-dimensional array of Cube objects

};

#endif // AREA_H
