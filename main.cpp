#include <iostream>
#include "include/Cube.h"
#include "Area.h"
#include <limits>
#include <math.h>
using namespace std;

int main() {
    int w_count, l_count, x, y;
    double w_total, l_total, h_total, w_cube, l_cube;
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
                cout << "Please enter the path of the file you want to load : " << endl;
                string path;
                cin >> path;
                MA.LoadAreaStructureTemplate(path);
                cout << "Loaded area-structure-template successfully." << endl;
                ProgramMode = MODIFY_VAL;
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
                cout << "Enter [X] for getting back into previous menu." << endl;
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
                double moleculesPerMoleculeGroup, simulationTimeStep, displayTimeStep;
                bool printEveryCalc;
                cout << "For simulation we still need two parameters:" << endl;
                cout << "1. The length of one timestep in seconds. This tells how detailed the simulation is calculated." << endl;
                cout << "2. The timestep in seconds between every print of the area." << endl;
                cout << "This tells how slow or fast the simulation is shown on display." << endl;
                cout << "So now please enter the simulation timestep." << endl;
                cin  >> simulationTimeStep;
                cout << "And now please enter the display timestep." << endl;
                cin  >> displayTimeStep;

                if (simulationTimeStep < displayTimeStep) {
                    cout << "Your simulation-time-step value (first value) is smaller than the display-time-step value (second value)." << endl;
                    cout << "Do you want to see every simulation timeStep?" << endl;
                    cout << "This will lead you to a simulation which is slower than real-time." << endl;
                    cout << "For example : " << endl;
                    cout << "If you entered : " << endl;
                    cout << "0.1 seconds for simulation-timestep and" << endl;
                    cout << "1   second for display-timestep" << endl;
                    cout << "If you now choose to print out every simulation timestep, you will see a simulation which is 10" << endl;
                    cout << "times slower than real-time. Because every simulation timestep is calculated with" << endl;
                    cout << "a time delta of 0.1 seconds, but is displayed for 1 second on the screen." << endl;
                    cout << "If you choose to not print out every simulation timestep, you get an real-time simulation," << endl;
                    cout << "which calculates 10 times per second new values, but displays them only once per second.)" << endl;

                    cout << "Enter [A] if you want to display every simulation-timestep." << endl;
                    cout << "Enter [O] if you want to display only once." << endl;
                    do {
                        ProgramMode = STAY_IN_LOOP;
                        cin >> s;
                        if (toupper(s[0]) == 'A') {
                            printEveryCalc = true;
                            ProgramMode = SIMULATION;
                        } else if (toupper(s[0]) == 'O') {
                            printEveryCalc = false;
                            ProgramMode = SIMULATION;
                        } else {
                            ProgramMode = STAY_IN_LOOP;
                        }
                    } while (ProgramMode == STAY_IN_LOOP);
                } else {
                    printEveryCalc = false;
                    ProgramMode = SIMULATION;
                }

                cout << "Simulation starts now, if you want to stop simulation press [X]" << endl;
                MA.initSimulation();
                MA.simulate(simulationTimeStep,displayTimeStep,printEveryCalc);
                cout << "Simulation has ended." << endl;
                cout << "Enter [C] if you want to continue the simulation" << endl;
                cout << "Enter [S] if you want to to Save current Area" << endl;
                cout << "Enter [R] if you want to restart the programm." << endl;
                cout << "Enter [X] if you want to exit the programm" << endl;
                do {
                    ProgramMode = STAY_IN_LOOP;
                    cin >> s;
                    if (toupper(s[0]) == 'C') {
                        ProgramMode = STOP;
                        // TODO
                    }
                    if (toupper(s[0]) == 'S') {
                        cout << "Please enter a path for the resulting file : " << endl;
                        string tempPath;
                        cin >> tempPath;
                        MA.saveAreaStructureTemplate(tempPath);
                        cout << "Area has been saved successfully." << endl;
                        cout << "Enter [C] if you want to continue the simulation" << endl;
                        cout << "Enter [R] if you want to restart the programm." << endl;
                        cout << "Enter [X] if you want to exit the programm" << endl;
                    }
                    if (toupper(s[0]) == 'R') {
                        ProgramMode = STOP;
                    }
                    if (toupper(s[0]) == 'X') {
                        ProgramMode = STOP;
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
