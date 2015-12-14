/**
    Area.cpp
    Purpose: Implements Class Model_Are

    @author Wall.Of.Death
    @version 1.0 20151120
*/

#include <iostream>"
#include <string>
#include <sstream>
#include <math.h>
#include "Area.h"
#include "Types.h"
#include "float.h"
#include <thread>

using namespace std;

Area::Area() {

};

Area::~Area() {
    //dtor
};

void Area::createStandardArea() {
    GeoCoords UpperLeftCube;
    UpperLeftCube.geoLength = 8.8;       //
    UpperLeftCube.geoWidth = 53.083333;  // Bremen
    UpperLeftCube.geoHeight = 11;        //
    createArea(10,10,1,10,10,UpperLeftCube);
};

void Area::createArea(int CubesCountWidth_, int CubesCountLength_, float heightArea_, float widthArea_, float lengthArea_, GeoCoords UpperLeftCube_) {
    CubesCountWidth = CubesCountWidth_;
    CubesCountLength = CubesCountLength_;
    height = heightArea_;
    width = widthArea_;
    length = lengthArea_;
    geoCoordsUpperLeftCube = UpperLeftCube_;
};

void Area::DestroyArea() {
    // clear storage
    Cubes.clear();
};

void Area::LoadBalancedAreaStructure() {
    // calculation of initial values for cubes
    float height_cube        = height;
    float width_cube         = width / CubesCountWidth;
    float length_cube        = length / CubesCountLength;
    float volume             = height_cube * width_cube * length_cube;
    float temperature   = 288;     // 288 Kelvin = 15 degree celcius = average temp. on earth
    float pressure      = 1013.25; //101325 Pa = 1012.25 hPa = standard pressure on earth / of the atmosphere
    float boltzmann_const = 1.38 * pow(10,-23);
    float molecules_count = (pressure * volume) / (boltzmann_const * temperature);

    // create a 2-dimensional vector of cubes with the size CubesCountWidth x CubesCountLength
    std::vector<std::vector<Cube> > temp(CubesCountWidth, std::vector<Cube>(CubesCountLength));
    Cubes = temp;
    temp.clear();

    // iterate all cubes and initialize them
    // x means the direction of width, y means the direction of length
    for (int x = 0; x < Cubes.size(); x++) {
        for (int y = 0; y < Cubes[x].size(); y++) {
            Cubes[x][y].setMoleculesCount(molecules_count);
            Cubes[x][y].setTemperature(temperature);
            Cubes[x][y].setPressure(pressure);
            coords tempCoords;
            tempCoords.x = x;
            tempCoords.y = y;
            Cubes[x][y].setCoordsInArea(tempCoords);
            coords maxCoords;
            maxCoords.x = Cubes.size();
            maxCoords.y = Cubes[x].size();
            Cubes[x][y].setMaxCoordsInArea(maxCoords);
            Cubes[x][y].setHeight(height_cube);
            Cubes[x][y].setLength(length_cube);
            Cubes[x][y].setWidth (width_cube);
            Cubes[x][y].setVolume();
        }
    }
};


void Area::LoadAreaStructureTemplate(string path){
    // TODO
};

float Area::GetMinMaxValue(string properties, bool max_) {

    float minmaxValue, temp = 0.0;

    if (max_) {
        minmaxValue = 0.0;
    } else {
        minmaxValue = FLT_MAX;
    }

    // iterate all cubes and initialize them
    // x means the direction of width, y means the direction of length
    for (int x = 0; x < Cubes.size(); x++) {
        for (int y = 0; y < Cubes[x].size(); y++) {
            switch (toupper(properties[0])) {
                case 'M' : temp = Cubes[x][y].getMoleculesCount();        break;
                case 'T' : temp = Cubes[x][y].getTemperature();           break;
                case 'P' : temp = Cubes[x][y].calcPressure();              break;
                case 'G' : temp = Cubes[x][y].getMoleculeGroupsPerCube(); break;
            };
            if (max_) {
                if (temp > minmaxValue) {
                    minmaxValue = temp;
                }
            } else {
                if (temp < minmaxValue) {
                    minmaxValue = temp;
                }
            }


        }
    }

    return minmaxValue;

}

/**
only to use for positive numbers!!!
*/
string Area::getANSIRGBScaleColor(float min_, float max_, float value_) {
    int red,green,blue;
    if (min_ == max_) {
        red = 0;
        green = 0;
        blue = 255;
    } else {
        float ratio = (value_ - min_) / (max_ - min_);
        float rgbvalue = 255 * ratio;
        red = round(rgbvalue);
        green = 0;
        blue = round(255-rgbvalue);
    }
    string s, red_s, green_s, blue_s;
    stringstream sStream;
    sStream << red;
    red_s = sStream.str();
    sStream.str("");            // clearing Stream for next calculation
    sStream << green;
    green_s = sStream.str();
    sStream.str("");            // clearing Stream for next calculation
    sStream << blue;
    blue_s = sStream.str();

    s = "\033[1;38;2;" + red_s + ";" + green_s + ";" + blue_s + "m";
    return s;
}

string Area::getANSIEndCode() {
    string s;
    s = "\033[0m";
    return s;
}

void Area::PrintCubes(string properties) {
    bool printCoordinates = false, printMoleculesCount = false, printTemperature = false;
    bool printPressure = false, printMoleculeGroups = false;

    for (int i = 0; i < properties.length(); i++) {
        switch (toupper(properties[i])) {
            case 'C' : printCoordinates = true; break;
            case 'M' : printMoleculesCount = true; break;
            case 'T' : printTemperature = true; break;
            case 'P' : printPressure = true; break;
            case 'G' : printMoleculeGroups = true; break;
        };
    };

    // pretty
    float MoleculesCount_Max = GetMinMaxValue("M",true);
    float MoleculesCount_Min = GetMinMaxValue("M",false);
    float Temperature_Max    = GetMinMaxValue("T",true);
    float Temperature_Min    = GetMinMaxValue("T",false);
    float Pressure_Max       = GetMinMaxValue("P",true);
    float Pressure_Min       = GetMinMaxValue("P",false);
    float MoleculeGroupsCount_Max = GetMinMaxValue("G",true);
    float MoleculeGroupsCount_Min = GetMinMaxValue("G",false);

    // calculate width of table coloumns
    int size;
    vector <int> ColumnWidthVec(Cubes[0].size());
    stringstream outStream, lengthStream;
    for (int i = 0; i < 2; i++) {

        for (int y = 0; y < Cubes.size(); y++) {
            for (int x = 0; x < Cubes[y].size(); x++) {
                if (printCoordinates) {
                    outStream    << "<" << x << ":" << y << ">";
                    lengthStream << "<" << x << ":" << y << ">";
                }
                if (printMoleculesCount) {
                    if (PRINT_PRETTY) {
                        outStream << getANSIRGBScaleColor(MoleculesCount_Max,MoleculesCount_Min,Cubes[x][y].getMoleculesCount());
                    }
                    outStream    << "M:" << Cubes[x][y].getMoleculesCount();
                    lengthStream << "M:" << Cubes[x][y].getMoleculesCount();
                    if (PRINT_PRETTY) {
                        outStream << getANSIEndCode();
                    }
                }
                if (printTemperature) {
                    if (PRINT_PRETTY) {
                        outStream << getANSIRGBScaleColor(Temperature_Max,Temperature_Min,Cubes[x][y].getTemperature());
                    }
                    outStream    << "T:" << Cubes[x][y].getTemperature();
                    lengthStream << "T:" << Cubes[x][y].getTemperature();
                    if (PRINT_PRETTY) {
                        outStream << getANSIEndCode();
                    }
                }
                if (printPressure) {
                    if (PRINT_PRETTY) {
                        outStream << getANSIRGBScaleColor(Pressure_Max,Pressure_Min,Cubes[x][y].calcPressure());
                    }
                    outStream    << "P:" << Cubes[x][y].calcPressure();
                    lengthStream << "P:" << Cubes[x][y].calcPressure();
                    if (PRINT_PRETTY) {
                        outStream << getANSIEndCode();
                    }
                }
                if (printMoleculeGroups) {
                    if (PRINT_PRETTY) {
                        outStream << getANSIRGBScaleColor(MoleculeGroupsCount_Max,MoleculeGroupsCount_Min,Cubes[x][y].getMoleculeGroupsPerCube());
                    }
                    outStream    << "G:" << Cubes[x][y].getMoleculeGroupsPerCube();
                    lengthStream << "G:" << Cubes[x][y].getMoleculeGroupsPerCube();
                    if (PRINT_PRETTY) {
                        outStream << getANSIEndCode();
                    }
                }


                lengthStream.seekg(0, ios::end); // calculate current length
                size = lengthStream.tellg();     //

                if (i == 0) {                   // find highest length per column
                    if (size > ColumnWidthVec[x]) {
                        ColumnWidthVec[x] = size;
                    }
                } else {                        // write data to console
                    cout << "[";
                    cout << outStream.str();
                    while (size < ColumnWidthVec[x]) {  // fill with empty spaces
                        size++;                         // for to get a nice formatted table
                        cout << " ";                    //
                    }
                    cout << "]";
                    if (x < Cubes[y].size() - 1) {
                        cout << " | ";
                    }

                }

                outStream.str("");            // clearing streams for next calculation
                lengthStream.str("");         //

            }
            if (i == 1) {
                cout << endl;
            }
        }
    }

};

void Area::ModifyTemperature(int x, int y, string s) {
    coords c = { .x = x, .y = y };
    Cubes[x][y].modifyTemperature(s);
    AffectSurroundingCubes(x,y);

    cout << "[<" << x << ":" << y << ">";
    cout << "M:" << Cubes[x][y].getMoleculesCount();
    cout << "T:" << Cubes[x][y].getTemperature();
    cout << "P:" << Cubes[x][y].calcPressure() << "]" << endl;
};


/**

|A1|A2|A3|
|B1|B2|B3|
|C1|C2|C3|

B2 is the cube which is changed.
Temperature changes at B2 do lead to pressure changes in B2.
The changes in B2 lead to temperature and pressure changes in
--> A1, A2, A3, B1, B3, C1, C2, C3
The molecules_count are not effected by temperature changes.



*/
void Area::AffectSurroundingCubes(int x, int y){
    // First Step :

    coords A1, A2, A3;
    coords B1, B2, B3;
    coords C1, C2, C3;
    B2 = {.x = x, .y = y};

    if (CheckCoordsStillInArea(B2)) {  // if coordinate of B2 is not valid, do not go on

        float volume_ = Cubes[B2.x][B2.y].getVolume(); // volume is equal for every cube
        float newTemperature_, mass_;
        Cube newCube;

        A1 = {.x = B2.x - 1, .y = B2.y - 1};
        A2 = {.x = B2.x    , .y = B2.y - 1};
        A3 = {.x = B2.x + 1, .y = B2.y - 1};
        B1 = {.x = B2.x - 1, .y = B2.y    };
        B3 = {.x = B2.x + 1, .y = B2.y    };
        C1 = {.x = B2.x - 1, .y = B2.y + 1};
        C2 = {.x = B2.x    , .y = B2.y + 1};
        C3 = {.x = B2.x + 1, .y = B2.y + 1};

        newCube = MixTemperaturesC(Cubes[B2.x][B2.y],Cubes[B2.x][B2.y]); // calculate a mixing of all cubes with valid coordinates

        if (CheckCoordsStillInArea(A1)) {
            newCube = MixTemperaturesC(Cubes[A1.x][A1.y],newCube);       //
        }
        if (CheckCoordsStillInArea(A2)) {
            newCube = MixTemperaturesC(Cubes[A2.x][A2.y],newCube);       //
        }
        if (CheckCoordsStillInArea(A3)) {
            newCube = MixTemperaturesC(Cubes[A3.x][A3.y],newCube);       //
        }
        if (CheckCoordsStillInArea(B1)) {
            newCube = MixTemperaturesC(Cubes[B1.x][B1.y],newCube);       //
        }
        if (CheckCoordsStillInArea(B3)) {
            newCube = MixTemperaturesC(Cubes[B3.x][B3.y],newCube);       //
        }
        if (CheckCoordsStillInArea(C1)) {
            newCube = MixTemperaturesC(Cubes[C1.x][C1.y],newCube);       //
        }
        if (CheckCoordsStillInArea(C2)) {
            newCube = MixTemperaturesC(Cubes[C2.x][C2.y],newCube);       //
        }
        if (CheckCoordsStillInArea(C3)) {
            newCube = MixTemperaturesC(Cubes[C3.x][C3.y],newCube);       //
        }

        newTemperature_ = newCube.getTemperature();         // get calculated mixed temperature

        if (CheckCoordsStillInArea(A1)) {
            Cubes[A1.x][A1.y].setTemperature(newTemperature_); // set new mixed temperature at all cubes with valid coordinates
            Cubes[A1.x][A1.y].calcPressure(); // calculate pressure depending on new temperature
        }
        if (CheckCoordsStillInArea(A2)) {
            Cubes[A2.x][A2.y].setTemperature(newTemperature_); //
            Cubes[A2.x][A2.y].calcPressure(); //
        }
        if (CheckCoordsStillInArea(A3)) {
            Cubes[A3.x][A3.y].setTemperature(newTemperature_); //
            Cubes[A3.x][A3.y].calcPressure(); //
        }
        if (CheckCoordsStillInArea(B1)) {
            Cubes[B1.x][B1.y].setTemperature(newTemperature_); //
            Cubes[B1.x][B1.y].calcPressure(); //
        }

        Cubes[B2.x][B2.y].setTemperature(newTemperature_);     //
        Cubes[B2.x][B2.y].calcPressure();     //

        if (CheckCoordsStillInArea(B3)) {
            Cubes[B3.x][B3.y].setTemperature(newTemperature_); //
            Cubes[B3.x][B3.y].calcPressure(); //
        }
        if (CheckCoordsStillInArea(C1)) {
            Cubes[C1.x][C1.y].setTemperature(newTemperature_); //
            Cubes[C1.x][C1.y].calcPressure(); //
        }
        if (CheckCoordsStillInArea(C2)) {
            Cubes[C2.x][C2.y].setTemperature(newTemperature_); //
            Cubes[C2.x][C2.y].calcPressure(); //
        }
        if (CheckCoordsStillInArea(C3)) {
            Cubes[C3.x][C3.y].setTemperature(newTemperature_); //
            Cubes[C3.x][C3.y].calcPressure(); //
        }

        if (SHOW_IN_DETAIL) {
            PrintCubes("T");
        }

        // Second and next steps
        coords leftUpperCorner;
        coords leftLowerCorner;
        coords rightUpperCorner;
        coords rightLowerCorner;
        stack <coords> left;
        stack <coords> right;
        stack <coords> up;
        stack <coords> down;

        leftUpperCorner = A1;
        leftLowerCorner = C1;
        rightUpperCorner = A3;
        rightLowerCorner = C3;
        if (CheckCoordsStillInArea(B1)) {
            left.push(B1);
        }
        if (CheckCoordsStillInArea(B3)) {
            right.push(B3);
        }
        if (CheckCoordsStillInArea(A2)) {
            up.push(A2);
        }
        if (CheckCoordsStillInArea(C2)) {
            down.push(C2);
        }

        AffectSurroundingCubes(leftUpperCorner, leftLowerCorner, rightUpperCorner, rightLowerCorner,
                               left, right, up, down);

    }

    PrintCubes("T");
};

/**

  0________> x-axis
 0|
  |
  |
  |
  v  y-axis


  first step:
   --> find new coordinates for next recursion depth

  second step of corner cubes:
   --> mix temperatures of leftUpperCorner B2 with A1, A2, B1
       [A1][A2]
       [B1][B2]

   --> mix temperatures of rightUpperCorner B1 with A1, A2, B2
       [A1][A2]
       [B1][B2]

   --> mix temperatures of leftLowerCorner A2 with A1, B1, B2
       [A1][A2]
       [B1][B2]

   --> mix temperatures of rightLowerCorner A1 with A2, B1, B2
       [A1][A2]
       [B1][B2]

  third step of side cubes
   --> mix temperatures of left cubes A2 with A1, B2 with B1, C2 with C1 and so on
       [A1][A2]
       [B1][B2]
       [C1][C2]
       ...
       [N1][N2]

   --> mix temperatures of right cubes A1 with A2, B1 with B2, C1 with C2 and so on
       [A1][A2]
       [B1][B2]
       [C1][C2]
       ...
       [N1][N2]

   --> mix temperatures of upper cubes A1 with B1, A2 with B2, A3 with B3 and so on
       [A1][A2][A3]...[AN]
       [B1][B2][B3]...[BN]

   --> mix temperatures of lower cubes B1 with A1, B2 with A2, B3 with A3 and so on
       [A1][A2][A3]...[AN]
       [B1][B2][B3]...[BN]

  fourth step :
   --> if one of the stacks which contains the new coordinates is not empty go into next recursion-depth with
       AffectSurroundingCubes(new_leftUpperCorner, new_leftLowerCorner, new_rightUpperCorner,
                              new_rightLowerCorner, new_left, new_right, new_up, new_down);

*/
void Area::AffectSurroundingCubes(coords leftUpperCorner, coords leftLowerCorner, coords rightUpperCorner, coords rightLowerCorner,
                                        stack <coords> left, stack <coords> right, stack <coords> up, stack <coords> down) {


    // variables;
    coords tempCoords;
    coords new_leftUpperCorner;
    coords new_leftLowerCorner;
    coords new_rightUpperCorner;
    coords new_rightLowerCorner;
    stack <coords> new_left;
    stack <coords> new_right;
    stack <coords> new_up;
    stack <coords> new_down;

    // first step :

    // new coordinates left upper corner
    new_leftUpperCorner  = {.x = leftUpperCorner.x - 1, .y = leftUpperCorner.y - 1};

    // new coordinates left lower corner
    new_leftLowerCorner = {.x = leftLowerCorner.x - 1, .y = leftLowerCorner.y + 1};

    // new coordinates right upper corner
    new_rightUpperCorner = {.x = rightUpperCorner.x + 1, .y = rightUpperCorner.y - 1};

    // new coordinates right lower corner
    new_rightLowerCorner = {.x = rightLowerCorner.x + 1, .y = rightLowerCorner.y + 1};

    // new coordinates up
    for (int i = new_leftUpperCorner.x + 1; i < new_rightUpperCorner.x; i++) {
        coords tempCoords5;
        tempCoords5.x = i;
        tempCoords5.y = new_leftUpperCorner.y;
        if (CheckCoordsStillInArea(tempCoords5)) {
            new_up.push(tempCoords5);
        }
    }

    // new coordinates down
    for (int i = new_leftLowerCorner.x + 1; i < new_rightLowerCorner.x; i++) {
        coords tempCoords6;
        tempCoords6.x = i;
        tempCoords6.y = new_leftLowerCorner.y;
        if (CheckCoordsStillInArea(tempCoords6)) {
            new_down.push(tempCoords6);
        }
    }

    // new coordinates left
    for (int i = new_leftUpperCorner.y + 1; i < new_leftLowerCorner.y; i++) {
        coords tempCoords7;
        tempCoords7.x = new_leftUpperCorner.x;
        tempCoords7.y = i;
        if (CheckCoordsStillInArea(tempCoords7)) {
            new_left.push(tempCoords7);
        }
    }

    // new coordinates right
    for (int i = new_rightUpperCorner.y + 1; i < new_rightLowerCorner.y; i++) {
        coords tempCoords8;
        tempCoords8.x = new_rightUpperCorner.x;
        tempCoords8.y = i;
        if (CheckCoordsStillInArea(tempCoords8)) {
            new_right.push(tempCoords8);
        }
    }



    // second step :

    coords A1, A2, B1, B2;
    Cube newCube;
    float newTemperature;

    // left upper corner
    B2 = leftUpperCorner;             // calculation
    A1 = {.x = B2.x - 1, .y = B2.y - 1};    // of coords
    A2 = {.x = B2.x    , .y = B2.y - 1};    // depending on
    B1 = {.x = B2.x - 1, .y = B2.y    };    // B2

    if (CheckCoordsStillInArea(B2)) {  // if coordinate of B2 is not valid, all other coordinates are also not valid
        newCube = MixTemperaturesC(Cubes[B2.x][B2.y],Cubes[B2.x][B2.y]);   // calculate a mixing of all cubes with valid coordinates
        if (CheckCoordsStillInArea(A1)) {
            newCube = MixTemperaturesC(Cubes[A1.x][A1.y],newCube);         //
        }
        if (CheckCoordsStillInArea(A2)) {
            newCube = MixTemperaturesC(Cubes[A2.x][A2.y],newCube);         //
        }
        if (CheckCoordsStillInArea(B1)) {
            newCube = MixTemperaturesC(Cubes[B1.x][B1.y],newCube);         //
        }
        newTemperature = newCube.getTemperature();         // get calculated mixed temperature
        Cubes[B2.x][B2.y].setTemperature(newTemperature);      // set new mixed temperature at all cubes with valid coordinates
        Cubes[B2.x][B2.y].calcPressure();                        // calculate pressure depending on new temperature
        if (CheckCoordsStillInArea(A1)) {
            Cubes[A1.x][A1.y].setTemperature(newTemperature);  //
            Cubes[A1.x][A1.y].calcPressure();                    //
        }
        if (CheckCoordsStillInArea(A2)) {
            Cubes[A2.x][A2.y].setTemperature(newTemperature);  //
            Cubes[A2.x][A2.y].calcPressure();                    //
        }
        if (CheckCoordsStillInArea(B1)) {
            Cubes[B1.x][B1.y].setTemperature(newTemperature);  //
            Cubes[B1.x][B1.y].calcPressure();                    //
        }
    }


    if (SHOW_IN_DETAIL) {
        cout << "press any key to go on " << endl;
        string s;
        cin >> s;
        PrintCubes("T");
    }

    // right upper corner
    B1 = rightUpperCorner;
    A1 = {.x = B1.x    , .y = B1.y - 1};
    A2 = {.x = B1.x + 1, .y = B1.y - 1};
    B2 = {.x = B1.x + 1, .y = B1.y    };

    if (CheckCoordsStillInArea(B1)) {  // if coordinate of B1 is not valid, all other coordinates are also not valid
        newCube = MixTemperaturesC(Cubes[B1.x][B1.y],Cubes[B1.x][B1.y]);   // calculate a mixing of all cubes with valid coordinates
        if (CheckCoordsStillInArea(A1)) {
            newCube = MixTemperaturesC(Cubes[A1.x][A1.y],newCube);         //
        }
        if (CheckCoordsStillInArea(A2)) {
            newCube = MixTemperaturesC(Cubes[A2.x][A2.y],newCube);         //
        }
        if (CheckCoordsStillInArea(B2)) {
            newCube = MixTemperaturesC(Cubes[B2.x][B2.y],newCube);         //
        }
        newTemperature = newCube.getTemperature();         // get calculated mixed temperature
        Cubes[B1.x][B1.y].setTemperature(newTemperature);      // set new mixed temperature at all cubes with valid coordinates
        Cubes[B1.x][B1.y].calcPressure();                        // calculate pressure depending on new temperature
        if (CheckCoordsStillInArea(A1)) {
            Cubes[A1.x][A1.y].setTemperature(newTemperature);  //
            Cubes[A1.x][A1.y].calcPressure();                    //
        }
        if (CheckCoordsStillInArea(A2)) {
            Cubes[A2.x][A2.y].setTemperature(newTemperature);  //
            Cubes[A2.x][A2.y].calcPressure();                    //
        }
        if (CheckCoordsStillInArea(B2)) {
            Cubes[B2.x][B2.y].setTemperature(newTemperature);  //
            Cubes[B2.x][B2.y].calcPressure();                    //
        }
    }

    if (SHOW_IN_DETAIL) {
        cout << "press any key to go on " << endl;
        string s;
        cin >> s;
        PrintCubes("T");
    }

    // left lower corner
    A2 = leftLowerCorner;
    A1 = {.x = A2.x - 1, .y = A2.y    };
    B1 = {.x = A2.x - 1, .y = A2.y + 1};
    B2 = {.x = A2.x    , .y = A2.y + 1};

    if (CheckCoordsStillInArea(A2)) {  // if coordinate of A2 is not valid, all other coordinates are also not valid
        newCube = MixTemperaturesC(Cubes[A2.x][A2.y],Cubes[A2.x][A2.y]);   // calculate a mixing of all cubes with valid coordinates
        if (CheckCoordsStillInArea(A1)) {
            newCube = MixTemperaturesC(Cubes[A1.x][A1.y],newCube);         //
        }
        if (CheckCoordsStillInArea(B1)) {
            newCube = MixTemperaturesC(Cubes[B1.x][B1.y],newCube);         //
        }
        if (CheckCoordsStillInArea(B2)) {
            newCube = MixTemperaturesC(Cubes[B2.x][B2.y],newCube);         //
        }
        newTemperature = newCube.getTemperature();         // get calculated mixed temperature
        Cubes[A2.x][A2.y].setTemperature(newTemperature);      // set new mixed temperature at all cubes with valid coordinates
        Cubes[A2.x][A2.y].calcPressure();                        // calculate pressure depending on new temperature
        if (CheckCoordsStillInArea(A1)) {
            Cubes[A1.x][A1.y].setTemperature(newTemperature);  //
            Cubes[A1.x][A1.y].calcPressure();                    //
        }
        if (CheckCoordsStillInArea(B1)) {
            Cubes[B1.x][B1.y].setTemperature(newTemperature);  //
            Cubes[B1.x][B1.y].calcPressure();                    //
        }
        if (CheckCoordsStillInArea(B2)) {
            Cubes[B2.x][B2.y].setTemperature(newTemperature);  //
            Cubes[B2.x][B2.y].calcPressure();                    //
        }
    }

    if (SHOW_IN_DETAIL) {
        cout << "press any key to go on " << endl;
        string s;
        cin >> s;
        PrintCubes("T");
    }

    // right lower corner
    A1 = rightLowerCorner;
    A2 = {.x = A1.x + 1, .y = A1.y    };
    B1 = {.x = A1.x    , .y = A1.y + 1};
    B2 = {.x = A1.x + 1, .y = A1.y + 1};

    if (CheckCoordsStillInArea(A1)) {  // if coordinate of A1 is not valid, all other coordinates are also not valid
        newCube = MixTemperaturesC(Cubes[A1.x][A1.y],Cubes[A1.x][A1.y]);   // calculate a mixing of all cubes with valid coordinates
        if (CheckCoordsStillInArea(A2)) {
            newCube = MixTemperaturesC(Cubes[A2.x][A2.y],newCube);         //
        }
        if (CheckCoordsStillInArea(B1)) {
            newCube = MixTemperaturesC(Cubes[B1.x][B1.y],newCube);         //
        }
        if (CheckCoordsStillInArea(B2)) {
            newCube = MixTemperaturesC(Cubes[B2.x][B2.y],newCube);         //
        }

        newTemperature = newCube.getTemperature();  // getting calculated mixed temperature
        Cubes[A1.x][A1.y].setTemperature(newTemperature);      // set new mixed temperature
        Cubes[A1.x][A1.y].calcPressure();                        // calculate pressure depending on new temperature

        if (CheckCoordsStillInArea(A2)) {
            Cubes[A2.x][A2.y].setTemperature(newTemperature);  //
            Cubes[A2.x][A2.y].calcPressure();                    //
        }
        if (CheckCoordsStillInArea(B1)) {
            Cubes[B1.x][B1.y].setTemperature(newTemperature);  //
            Cubes[B1.x][B1.y].calcPressure();                    //
        }
        if (CheckCoordsStillInArea(B2)) {
            Cubes[B2.x][B2.y].setTemperature(newTemperature);  //
            Cubes[B2.x][B2.y].calcPressure();                    //
        }
    }

    if (SHOW_IN_DETAIL) {
        cout << "press any key to go on " << endl;
        string s;
        cin >> s;
        PrintCubes("T");
    }

    // third step

    // left
    while (!left.empty()) {
        A1 = left.top();
        A2 = {.x = A1.x - 1, .y = A1.y};
        if ( (CheckCoordsStillInArea(A1)) && (CheckCoordsStillInArea(A2)) ) {
            newCube = MixTemperaturesC(Cubes[A1.x][A1.y],Cubes[A2.x][A2.y]);
            newTemperature = newCube.getTemperature();  // getting calculated mixed temperature
            Cubes[A1.x][A1.y].setTemperature(newTemperature);  // set new mixed temperature
            Cubes[A1.x][A1.y].calcPressure();                    // calculate pressure depending on new temperature
            Cubes[A2.x][A2.y].setTemperature(newTemperature);  //
            Cubes[A2.x][A2.y].calcPressure();
        }
        left.pop();                 //
    }

    if (SHOW_IN_DETAIL) {
        cout << "press any key to go on " << endl;
        string s;
        cin >> s;
        PrintCubes("T");
    }

    // right
    while (!right.empty()) {
        A1 = right.top();
        A2 = {.x = A1.x + 1, .y = A1.y};
        if ( (CheckCoordsStillInArea(A1)) && (CheckCoordsStillInArea(A2)) ) {
            newCube = MixTemperaturesC(Cubes[A1.x][A1.y],Cubes[A2.x][A2.y]);
            newTemperature = newCube.getTemperature();  // getting calculated mixed temperature
            Cubes[A1.x][A1.y].setTemperature(newTemperature);  // set new mixed temperature
            Cubes[A1.x][A1.y].calcPressure();                    // calculate pressure depending on new temperature
            Cubes[A2.x][A2.y].setTemperature(newTemperature);  //
            Cubes[A2.x][A2.y].calcPressure();
        }
        right.pop();                 //
    }

    if (SHOW_IN_DETAIL) {
        cout << "press any key to go on " << endl;
        string s;
        cin >> s;
        PrintCubes("T");
    }

    // up
    while (!up.empty()) {
        B1 = up.top();
        A1 = {.x = B1.x, .y = B1.y - 1};
        if ( (CheckCoordsStillInArea(A1)) && (CheckCoordsStillInArea(B1)) ) {
            newCube = MixTemperaturesC(Cubes[A1.x][A1.y],Cubes[B1.x][B1.y]);
            newTemperature = newCube.getTemperature();  // getting calculated mixed temperature
            Cubes[A1.x][A1.y].setTemperature(newTemperature);  // set new mixed temperature
            Cubes[A1.x][A1.y].calcPressure();                    // calculate pressure depending on new temperature
            Cubes[B1.x][B1.y].setTemperature(newTemperature);  //
            Cubes[B1.x][B1.y].calcPressure();
        }
        up.pop();                 //
    }

    if (SHOW_IN_DETAIL) {
        cout << "press any key to go on " << endl;
        string s;
        cin >> s;
        PrintCubes("T");
    }

    // down
    while (!down.empty()) {
        A1 = down.top();
        B1 = {.x = A1.x, .y = A1.y + 1};
        if ( (CheckCoordsStillInArea(A1)) && (CheckCoordsStillInArea(B1)) ) {
            newCube = MixTemperaturesC(Cubes[A1.x][A1.y],Cubes[B1.x][B1.y]);
            newTemperature = newCube.getTemperature();  // getting calculated mixed temperature
            Cubes[A1.x][A1.y].setTemperature(newTemperature);  // set new mixed temperature
            Cubes[A1.x][A1.y].calcPressure();                    // calculate pressure depending on new temperature
            Cubes[B1.x][B1.y].setTemperature(newTemperature);  //
            Cubes[B1.x][B1.y].calcPressure();
        }
        down.pop();
    }

    if (SHOW_IN_DETAIL) {
        cout << "press any key to go on " << endl;
        string s;
        cin >> s;
        PrintCubes("T");
    }


    // fourth step :


    if ( !( (new_left.empty()) && (new_right.empty()) &&
            (new_up.empty())   && (new_down.empty()) ) ) {
        AffectSurroundingCubes(new_leftUpperCorner, new_leftLowerCorner, new_rightUpperCorner,
                               new_rightLowerCorner, new_left, new_right, new_up, new_down);
    }

};

bool  Area::CheckCoordsStillInArea(coords c) {
    bool temp = true;
    if ( (c.x < 0) || (c.x > (GetCubesCountWidth()-1) )) {
        temp = false;
    }
    if ( (c.y < 0) || (c.y > (GetCubesCountLength()-1)) ) {
        temp = false;
    }
    return temp;
};

/**

returns new Temperature
*/
float Area::MixTemperatures(Cube Cube1, Cube Cube2) {
    float Temp1 = Cube1.getTemperature();
    float Mass1 = Cube1.calcMass();
    float Volume1 = Cube1.getVolume();
    float Temp2 = Cube2.getTemperature();
    float Mass2 = Cube2.calcMass();
    float Volume2 = Cube2.getVolume();
    return MixTemperatures(Temp1, Mass1, Volume1, Temp2, Mass2, Volume2);
};

/**

returns new Temperature
*/
float Area::MixTemperatures(float Temp1, float Mass1, float Volume1, float Temp2, float Mass2, float Volume2) {
    float newTemperature_ = ( (Mass1 * Temp1) + (Mass2 * Temp2) ) / (Mass1 + Mass2);
    return newTemperature_;
};

/**

returns new cube object which has
 -> new Temperature = mixed Temperature
 -> new Volume      = Cube1.Volume + Cube2.Volume
 -> new Mass        = Cube1.Mass + Cube2.Mass
*/
Cube Area::MixTemperaturesC(Cube Cube1, Cube Cube2) {
    float Temp1 = Cube1.getTemperature();
    float Mass1 = Cube1.calcMass();
    float Volume1 = Cube1.getVolume();
    float Temp2 = Cube2.getTemperature();
    float Mass2 = Cube2.calcMass();
    float Volume2 = Cube2.getVolume();
    Cube newCube = Cube1;
    newCube.setTemperature(MixTemperatures(Temp1, Mass1, Volume1, Temp2, Mass2, Volume2));
    newCube.setVolume(Volume1 + Volume2);
    newCube.calcMass();
    return newCube;
};

/* --- simulation --- */
// simulation
void Area::initSimulation(int moleculeGroupsPerCube_) {
    float tempMassPerCube = Cubes[0][0].calcMass();
    float tempMassPerMoleculeGroup = tempMassPerCube / moleculeGroupsPerCube_;
    setMassPerMoleculeGroup(tempMassPerMoleculeGroup);

    for (int x = 0; x < Cubes.size(); x++) {
        for (int y = 0; y < Cubes[x].size(); y++) {
            Cubes[x][y].initSimulation(moleculeGroupsPerCube_);
        }
    }
};



void Area::simulate(float timeStepInSeconds_, float simulationSpeedInSeconds_) {
    int startTime = GetTimeMs64();
    float timeDelta = 0;

    // while-loop == whole simulation
    // --> one loop execution is one simulationStep for displaying
    //     and (simulationSpeedInSeconds / timeStepInSeconds) simulation steps for calculation
    while (1) {

        for (int i = 0; i < (simulationSpeedInSeconds_ / timeStepInSeconds_); i++) { //
            simulateTimeStep(timeStepInSeconds_);                                   // calculations
        }                                                                          //

        while (timeDelta <= simulationSpeedInSeconds_ * 1000) { // wait until display simulation time has ended
            // wait
            int newTime = GetTimeMs64();
            timeDelta = newTime-startTime;
        }

        timeDelta = 0;
        startTime = GetTimeMs64();
        // TODO output simulation results to console
        PrintCubes("G");
        cout << "ShowSimulation [][][][][]" << endl;

    }
};

void Area::simulateTimeStep(float timeStepInSeconds_) {
    //TODO
    cout << "simulateTimeStep - dummy :P --> crunching data <--" << endl;
    simulateMoleculesFlow(timeStepInSeconds_);
};

void Area::simulateMoleculesFlow(float timeStepInSeconds_) {
    calculateForces(); // calculates all forces for every cube
    for (int y = 0; y < Cubes.size(); y++) {
        for (int x = 0; x < Cubes[y].size(); x++) {
            Cubes[x][y].simulateTimeStep(timeStepInSeconds_);
        }
    }
    simulateMoleculeGroupsHandover();
    for (int y = 0; y < Cubes.size(); y++) {
        for (int x = 0; x < Cubes[y].size(); x++) {
            Cubes[x][y].recalculateAttributes(changeType::MOLECULE_FLOW);
        }
    }
}

void Area::simulateMoleculeGroupsHandover() {
    for (int y = 0; y < Cubes.size(); y++) {
        for (int x = 0; x < Cubes[y].size(); x++) {
            list<MoleculeGroup> tempMoleculeGroupsWhichAreLeavingTheCube = Cubes[x][y].getMoleculeGroupsWhichAreLeavingTheCube();
            for(auto iterateMoleculeGroups = tempMoleculeGroupsWhichAreLeavingTheCube.begin(); iterateMoleculeGroups != tempMoleculeGroupsWhichAreLeavingTheCube.end(); iterateMoleculeGroups++) {
                MoleculeGroup tempMoleculeGroup = *iterateMoleculeGroups;
                coords newCoords = tempMoleculeGroup.getCoordsOfCube();
                Cubes[newCoords.x][newCoords.y].addMoleculeGroup(*iterateMoleculeGroups);
            }
        }
    }

}

// calculating forces
void Area::calculateForces() {
    coords c;
    for (int y = 0; y < Cubes.size(); y++) {
        for (int x = 0; x < Cubes[y].size(); x++) {
            c.x = x;
            c.y = y;
            calculateForces(c);
        }
    }
};

void Area::calculateForces(coords c) {
    Cubes[c.x][c.y].clearForce();
    Cubes[c.x][c.y].addForce(calculateGradientForce(c));
    Cubes[c.x][c.y].addForce(calculateCoriolisForce(c));
    Cubes[c.x][c.y].addForce(calculateSurfaceFrictionForce(c));
    Cubes[c.x][c.y].addForce(calculateInnerFrictionForce(c));
};

vector3 Area::calculateGradientForce(coords c) {
    float mass ;
    // TODO
    vector3 tempForces;
    tempForces.x = 0.00010;
    tempForces.y = 0.00040;
    tempForces.z = 0.0;
    return tempForces;
};

vector3 Area::calculateCoriolisForce(coords c) {
    // TODO
    vector3 tempForces;
    tempForces.x = 0.0;
    tempForces.y = 0.0;
    tempForces.z = 0.0;
    return tempForces;
};

vector3 Area::calculateSurfaceFrictionForce(coords c) {
    // TODO
    vector3 tempForces;
    tempForces.x = -0.00003;
    tempForces.y = -0.00002;
    tempForces.z = 0.0;
    return tempForces;
};

vector3 Area::calculateInnerFrictionForce(coords) {
    // TODO
    vector3 tempForces;
    tempForces.x = 0.0;
    tempForces.y = 0.0;
    tempForces.z = 0.0;
    return tempForces;
};

/**
 *
 * Returns the amount of milliseconds elapsed since the UNIX epoch. Works on both
 * windows and linux.
 * @source https://stackoverflow.com/questions/1861294/how-to-calculate-execution-time-of-a-code-snippet-in-c
 */

uint64 Area::GetTimeMs64() {
#ifdef _WIN32
 /* Windows */
 FILETIME ft;
 LARGE_INTEGER li;

 /* Get the amount of 100 nano seconds intervals elapsed since January 1, 1601 (UTC) and copy it
  * to a LARGE_INTEGER structure. */
 GetSystemTimeAsFileTime(&ft);
 li.LowPart = ft.dwLowDateTime;
 li.HighPart = ft.dwHighDateTime;

 uint64 ret = li.QuadPart;
 ret -= 116444736000000000LL; /* Convert from file time to UNIX epoch time. */
 ret /= 10000; /* From 100 nano seconds (10^-7) to 1 millisecond (10^-3) intervals */

 return ret;
#else
 /* Linux */
 struct timeval tv;

 gettimeofday(&tv, NULL);

 uint64 ret = tv.tv_usec;
 /* Convert from micro seconds (10^-6) to milliseconds (10^-3) */
 ret /= 1000;

 /* Adds the seconds (10^0) after converting them to milliseconds (10^-3) */
 ret += (tv.tv_sec * 1000);

 return ret;
#endif
}

float Area::getMoleculesPerCubeAfterStart() {
    return Cubes[0][0].getMoleculesCount();
}
