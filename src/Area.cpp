/**
    Area.cpp
    Purpose: Implements Class Model_Are

    @author Wall.Of.Death
    @version 1.0 20151120
*/

#include <iostream>"
#include <string>
#include <sstream>
#include <fstream>
#include <math.h>
#include "Area.h"
#include "Types.h"
#include <thread>
#include "NumericIntegrator.h"
#include "saveSqlite.hpp"

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

void Area::createArea(int CubesCountWidth_, int CubesCountLength_, double heightArea_, double widthArea_, double lengthArea_, GeoCoords UpperLeftCube_) {
    CubesCountWidth = CubesCountWidth_;
    CubesCountLength = CubesCountLength_;
    height = heightArea_;
    width = widthArea_;
    length = lengthArea_;
    geoCoordsUpperLeftCube = UpperLeftCube_;
    openSQLite();
};

void Area::DestroyArea() {
    closeDataBase(db);
    // clear storage
    Cubes.clear();
};

void Area::LoadBalancedAreaStructure() {
    // calculation of initial values for cubes
    double height_cube        = height;
    double width_cube         = width / CubesCountWidth;
    double length_cube        = length / CubesCountLength;
    double volume_cube             = height_cube * width_cube * length_cube;
    double temperature   = 288;     // 288 Kelvin = 15 degree celcius = average temp. on earth
    double pressure      = 1013.25; //101325 Pa = 1012.25 hPa = standard pressure on earth / of the atmosphere
    double boltzmann_const = 1.38 * pow(10,-23);
    double molecules_count = (pressure * volume_cube) / (boltzmann_const * temperature);
    surfaceRoughnessType surfaceRoughness = surfaceRoughnessType::MEADOW_WITH_MANY_HEDGES;

    // create a 2-dimensional vector of cubes with the size CubesCountLength x CubesCountWidth
    if (CubesCountLength <= 0) {
        CubesCountLength = 1;
    }
    if (CubesCountWidth <= 0) {
        CubesCountWidth = 1;
    }
    vector<vector<Cube> > temp(CubesCountLength, std::vector<Cube>(CubesCountWidth));
    Cubes = temp;
    temp.clear();

    // iterate all cubes and initialize them
    // y means the direction of length, x means the direction of width
    for (int y = 0; y < Cubes.size(); y++) {
        for (int x = 0; x < Cubes[y].size(); x++) {
            Cubes[y][x].setMoleculesCount(molecules_count);
            Cubes[y][x].setTemperature(temperature);
            Cubes[y][x].setPressure(pressure);
            Cubes[y][x].setSurfaceRoughness(surfaceRoughness);
            coords tempCoords;
            tempCoords.x = x;
            tempCoords.y = y;
            Cubes[y][x].setCoordsInArea(tempCoords);
            coords maxCoords;
            maxCoords.y = Cubes.size()-1;
            maxCoords.x = Cubes[y].size()-1;
            Cubes[y][x].setMaxCoordsInArea(maxCoords);
            Cubes[y][x].setHeight(height_cube);
            Cubes[y][x].setLength(length_cube);
            Cubes[y][x].setWidth (width_cube);
            Cubes[y][x].setVolume();
            setNeighbourCubes(tempCoords);
            Cubes[y][x].setGeoCoords(calcGeoCoords(tempCoords));
        }
    }
};

void Area::saveAreaStructureTemplate(string path_) {
    ofstream file(path_.c_str());
    if (file) {
        file << fixed << setprecision(20);
        file << GetCubesCountWidth() << " ";
        file << GetCubesCountLength() << " ";
        file << Getheight() << " ";
        file << Getwidth() << " ";
        file << Getlength() << " ";
        GeoCoords tempGeoCoords = getGeoCoordsUpperLeftCube();
        file << tempGeoCoords.geoHeight << " ";
        file << tempGeoCoords.geoLength << " ";
        file << tempGeoCoords.geoWidth << " ";
        file << endl;
        for (int y = 0; y < Cubes.size(); y++) {
            for (int x = 0; x < Cubes[y].size(); x++) {
                    file << y << " " << x << " ";
                    file << Cubes[y][x].getMoleculesCount() << " ";
                    file << Cubes[y][x].getTemperature() << " ";
                    file << Cubes[y][x].getPressure() << " ";
                    file << endl;
            }
        }
    } else {
        cout << "Error while saving Area Structure Template : Could not create file at path : " << path_ << endl;
    }

};

void Area::setNeighbourCubes(coords c_) {
    coords leftUpperCorner  = {.x = c_.x - 1, .y = c_.y - 1};
    coords up               = {.x = c_.x    , .y = c_.y - 1};
    coords rightUpperCorner = {.x = c_.x + 1, .y = c_.y - 1};
    coords right            = {.x = c_.x + 1, .y = c_.y    };
    coords rightLowerCorner = {.x = c_.x + 1, .y = c_.y + 1};
    coords down             = {.x = c_.x    , .y = c_.y + 1};
    coords leftLowerCorner  = {.x = c_.x - 1, .y = c_.y + 1};
    coords left             = {.x = c_.x - 1, .y = c_.y    };
    if (CheckCoordsStillInArea(leftUpperCorner)) {
        Cubes[c_.y][c_.x].setLeftUpperNeighbour(&Cubes[leftUpperCorner.y][leftUpperCorner.x]);
    }
    if (CheckCoordsStillInArea(up)) {
        Cubes[c_.y][c_.x].setUpNeighbour(&Cubes[up.y][up.x]);
    }
    if (CheckCoordsStillInArea(rightUpperCorner)) {
        Cubes[c_.y][c_.x].setRightUpperNeighbour(&Cubes[rightUpperCorner.y][rightUpperCorner.x]);
    }
    if (CheckCoordsStillInArea(right)) {
        Cubes[c_.y][c_.x].setRightNeighbour(&Cubes[right.y][right.x]);
    }
    if (CheckCoordsStillInArea(rightLowerCorner)) {
        Cubes[c_.y][c_.x].setRightLowerNeighbour(&Cubes[rightLowerCorner.y][rightLowerCorner.x]);
    }
    if (CheckCoordsStillInArea(down)) {
        Cubes[c_.y][c_.x].setDownNeighbour(&Cubes[down.y][down.x]);
    }
    if (CheckCoordsStillInArea(leftLowerCorner)) {
        Cubes[c_.y][c_.x].setLeftLowerNeighbour(&Cubes[leftLowerCorner.y][leftLowerCorner.x]);
    }
    if (CheckCoordsStillInArea(left)) {
        Cubes[c_.y][c_.x].setLeftNeighbour(&Cubes[left.y][left.x]);
    }

};

void Area::LoadAreaStructureTemplate(string path_){
    ifstream file(path_.c_str());
    if (file) {
        double tempCubesCountWidth, tempCubesCountLength, tempHeight, tempWidth ,tempLength;
        GeoCoords tempGeoCoords;
        file >> tempCubesCountWidth;
        file >> tempCubesCountLength;
        file >> tempHeight;
        file >> tempWidth;
        file >> tempLength;
        file >> tempGeoCoords.geoHeight;
        file >> tempGeoCoords.geoLength;
        file >> tempGeoCoords.geoWidth;
        createArea(tempCubesCountWidth,tempCubesCountLength,tempHeight,tempWidth,tempLength,tempGeoCoords);
        vector<vector<Cube> > temp(tempCubesCountLength, std::vector<Cube>(tempCubesCountWidth));
        Cubes = temp;
        temp.clear();

        double height_cube        = height;
        double width_cube         = width / CubesCountWidth;
        double length_cube        = length / CubesCountLength;
        double volume_cube        = height_cube * width_cube * length_cube;
        surfaceRoughnessType surfaceRoughness = surfaceRoughnessType::MEADOW_WITH_MANY_HEDGES;

        int y, x;
        double tempMoleculesCount, tempTemperature, tempPressure;
        while (file >> y) {
            file >> x;
            file >> tempMoleculesCount;
            file >> tempTemperature;
            file >> tempPressure;
            Cubes[y][x].setMoleculesCount(tempMoleculesCount);
            Cubes[y][x].setTemperature(tempTemperature);
            Cubes[y][x].setPressure(tempPressure);
            Cubes[y][x].setSurfaceRoughness(surfaceRoughness);
            coords tempCoords;
            tempCoords.x = x;
            tempCoords.y = y;
            Cubes[y][x].setCoordsInArea(tempCoords);
            coords maxCoords;
            maxCoords.y = Cubes.size()-1;
            maxCoords.x = Cubes[y].size()-1;
            Cubes[y][x].setMaxCoordsInArea(maxCoords);
            Cubes[y][x].setHeight(height_cube);
            Cubes[y][x].setLength(length_cube);
            Cubes[y][x].setWidth (width_cube);
            Cubes[y][x].setVolume();
            setNeighbourCubes(tempCoords);
            Cubes[y][x].setGeoCoords(calcGeoCoords(tempCoords));
        }
    } else {
        cout << "Error while loading Area Structure Template : Could not load file at path : " << path_ << endl;
    }


};

double Area::GetMinMaxValue(string properties, bool max_) {

    double minmaxValue, temp = 0.0;

    if (max_) {
        minmaxValue = 0.0;
    } else {
        minmaxValue = DBL_MAX;
    }

    // iterate all cubes and initialize them
    // y means the direction of length, x means the direction of width
    for (int y = 0; y < Cubes.size(); y++) {
        for (int x = 0; x < Cubes[y].size(); x++) {
            switch (toupper(properties[0])) {
                case 'M' : temp = Cubes[y][x].getMoleculesCount();        break;
                case 'T' : temp = Cubes[y][x].getTemperature();           break;
                case 'P' : temp = Cubes[y][x].getPressure();              break;
                case 'S' : temp = Cubes[y][x].getSpeed().x;                 break;
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
string Area::getANSIRGBScaleColor(double min_, double max_, double value_) {
    int red,green,blue;
    if (min_ == max_) {
        red = 255;
        green = 0;
        blue = 0;
    } else {
        double ratio = (value_ - min_) / (max_ - min_);
        double rgbvalue = 255 * ratio;
        red = round(255-rgbvalue);
        green = 0;
        blue = round(rgbvalue);
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
    double MoleculesCount_Max = GetMinMaxValue("M",true);
    double MoleculesCount_Min = GetMinMaxValue("M",false);
    double Temperature_Max    = GetMinMaxValue("T",true);
    double Temperature_Min    = GetMinMaxValue("T",false);
    double Pressure_Max       = GetMinMaxValue("P",true);
    double Pressure_Min       = GetMinMaxValue("P",false);
    double MoleculeGroupsCount_Max = GetMinMaxValue("G",true);
    double MoleculeGroupsCount_Min = GetMinMaxValue("G",false);



    // calculate width of table coloumns
    int size;
    vector <int> ColumnWidthVec(Cubes[0].size());
    stringstream outStream, lengthStream;

    // set how many decimal places are printed
    cout << setprecision(PRINT_PRECISION) << fixed;
    outStream << setprecision(PRINT_PRECISION) << fixed;

    for (int i = 0; i < 2; i++) {

        for (int y = 0; y < Cubes.size(); y++) {
            for (int x = 0; x < Cubes[y].size(); x++) {
                if (printCoordinates) {
                    outStream    << "<" << y << ":" << x << ">";
                    lengthStream << "<" << y << ":" << x << ">";
                }
                if (printMoleculesCount) {
                    if (PRINT_PRETTY) {
                        outStream << getANSIRGBScaleColor(MoleculesCount_Max,MoleculesCount_Min,Cubes[y][x].getMoleculesCount());
                    }
                    outStream    << "M:" << Cubes[y][x].getMoleculesCount();
                    lengthStream << "M:" << Cubes[y][x].getMoleculesCount();
                    if (PRINT_PRETTY) {
                        outStream << getANSIEndCode();
                    }
                }
                if (printTemperature) {
                    if (PRINT_PRETTY) {
                        outStream << getANSIRGBScaleColor(Temperature_Max,Temperature_Min,Cubes[y][x].getTemperature());
                    }
                    outStream    << "T:" << Cubes[y][x].getTemperature();
                    lengthStream << "T:" << Cubes[y][x].getTemperature();
                    if (PRINT_PRETTY) {
                        outStream << getANSIEndCode();
                    }
                }
                if (printPressure) {
                    if (PRINT_PRETTY) {
                        outStream << getANSIRGBScaleColor(Pressure_Max,Pressure_Min,Cubes[y][x].calcPressure());
                    }
                    outStream    << "P:" << Cubes[y][x].calcPressure();
                    lengthStream << "P:" << Cubes[y][x].calcPressure();
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
    Cubes[y][x].modifyTemperature(s);
    //AffectSurroundingCubes(x,y); //TODO

    //cout << "[<" << y << ":" << x << ">";
    //cout << "M:" << Cubes[y][x].getMoleculesCount();
    //cout << "T:" << Cubes[y][x].getTemperature();
    //cout << "P:" << Cubes[y][x].calcPressure() << "]" << endl;
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

        double volume_ = Cubes[B2.y][B2.x].getVolume(); // volume is equal for every cube
        double newTemperature_, mass_;
        Cube newCube;

        A1 = {.x = B2.x - 1, .y = B2.y - 1};
        A2 = {.x = B2.x    , .y = B2.y - 1};
        A3 = {.x = B2.x + 1, .y = B2.y - 1};
        B1 = {.x = B2.x - 1, .y = B2.y    };
        B3 = {.x = B2.x + 1, .y = B2.y    };
        C1 = {.x = B2.x - 1, .y = B2.y + 1};
        C2 = {.x = B2.x    , .y = B2.y + 1};
        C3 = {.x = B2.x + 1, .y = B2.y + 1};

        newCube = MixTemperaturesC(Cubes[B2.y][B2.x],Cubes[B2.y][B2.x]); // calculate a mixing of all cubes with valid coordinates

        if (CheckCoordsStillInArea(A1)) {
            newCube = MixTemperaturesC(Cubes[A1.y][A1.x],newCube);       //
        }
        if (CheckCoordsStillInArea(A2)) {
            newCube = MixTemperaturesC(Cubes[A2.y][A2.x],newCube);       //
        }
        if (CheckCoordsStillInArea(A3)) {
            newCube = MixTemperaturesC(Cubes[A3.y][A3.x],newCube);       //
        }
        if (CheckCoordsStillInArea(B1)) {
            newCube = MixTemperaturesC(Cubes[B1.y][B1.x],newCube);       //
        }
        if (CheckCoordsStillInArea(B3)) {
            newCube = MixTemperaturesC(Cubes[B3.y][B3.x],newCube);       //
        }
        if (CheckCoordsStillInArea(C1)) {
            newCube = MixTemperaturesC(Cubes[C1.y][C1.x],newCube);       //
        }
        if (CheckCoordsStillInArea(C2)) {
            newCube = MixTemperaturesC(Cubes[C2.y][C2.x],newCube);       //
        }
        if (CheckCoordsStillInArea(C3)) {
            newCube = MixTemperaturesC(Cubes[C3.y][C3.x],newCube);       //
        }

        newTemperature_ = newCube.getTemperature();         // get calculated mixed temperature

        if (CheckCoordsStillInArea(A1)) {
            Cubes[A1.y][A1.x].setTemperature(newTemperature_); // set new mixed temperature at all cubes with valid coordinates
            Cubes[A1.y][A1.x].calcPressure(); // calculate pressure depending on new temperature
        }
        if (CheckCoordsStillInArea(A2)) {
            Cubes[A2.y][A2.x].setTemperature(newTemperature_); //
            Cubes[A2.y][A2.x].calcPressure(); //
        }
        if (CheckCoordsStillInArea(A3)) {
            Cubes[A3.y][A3.x].setTemperature(newTemperature_); //
            Cubes[A3.y][A3.x].calcPressure(); //
        }
        if (CheckCoordsStillInArea(B1)) {
            Cubes[B1.y][B1.x].setTemperature(newTemperature_); //
            Cubes[B1.y][B1.x].calcPressure(); //
        }

        Cubes[B2.y][B2.x].setTemperature(newTemperature_);     //
        Cubes[B2.y][B2.x].calcPressure();     //

        if (CheckCoordsStillInArea(B3)) {
            Cubes[B3.y][B3.x].setTemperature(newTemperature_); //
            Cubes[B3.y][B3.x].calcPressure(); //
        }
        if (CheckCoordsStillInArea(C1)) {
            Cubes[C1.y][C1.x].setTemperature(newTemperature_); //
            Cubes[C1.y][C1.x].calcPressure(); //
        }
        if (CheckCoordsStillInArea(C2)) {
            Cubes[C2.y][C2.x].setTemperature(newTemperature_); //
            Cubes[C2.y][C2.x].calcPressure(); //
        }
        if (CheckCoordsStillInArea(C3)) {
            Cubes[C3.y][C3.x].setTemperature(newTemperature_); //
            Cubes[C3.y][C3.x].calcPressure(); //
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

  0--------> x-axis
 0|
  |
  |
  |
  v y-axis


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
    double newTemperature;

    // left upper corner
    B2 = leftUpperCorner;             // calculation
    A1 = {.x = B2.x - 1, .y = B2.y - 1};    // of coords
    A2 = {.x = B2.x    , .y = B2.y - 1};    // depending on
    B1 = {.x = B2.x - 1, .y = B2.y    };    // B2

    if (CheckCoordsStillInArea(B2)) {  // if coordinate of B2 is not valid, all other coordinates are also not valid
        newCube = MixTemperaturesC(Cubes[B2.y][B2.x],Cubes[B2.y][B2.x]);   // calculate a mixing of all cubes with valid coordinates
        if (CheckCoordsStillInArea(A1)) {
            newCube = MixTemperaturesC(Cubes[A1.y][A1.x],newCube);         //
        }
        if (CheckCoordsStillInArea(A2)) {
            newCube = MixTemperaturesC(Cubes[A2.y][A2.x],newCube);         //
        }
        if (CheckCoordsStillInArea(B1)) {
            newCube = MixTemperaturesC(Cubes[B1.y][B1.x],newCube);         //
        }
        newTemperature = newCube.getTemperature();         // get calculated mixed temperature
        Cubes[B2.y][B2.x].setTemperature(newTemperature);      // set new mixed temperature at all cubes with valid coordinates
        Cubes[B2.y][B2.x].calcPressure();                        // calculate pressure depending on new temperature
        if (CheckCoordsStillInArea(A1)) {
            Cubes[A1.y][A1.x].setTemperature(newTemperature);  //
            Cubes[A1.y][A1.x].calcPressure();                    //
        }
        if (CheckCoordsStillInArea(A2)) {
            Cubes[A2.y][A2.x].setTemperature(newTemperature);  //
            Cubes[A2.y][A2.x].calcPressure();                    //
        }
        if (CheckCoordsStillInArea(B1)) {
            Cubes[B1.y][B1.x].setTemperature(newTemperature);  //
            Cubes[B1.y][B1.x].calcPressure();                    //
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
        newCube = MixTemperaturesC(Cubes[B1.y][B1.x],Cubes[B1.y][B1.x]);   // calculate a mixing of all cubes with valid coordinates
        if (CheckCoordsStillInArea(A1)) {
            newCube = MixTemperaturesC(Cubes[A1.y][A1.x],newCube);         //
        }
        if (CheckCoordsStillInArea(A2)) {
            newCube = MixTemperaturesC(Cubes[A2.y][A2.x],newCube);         //
        }
        if (CheckCoordsStillInArea(B2)) {
            newCube = MixTemperaturesC(Cubes[B2.y][B2.x],newCube);         //
        }
        newTemperature = newCube.getTemperature();         // get calculated mixed temperature
        Cubes[B1.y][B1.x].setTemperature(newTemperature);      // set new mixed temperature at all cubes with valid coordinates
        Cubes[B1.y][B1.x].calcPressure();                        // calculate pressure depending on new temperature
        if (CheckCoordsStillInArea(A1)) {
            Cubes[A1.y][A1.x].setTemperature(newTemperature);  //
            Cubes[A1.y][A1.x].calcPressure();                    //
        }
        if (CheckCoordsStillInArea(A2)) {
            Cubes[A2.y][A2.x].setTemperature(newTemperature);  //
            Cubes[A2.y][A2.x].calcPressure();                    //
        }
        if (CheckCoordsStillInArea(B2)) {
            Cubes[B2.y][B2.x].setTemperature(newTemperature);  //
            Cubes[B2.y][B2.x].calcPressure();                    //
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
        newCube = MixTemperaturesC(Cubes[A2.y][A2.x],Cubes[A2.y][A2.x]);   // calculate a mixing of all cubes with valid coordinates
        if (CheckCoordsStillInArea(A1)) {
            newCube = MixTemperaturesC(Cubes[A1.y][A1.x],newCube);         //
        }
        if (CheckCoordsStillInArea(B1)) {
            newCube = MixTemperaturesC(Cubes[B1.y][B1.x],newCube);         //
        }
        if (CheckCoordsStillInArea(B2)) {
            newCube = MixTemperaturesC(Cubes[B2.y][B2.x],newCube);         //
        }
        newTemperature = newCube.getTemperature();         // get calculated mixed temperature
        Cubes[A2.y][A2.x].setTemperature(newTemperature);      // set new mixed temperature at all cubes with valid coordinates
        Cubes[A2.y][A2.x].calcPressure();                        // calculate pressure depending on new temperature
        if (CheckCoordsStillInArea(A1)) {
            Cubes[A1.y][A1.x].setTemperature(newTemperature);  //
            Cubes[A1.y][A1.x].calcPressure();                    //
        }
        if (CheckCoordsStillInArea(B1)) {
            Cubes[B1.y][B1.x].setTemperature(newTemperature);  //
            Cubes[B1.y][B1.x].calcPressure();                    //
        }
        if (CheckCoordsStillInArea(B2)) {
            Cubes[B2.y][B2.x].setTemperature(newTemperature);  //
            Cubes[B2.y][B2.x].calcPressure();                    //
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
        newCube = MixTemperaturesC(Cubes[A1.y][A1.x],Cubes[A1.y][A1.x]);   // calculate a mixing of all cubes with valid coordinates
        if (CheckCoordsStillInArea(A2)) {
            newCube = MixTemperaturesC(Cubes[A2.y][A2.x],newCube);         //
        }
        if (CheckCoordsStillInArea(B1)) {
            newCube = MixTemperaturesC(Cubes[B1.y][B1.x],newCube);         //
        }
        if (CheckCoordsStillInArea(B2)) {
            newCube = MixTemperaturesC(Cubes[B2.y][B2.x],newCube);         //
        }

        newTemperature = newCube.getTemperature();  // getting calculated mixed temperature
        Cubes[A1.y][A1.x].setTemperature(newTemperature);      // set new mixed temperature
        Cubes[A1.y][A1.x].calcPressure();                        // calculate pressure depending on new temperature

        if (CheckCoordsStillInArea(A2)) {
            Cubes[A2.y][A2.x].setTemperature(newTemperature);  //
            Cubes[A2.y][A2.x].calcPressure();                    //
        }
        if (CheckCoordsStillInArea(B1)) {
            Cubes[B1.y][B1.x].setTemperature(newTemperature);  //
            Cubes[B1.y][B1.x].calcPressure();                    //
        }
        if (CheckCoordsStillInArea(B2)) {
            Cubes[B2.y][B2.x].setTemperature(newTemperature);  //
            Cubes[B2.y][B2.x].calcPressure();                    //
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
            newCube = MixTemperaturesC(Cubes[A1.y][A1.x],Cubes[A2.y][A2.x]);
            newTemperature = newCube.getTemperature();  // getting calculated mixed temperature
            Cubes[A1.y][A1.x].setTemperature(newTemperature);  // set new mixed temperature
            Cubes[A1.y][A1.x].calcPressure();                    // calculate pressure depending on new temperature
            Cubes[A2.y][A2.x].setTemperature(newTemperature);  //
            Cubes[A2.y][A2.x].calcPressure();
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
            newCube = MixTemperaturesC(Cubes[A1.y][A1.x],Cubes[A2.y][A2.x]);
            newTemperature = newCube.getTemperature();  // getting calculated mixed temperature
            Cubes[A1.y][A1.x].setTemperature(newTemperature);  // set new mixed temperature
            Cubes[A1.y][A1.x].calcPressure();                    // calculate pressure depending on new temperature
            Cubes[A2.y][A2.x].setTemperature(newTemperature);  //
            Cubes[A2.y][A2.x].calcPressure();
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
            newCube = MixTemperaturesC(Cubes[A1.y][A1.x],Cubes[B1.y][B1.x]);
            newTemperature = newCube.getTemperature();  // getting calculated mixed temperature
            Cubes[A1.y][A1.x].setTemperature(newTemperature);  // set new mixed temperature
            Cubes[A1.y][A1.x].calcPressure();                    // calculate pressure depending on new temperature
            Cubes[B1.y][B1.x].setTemperature(newTemperature);  //
            Cubes[B1.y][B1.x].calcPressure();
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
            newCube = MixTemperaturesC(Cubes[A1.y][A1.x],Cubes[B1.y][B1.x]);
            newTemperature = newCube.getTemperature();  // getting calculated mixed temperature
            Cubes[A1.y][A1.x].setTemperature(newTemperature);  // set new mixed temperature
            Cubes[A1.y][A1.x].calcPressure();                    // calculate pressure depending on new temperature
            Cubes[B1.y][B1.x].setTemperature(newTemperature);  //
            Cubes[B1.y][B1.x].calcPressure();
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
double Area::MixTemperatures(Cube Cube1, Cube Cube2) {
    double Temp1 = Cube1.getTemperature();
    double Mass1 = Cube1.calcMass();
    double Volume1 = Cube1.getVolume();
    double Temp2 = Cube2.getTemperature();
    double Mass2 = Cube2.calcMass();
    double Volume2 = Cube2.getVolume();
    return MixTemperatures(Temp1, Mass1, Volume1, Temp2, Mass2, Volume2);
};

/**

returns new Temperature
*/
double Area::MixTemperatures(double Temp1, double Mass1, double Volume1, double Temp2, double Mass2, double Volume2) {
    double newTemperature_ = ( (Mass1 * Temp1) + (Mass2 * Temp2) ) / (Mass1 + Mass2);
    return newTemperature_;
};

/**

returns new cube object which has
 -> new Temperature = mixed Temperature
 -> new Volume      = Cube1.Volume + Cube2.Volume
 -> new Mass        = Cube1.Mass + Cube2.Mass
*/
Cube Area::MixTemperaturesC(Cube Cube1, Cube Cube2) {
    double Temp1 = Cube1.getTemperature();
    double Mass1 = Cube1.calcMass();
    double Volume1 = Cube1.getVolume();
    double Temp2 = Cube2.getTemperature();
    double Mass2 = Cube2.calcMass();
    double Volume2 = Cube2.getVolume();
    Cube newCube = Cube1;
    newCube.setTemperature(MixTemperatures(Temp1, Mass1, Volume1, Temp2, Mass2, Volume2));
    newCube.setVolume(Volume1 + Volume2);
    newCube.calcMass();
    return newCube;
};

/* --- simulation --- */
// simulation
/**
 * Area::initSimulation()
 *
 * @brief sets speed of all cubes to 0;
 */
void Area::initSimulation() {
    for (int y = 0; y < Cubes.size(); y++) {
        for (int x = 0; x < Cubes[y].size(); x++) {
            Cubes[y][x].initSimulation();
        }
    }
};


/**
 @todo doc
 */
void Area::simulate(double timeStepInSeconds_, double simulationSpeedInSeconds_, bool printEveryCalculation_) {
    int startTime = GetTimeMs64();
    double timeDelta = 0;

    // while-loop == whole simulation
    // --> one loop execution is one simulationStep for displaying
    //     and (simulationSpeedInSeconds / timeStepInSeconds) simulation steps for calculation
    int i = 0;
    while (i < 1000) {
        i++;
        cout << "ShowSimulation [][][][][]" << endl;
        double Max = 0;
        double Min = DBL_MAX;

        if (!printEveryCalculation_) {
            for (int i = 0; i < (simulationSpeedInSeconds_ / timeStepInSeconds_); i++) {
                simulateTimeStep(timeStepInSeconds_,i);
                //if (GetMinMaxValue("S",false) < 0) {
                double tempMin = GetMinMaxValue("S",false);
                if (tempMin < Min) {
                    Min = tempMin;
                }
                double tempMax = GetMinMaxValue("S",true);
                if (tempMax > Max) {
                    Max = tempMax;
                }
                //}
            }
            PrintCubes("P");                // print only after last calculation
        } else {
            simulateTimeStep(timeStepInSeconds_,i);
            double tempMin = GetMinMaxValue("S",false);
            if (tempMin < Min) {
                Min = tempMin;
            }
            double tempMax = GetMinMaxValue("S",true);
            if (tempMax > Max) {
                Max = tempMax;
            }
            PrintCubes("P");                // print only after last calculation
        }

        cout << "Max : " << Max << "Min" << Min << endl;
        while (timeDelta <= simulationSpeedInSeconds_ * 1000) { // wait until display simulation time has ended
            // wait
            int newTime = GetTimeMs64();
            timeDelta = newTime-startTime;
        }

        timeDelta = 0;
        startTime = GetTimeMs64();
        // TODO output simulation results to console



    }
};

/**
 * simulateAirExchange
 * simulateTemperatureExchange
 * iterate all Cubes
 *   | Cube.recalculateAttributes()
 *
 *
 */
void Area::simulateTimeStep(double timeStepInSeconds_, int sqliteCounter_) {
    //TODO
    if (SHOW_IN_DETAIL) {
        cout << "simulateTimeStep - dummy :P --> crunching data <--" << endl;
    }

    simulateTemperatureChanges();
    simulateAirExchange(timeStepInSeconds_);
    simulateTemperatureExchange(timeStepInSeconds_);
    for (int y = 0; y < Cubes.size(); y++) {
        for (int x = 0; x < Cubes[y].size(); x++) {
            Cubes[y][x].recalculateAttributes();
            Cubes[y][x].clearAirDeltas();
        }
    }
        //write Data To SQLite
    //if (sqliteCounter_ % 10 == 0) {
            saveToSQLite();
    //}

};

void Area::simulateTemperatureChanges() {
    // TODO read database
    string path = "tempChanges.txt";
    ifstream file(path.c_str());
    if (file) {
        int x,y;
        bool tempChangePositive;
        while(file >> y) {
            file >> x;
            file >> tempChangePositive;
            if (tempChangePositive) {
                ModifyTemperature(x,y,"+");
            } else {
                ModifyTemperature(x,y,"-");
            }
        }
    }
}

/**
 * Area::simulateAirExchange(double timeStepInSeconds_)
 *
 * @brief
 *
 * iterate all Cubes
 *   | Cube.calcForces()
 *   | Cube.calcAcceleration()
 *   | Cube.calcSpeed()
 *   | list of airDeltas = calcOutAirDeltas()
 *   | iterate all airDeltas // always 1 or two per cube
 *   |   | NewCube.addInAirDelta(airDelta)
 *
 *
 */
void Area::simulateAirExchange(double timeStepInSeconds_) {
    coords c;
    for (int y = 0; y < Cubes.size(); y++) {
        for (int x = 0; x < Cubes[y].size(); x++) {
            if ((x == 3) && (y == 0)) {  // todo delete debug code
              double todo = 1;
            }
            c.x = x;
            c.y = y;
            //Cubes[c.y][c.x].calcForces(Cubes, timeStepInSeconds_); is now used in calcAcceleration
            //calculateForces(c, timeStepInSeconds_); // old function of class Area
            NumericIntegrator NI;
            //NI
            vector3 previousSpeed = Cubes[c.y][c.x].getSpeed();
            //Cubes[c.y][c.x].calcAcceleration(timeStepInSeconds_, previousSpeed); // old is now done by numeric integrator
            //Cubes[c.y][c.x].calcSpeed(timeStepInSeconds_);  // old is now done by numeric integrator
            NI.setObjectOfFunctionToIntegrate(&Cubes[c.y][c.x]);
            NI.setFunctionToIntegrate(&Cube::calcAcceleration);
            vector3 newSpeed = NI.integrateOneTimeStep(timeStepInSeconds_, IntegrationType::EULER_CAUCHY, previousSpeed);
            Cubes[c.y][c.x].setSpeed(newSpeed);
            list<airDelta> tempOutAirDeltas = Cubes[c.y][c.x].calcLeavingAirDeltas(timeStepInSeconds_);
            for(auto iterateOutAirDeltas = tempOutAirDeltas.begin(); iterateOutAirDeltas != tempOutAirDeltas.end(); iterateOutAirDeltas++) {
                airDelta tempAirDelta = *iterateOutAirDeltas;
                Cubes[tempAirDelta.newCoords.y][tempAirDelta.newCoords.x].addInAirDelta(tempAirDelta);
            }
        }
    }

};

void Area::simulateTemperatureExchange(double timeStepInSeconds_) {
    simulateHeatConduction(timeStepInSeconds_);
};

void Area::simulateHeatConduction(double timeStepInSeconds_) {
    coords c;
    for (int y = 0; y < Cubes.size(); y++) {
        for (int x = 0; x < Cubes[y].size(); x++) {
            c.x = x;
            c.y = y;
            coords up    = {.x = c.x,     .y = c.y - 1};
            coords down  = {.x = c.x, .    y = c.y + 1};
            coords left  = {.x = c.x - 1, .y = c.y    };
            coords right = {.x = c.x + 1, .y = c.y    };
            double tempTemperatureDelta = 0;

            tempTemperatureDelta += calculateTemperatureDelta(c, up, timeStepInSeconds_);
            tempTemperatureDelta += calculateTemperatureDelta(c, down, timeStepInSeconds_);
            tempTemperatureDelta += calculateTemperatureDelta(c, left, timeStepInSeconds_);
            tempTemperatureDelta += calculateTemperatureDelta(c, right, timeStepInSeconds_);
            Cubes[c.y][c.x].setTemperatureDelta(tempTemperatureDelta);
        }
    }
    for (int y = 0; y < Cubes.size(); y++) {
        for (int x = 0; x < Cubes[y].size(); x++) {
            double tempTemperatureDelta = Cubes[y][x].getTemperatureDelta();
            double oldTemperature = Cubes[y][x].getTemperature();
            double newTemperature = oldTemperature + tempTemperatureDelta;
            Cubes[y][x].setTemperature(newTemperature);
        }
    }
};

void Area::openSQLite(string path_) {
    db = openDataBase(path_);
};

void Area::saveToSQLite() {
    writeToDataBaseFast(db,Cubes);
};

double Area::calculateTemperatureDelta(coords fromCoords_, coords toCoords_, double timeStepInSeconds_) {
   // bool left = false;
    temperatureDelta tempTemperatureDelta;
    double temperatureDifference;
    double A, l;
    if (CheckCoordsStillInArea(fromCoords_)) {
        if ((toCoords_.x == fromCoords_.x - 1 ) || (toCoords_.x == fromCoords_.x + 1)) { // left or right
            A = Cubes[fromCoords_.y][fromCoords_.x].getLength(); // size of contact-area between the two cubes [m^2]
            l = Cubes[fromCoords_.y][fromCoords_.x].getWidth(); // distance (between the two midpoints of the two cubes [m])
        } else if  ((toCoords_.y == fromCoords_.y - 1) || (toCoords_.y == fromCoords_.y + 1)) { // up or Down
            A = Cubes[fromCoords_.y][fromCoords_.x].getWidth(); // size of contact-area between the two cubes [m^2]
            l = Cubes[fromCoords_.y][fromCoords_.x].getLength(); // distance (between the two midpoints of the two cubes [m])
        } else { // error
            return 0;
        }

        if (CheckCoordsStillInArea(toCoords_)) {
            double fromTemperature = Cubes[fromCoords_.y][fromCoords_.x].getTemperature();
            double toTemperature   = Cubes[toCoords_.y][toCoords_.x].getTemperature();
            temperatureDifference = toTemperature - fromTemperature;
        } else { // Todo
            double fromTemperature = Cubes[fromCoords_.y][fromCoords_.x].getTemperature();
            double toTemperature   = 288;
            temperatureDifference = toTemperature - fromTemperature;
            //temperatureDifference = 0;
        }
        double dQ = THERMAL_CONDUCTIVITY_COEFFICIENT_AIR * (A/l) * timeStepInSeconds_ * temperatureDifference; // dQ = HeatDifference [J]
        double tempMass = Cubes[fromCoords_.y][fromCoords_.x].calcMass();
        double temperatureDifferenceWhichIsToAddOnOldTemperature = dQ / (SPECIFIC_THERMAL_CAPACITY_AIR * tempMass); // [K]
        return temperatureDifferenceWhichIsToAddOnOldTemperature;

    } else { // error
        return 0;
    }
};

double Area::decreaseTemperatureUntilStandardTemperature(double temperature_,double timeStepInSeconds_) {
    if (temperature_ > 288) {
        temperature_ -= MODIFY_TEMPERATURE_DELTA / 10 * timeStepInSeconds_;
    }
    if (temperature_ < 288) {
        temperature_ = 288;
    }
    return temperature_;
};

// calculating forces

void Area::calculateForces(coords c,double timeStepInSeconds_) {
    Cubes[c.y][c.x].clearForce();
    Cubes[c.y][c.x].addForce(calculateGradientForce(c));
    Cubes[c.y][c.x].addForce(calculateCoriolisForce(c,timeStepInSeconds_));
    Cubes[c.y][c.x].addForce(calculateFrictionForce(c,timeStepInSeconds_));
};


vector3 Area::calculateGradientForce(coords c) {
    vector3 tempForces      = {.x = 0, .y = 0, .z = 0};
    coords leftUpperCorner  = {.x = c.x - 1, .y = c.y - 1};
    coords up               = {.x = c.x    , .y = c.y - 1};
    coords rightUpperCorner = {.x = c.x + 1, .y = c.y - 1};
    coords right            = {.x = c.x + 1, .y = c.y    };
    coords rightLowerCorner = {.x = c.x + 1, .y = c.y + 1};
    coords down             = {.x = c.x    , .y = c.y + 1};
    coords leftLowerCorner  = {.x = c.x - 1, .y = c.y + 1};
    coords left             = {.x = c.x - 1, .y = c.y    };
    tempForces = sumVector3(tempForces,calculateGradientForce(c, leftUpperCorner));
    tempForces = sumVector3(tempForces,calculateGradientForce(c, up));
    tempForces = sumVector3(tempForces,calculateGradientForce(c, rightUpperCorner));
    tempForces = sumVector3(tempForces,calculateGradientForce(c, right));
    tempForces = sumVector3(tempForces,calculateGradientForce(c, rightLowerCorner));
    tempForces = sumVector3(tempForces,calculateGradientForce(c, down));
    tempForces = sumVector3(tempForces,calculateGradientForce(c, leftLowerCorner));
    tempForces = sumVector3(tempForces,calculateGradientForce(c, left));
/*    tempForces += calculateGradientForce{c, leftUpperCorner};
    tempForces += calculateGradientForce{c, up};
    tempForces += calculateGradientForce{c, rightUpperCorner};
    tempForces += calculateGradientForce{c, right};
    tempForces += calculateGradientForce{c, rightLowerCorner};
    tempForces += calculateGradientForce{c, down};
    tempForces += calculateGradientForce{c, leftLowerCorner};
    tempForces += calculateGradientForce{c, left};
*/

    // TODO
    /*
    tempForces.x = 0.00010;
    tempForces.y = 0.00040;
    tempForces.z = 0.0;*/
    return tempForces;
};

/**
 * @brief calculates the gradientforce beetween two cubes
 * @param fromCube_ the cube for which you calculate the force
 * @param toCube_ the cube the calculation is referencing to
 */
vector3 Area::calculateGradientForce(coords fromCube_, coords toCube_) {
    vector3 tempGradientForce;
    if (CheckCoordsStillInArea(fromCube_) && CheckCoordsStillInArea(toCube_)) {
        double tempMass        = Cubes[fromCube_.y][fromCube_.x].calcMass();
        double tempDensity     = Cubes[fromCube_.y][fromCube_.x].calcDensity();
        double pressureDifference = Cubes[fromCube_.y][fromCube_.x].calcPressure()-Cubes[toCube_.y][toCube_.x].calcPressure();
        vector3 positionOfFromCube;
        positionOfFromCube.x = (fromCube_.x + 0.5) * (Cubes[fromCube_.y][fromCube_.x].getWidth());
        positionOfFromCube.y = (fromCube_.y + 0.5) * (Cubes[fromCube_.y][fromCube_.x].getLength());
        positionOfFromCube.z = (         0  + 0.5) * (Cubes[fromCube_.y][fromCube_.x].getHeight());
        vector3 positionOfToCube;
        positionOfToCube.x = (toCube_.x + 0.5) * (Cubes[toCube_.y][toCube_.x].getWidth());
        positionOfToCube.y = (toCube_.y + 0.5) * (Cubes[toCube_.y][toCube_.x].getLength());
        positionOfToCube.z = (       0  + 0.5) * (Cubes[toCube_.y][toCube_.x].getHeight());

        double xDifference = positionOfToCube.x - positionOfFromCube.x;
        double yDifference = positionOfToCube.y - positionOfFromCube.y;
        double zDifference = positionOfToCube.z - positionOfFromCube.z;
        if (xDifference != 0) {
            tempGradientForce.x = - (tempMass / tempDensity) * (pressureDifference / (xDifference)) * (-1);
        } else {
            tempGradientForce.x = 0;
        }
        if (yDifference != 0) {
            tempGradientForce.y = - (tempMass / tempDensity) * (pressureDifference / (yDifference)) * (-1);
        } else {
            tempGradientForce.y = 0;
        }
        if (zDifference != 0) {
            tempGradientForce.z = - (tempMass / tempDensity) * (pressureDifference / (zDifference)) * (-1);
        } else  {
            tempGradientForce.z = 0;
        }
    } else {
        tempGradientForce.x = 0;
        tempGradientForce.y = 0;
        tempGradientForce.z = 0;
    }
    return tempGradientForce;
};

vector3 Area::calculateCoriolisForce(coords c,double timeStepInSeconds_) {
    // TODO
    vector3 tempForces;
    tempForces.x = 0.0;
    tempForces.y = 0.0;
    tempForces.z = 0.0;
    return tempForces;
};

vector3 Area::calculateFrictionForce(coords c,double timeStepInSeconds_) {
    // TODO
    //vector3 tempForces;
    //tempForces.x = 0;//-0.00003;
    //tempForces.y = 0;//-0.00002;
    //tempForces.z = 0.0;
    surfaceRoughnessType tempSurfaceRoughness = Cubes[c.y][c.x].getSurfaceRoughness();
    double HellmannExponent;
    switch (tempSurfaceRoughness) {
        case WATER                   : HellmannExponent = HELLMANN_EXPONENT_WATER;                   break;
        case MEADOW                  : HellmannExponent = HELLMANN_EXPONENT_MEADOW;                  break;
        case MEADOW_WITH_MANY_HEDGES : HellmannExponent = HELLMANN_EXPONENT_MEADOW_WITH_MANY_HEDGES; break;
        case PARK_LANDSCAPE          : HellmannExponent = HELLMANN_EXPONENT_PARK_LANDSCAPE;          break;
        case DENSELY_BUILT_UP_AREA   : HellmannExponent = HELLMANN_EXPONENT_DENSELY_BUILT_UP_AREA;   break;
        case SKYSCRAPER_CITY         : HellmannExponent = HELLMANN_EXPONENT_SKYSCRAPER_CITY;         break;
    }

    double tempMass = Cubes[c.y][c.x].getMass();
    vector3 oldSpeed = Cubes[c.y][c.x].getSpeed();
    double h0 = Cubes[c.y][c.x].getHeight() / 2; // reference-height
    vector3 frictionForce = oldSpeed * VISCOSITY_AIR * ( HellmannExponent * (HellmannExponent - 1) / (h0 * h0) ) * tempMass * 10000;
    return frictionForce;
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

vector3 Area::sumVector3(vector3 a, vector3 b) {
    a.x += b.x;
    a.y += b.y;
    a.z += b.z;
    return a;
}

/**
 *        -----------          ---_-_--
 *     ((    |   |    ))           |
 *   ((-----------------))         |   distanceGeoWidth between Cube 0,0 and cube x,y = (lengthOfCube(x,y) * y-coordinate of cube(x,y))
 *  (|   |   |   |   |   |)       _|_
 * (-----------------------)   ------- geoWidth
 *  (|   |   |   |   |    |)
 *   ((------------------))
 *     ((    |   |    ))
 *       -------------
 *
 *               geoLength
 *       |       |
 *       |-------|
 *         distanceGeoLength between Cube 0,0 and cube x,y = widthOfCube(x,y) * x-coordinate of cube (x,y)
 */


GeoCoords Area::calcGeoCoords(coords c_) {
    GeoCoords tempUpperLeftCube = getGeoCoordsUpperLeftCube();
    GeoCoords newGeoCoords;

    // geoHeight
    newGeoCoords.geoHeight = tempUpperLeftCube.geoHeight;

    // geoWidth
    double minutesPerKilometer = 1.0 / 1.852;
    double distanceToUpperLeftCube = Cubes[c_.y][c_.x].getLength() * c_.y; // in [m]
    double distanceInKilometer = distanceToUpperLeftCube / 1000;           // in [km]

    newGeoCoords.geoWidth = distanceInKilometer * minutesPerKilometer + tempUpperLeftCube.geoWidth;

    // geoLength
    minutesPerKilometer = 1.0 / (1.852 * cos(newGeoCoords.geoWidth * M_PI / 180.0));
    distanceToUpperLeftCube = Cubes[c_.y][c_.x].getWidth() * c_.x; // in [m]
    distanceInKilometer = distanceToUpperLeftCube / 1000;           // in [km]

    newGeoCoords.geoLength = distanceInKilometer * minutesPerKilometer + tempUpperLeftCube.geoLength;
    return newGeoCoords;
}


double* Area::getSimulationValues() {
    int tempCubesCountWidth = GetCubesCountWidth();
    int tempCubesCountLength = GetCubesCountLength();
    int arrayLength = (tempCubesCountWidth * tempCubesCountLength * 3) + 2;
    double simulationValues[arrayLength];
    simulationValues[0] = tempCubesCountWidth;
    simulationValues[1] = tempCubesCountLength;
    int counter = 1;
    for (int y = 0; y < Cubes.size(); y++) {
        for (int x = 0; x < Cubes[y].size(); x++) {
                vector3 tempForce = Cubes[y][x].getForce();
                counter++;
                simulationValues[counter] = tempForce.x;
                counter++;
                simulationValues[counter] = tempForce.y;
                counter++;
                simulationValues[counter] = tempForce.z;
        }
    }
    return simulationValues;
}

