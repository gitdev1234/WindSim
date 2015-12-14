#include <iostream>
#include "include/Cube.h"
#include "Area.h"
#include <limits>
#include <math.h>
using namespace std;


enum programMode {START, ABORT, STAY_IN_LOOP, LOAD_STANDARD, CONFIGURE_AREA, LOAD_AREA_TEMPLATE, MODIFY_VAL, PRINT_AREA,  STOP};

int main() {
    int w_count, l_count, x, y;
    float w_total, l_total, h_total, w_cube, l_cube;
    GeoCoords geoCoords;
    string s;
    Area MA;
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
        cout << "Enter [S] for starting the simulation" << endl;

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
            } else if (toupper(s[0]) == 'S') {
                int moleculeGroupsPerCube;
                float moleculesPerMoleculeGroup, timeStepInSeconds, simulationSpeedInSeconds;
                cout << "At current speed of most of the computers, this program can not simulate ";
                cout << "every molecule in our Model-Area." << endl;
                cout << "Thus we have to abstract our simulation a bit." << endl;
                cout << "Please enter how many molecule-groups shall be simulated in one cube." << endl;
                cout << "For example, if you enter 1000, then every cube contains 1000 molecule-groups." << endl;
                cout << "Every molecule-group represents a number of molecules and all moleules within a ";
                cout << "molecule-group behave equal." << endl;
                cout << "Notice that the number of molecule-groups has to be a square number. ";
                cout << "Other values can not be simulated. If you enter another value it is ";
                cout << "automatically transformed to a square number." << endl;
                cout << "So please enter an number of molecules per cube." << endl;
                cin  >> moleculeGroupsPerCube;
                float quadraticMoleculeGroupsPerCube = round(sqrt(moleculeGroupsPerCube)) * round(sqrt(moleculeGroupsPerCube));
                if (quadraticMoleculeGroupsPerCube != moleculeGroupsPerCube) {
                    cout << "Your value was automatically transformed to the quadratic value " << quadraticMoleculeGroupsPerCube << endl;
                }
                moleculesPerMoleculeGroup = MA.getMoleculesPerCubeAfterStart() / quadraticMoleculeGroupsPerCube;
                cout << "Okay you entered : " << quadraticMoleculeGroupsPerCube << ". This means that every molecule group within a cube ";
                cout << "represents " << moleculesPerMoleculeGroup << " molecules." << endl;
                cout << "For simulation we still need two parameters:" << endl;
                cout << "1. The length of one timestep in seconds. This tells how detailed the simulation is calculated." << endl;
                cout << "2. The length of how long every timestep shall be displayed. This tells how slow or fast the simulation ";
                cout << "is shown on display." << endl;
                cout << "For example if you enter first a 0.25 and second a 1 the programm calculates all moving of molecules and ";
                cout << "changing of air pressure and so on for always a timestep of 0.25seconds (250 ms), but it displays every ";
                cout << "timestep for 1 second on the screen. This means you are seeing the simulation not in real-time, but four ";
                cout << "times slower." << endl;
                cout << "If you would enter first a 0.5 and second a 0.5, then you have a real-time simulation in which the ";
                cout << "programm simulates always timesteps of 0.5 seconds (500 ms) and shows every timestep for 0.5 seconds on ";
                cout << "the screen." << endl;
                cout << "So now please enter first value." << endl;
                cin  >> timeStepInSeconds;
                cout << "And now please enter second value." << endl;
                cin  >> simulationSpeedInSeconds;
                cout << "Simulation starts now, if you want to stop simulation press [X]" << endl;
                MA.initSimulation(quadraticMoleculeGroupsPerCube);
                MA.simulate(timeStepInSeconds,simulationSpeedInSeconds);
                do {
                    ProgramMode = STAY_IN_LOOP;
                    if (toupper(s[0]) == 'X') {
                        ProgramMode = MODIFY_VAL;
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
