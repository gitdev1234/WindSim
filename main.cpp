#include <iostream>
#include "include/Model_Cube.h"
#include "Model_Area.h"
#include <limits>
#include <math.h>
using namespace std;


enum programMode {START, ABORT, STAY_IN_LOOP, LOAD_STANDARD, CONFIGURE_AREA, LOAD_AREA_TEMPLATE, MODIFY_VAL, PRINT_AREA,  STOP};

int main() {
    int w_count, l_count, x, y;
    float w_total, l_total, h_total, w_cube, l_cube;
    GeoCoords geoCoords;
    string s;
    Model_Area MA;
    programMode ProgramMode = START;

    cout << "Welcome to this WindSim-Console UI." << endl;
    cout << "Do you want to load standard-values for your model area";
    cout << " or enter configuration mode? [Press S or C]" << endl;

    do {
        cin >> s;
        if (toupper(s[0]) == 'S') {
            ProgramMode = LOAD_STANDARD;
        } else if (toupper(s[0]) == 'C') {
            ProgramMode = CONFIGURE_AREA;
        } else {
            ProgramMode = STAY_IN_LOOP;
        }
    } while (ProgramMode == STAY_IN_LOOP);


    if (ProgramMode == LOAD_STANDARD) {
        cout << "Standard program mode configures an Area of 10 x 10 x 1 meters [w x l x h]." << endl;
        cout << "10 x 10 cubes will be simulated. Do you want to continue? [Y/N]" << endl;
        do {
            ProgramMode = STAY_IN_LOOP;
            cin >> s;
            if (toupper(s[0]) == 'Y') {
                ProgramMode = LOAD_AREA_TEMPLATE;
                MA.createStandardArea();
                cout << "Area configured succesfully." << endl;
            } else if (toupper(s[0]) == 'N') {
                cout << "You entered configuration mode." << endl;
                ProgramMode = CONFIGURE_AREA;
            } else {
                ProgramMode = STAY_IN_LOOP;
            }
        } while (ProgramMode == STAY_IN_LOOP);
    };

    if (ProgramMode == CONFIGURE_AREA) {
        while (ProgramMode == CONFIGURE_AREA) {
            cout << "What is the total size of your model area?" << endl;
            cout << "Enter the width in meters." << endl;
            cin  >> w_total;
            cout << "Enter the length in meters." << endl;
            cin  >> l_total;
            cout << "Enter the height in meters." << endl;
            cin  >> h_total;
            cout << "Enter the number of cubes you want to simulate in the direction of width." << endl;
            cin  >> w_count;
            cout << "Enter the number of cubes you want to simulate in the direction of length." << endl;
            cin  >> l_count;
            w_cube = w_total / w_count;
            l_cube = l_total / l_count;
            cout << "Enter the location of the upper left cube." << endl;
            cout << "At first enter the latitude of the coordinates in decimal-degrees (e.g. 53.083333)." << endl;
            cin  >> geoCoords.geoWidth;
            cout << "Now enter the longitude of the coordinates in decimal-degrees (e.g. 8.8)." << endl;
            cin  >> geoCoords.geoLength;
            cout << "Now enter the height of the coordinates in meters (e.g. 11.0)" << endl;
            cin  >> geoCoords.geoHeight;

            cout << "You have configured an Area of " << w_total;
            cout << " x " << l_total << " x " << h_total << " meters [w x l x h]." << endl;
            cout << "The total volume is " << (w_total*l_total*h_total) << "m^3." << endl;
            cout << "The model area is simulated by " << (w_count*l_count) << " cubes";
            cout << "of " << (w_cube*l_cube*h_total) << "m^3 each." << endl;
            cout << "At the location of Lat:" << geoCoords.geoWidth << "Long:" << geoCoords.geoLength << "H:" << geoCoords.geoHeight << endl;
            cout << "Do you want to continue with this configuration [Y/N]" << endl;
            do {
                ProgramMode = STAY_IN_LOOP;
                cin >> s;
                if (toupper(s[0]) == 'Y') {
                    ProgramMode = LOAD_AREA_TEMPLATE;
                    MA.createArea(w_count,l_count,h_total,w_total,l_total,geoCoords);
                    cout << "Area configured succesfully." << endl;
                } else if (toupper(s[0]) == 'N') {
                    ProgramMode = CONFIGURE_AREA;
                } else {
                    ProgramMode = STAY_IN_LOOP;
                }
            } while (ProgramMode == STAY_IN_LOOP);

        }
    }

    if (ProgramMode == LOAD_AREA_TEMPLATE) {
        cout << "Now the pressure and temperature structure of your area has to be configured." << endl;
        cout << "Do you want to load a balanced normal pressure and temperature situation for the whole area? [B]" << endl;
        cout << "Or do you want to load an AREA_TEMPLATE? [T]" << endl;
        do {
            ProgramMode = STAY_IN_LOOP;
            cin >> s;
            if (toupper(s[0]) == 'B') {
                MA.LoadBalancedAreaStructure();
                cout << "Configured balanced area situation succesfully." << endl;
                ProgramMode = MODIFY_VAL;
            } else if (toupper(s[0]) == 'T') {
                //MA.LoadAreaStructureTemplate(s)
                cout << "Not implemented yet. Program will stop. :(" << endl;
                ProgramMode = STOP;
            } else {
                ProgramMode = STAY_IN_LOOP;
            }
        } while (ProgramMode == STAY_IN_LOOP);
    }

    while (ProgramMode == MODIFY_VAL) {
        cout << "You are now in modify-values mode." << endl;
        cout << "Enter [M] for modifying." << endl;
        cout << "Enter [P] for printing the area." << endl;

        do {
            ProgramMode = STAY_IN_LOOP;
            cin >> s;
            if (toupper(s[0]) == 'M') {
                cout << "Enter the x-coordinate of the cube you want to modify." << endl;
                cin  >> x;
                cout << "Enter the y-coordinate of the cube you want to modify." << endl;
                cin  >> y;
                cout << "Enter '+' for increasing the temperature." << endl;
                cout << "Enter '-' for decreasing the temperature." << endl;
                cout << "Enter 'X' for getting back into previous menu." << endl;

                do {
                    ProgramMode = STAY_IN_LOOP;
                    cin >> s;
                    if (toupper(s[0]) == 'X') {
                        ProgramMode = MODIFY_VAL;
                    } else {
                        MA.ModifyTemperature(x,y,s);
                    }

                } while (ProgramMode == STAY_IN_LOOP);

            } else if (toupper(s[0]) == 'P') {
                cout << "Enter [C] for printing coordinates." << endl;
                cout << "Enter [T] for printing temperatures." << endl;
                cout << "Enter [M] for printing molecules_count." << endl;
                cout << "Enter [P] for printing pressures." << endl;
                cout << "You can also enter a combinition of all letters." << endl;
                cout << "Enter 'X' for getting back into previous menu." << endl;
                do {
                    ProgramMode = STAY_IN_LOOP;
                    cin >> s;
                    if (toupper(s[0]) == 'X') {
                        ProgramMode = MODIFY_VAL;
                    } else {
                        MA.PrintCubes(s);
                    }
                } while (ProgramMode == STAY_IN_LOOP);
            } else {
                ProgramMode = STAY_IN_LOOP;
            }
        } while (ProgramMode == STAY_IN_LOOP);

    }

    if  (ProgramMode == STOP) {
        MA.DestroyArea();
        return 0;
    }

}
