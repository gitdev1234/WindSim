#include <iostream>
#include "include/Cube.h"
#include "Area.h"
#include <limits>
#include <math.h>
using namespace std;

int main() {
    Area MA;
    //cout << "Please enter the path of the file you want to load : " << endl;
    //string path = "area_templates/11_40_vulcano.wsim";
    string path = "area_templates/11_1.wsim";
    //cin >> path;
    MA.LoadAreaStructureTemplate(path);
    cout << "Loaded area-structure-template successfully." << endl;
    MA.initSimulation();
    double simulationTimeStep = 10;
    double displayTimeStep = 1;
    bool printEveryCalc = false;
    cout << "Started Simulation" << endl;
    MA.simulate(simulationTimeStep,displayTimeStep,printEveryCalc);


    /*
    double *simulationValuesAr = MA.simulateTimeStep(simulationTimeStep,1);
    int CubesCountLength = simulationValuesAr[0];
    int CubesCountWidth = simulationValuesAr[1];
    cout << CubesCountLength << endl;
    cout << CubesCountWidth << endl;
    int CounterMax = CubesCountLength * CubesCountWidth + 2;
    for (int Counter = 2; Counter < CounterMax; Counter++) {
        cout << simulationValuesAr[Counter];
    }*/

    cout << "Simulation has ended" << endl;

    MA.DestroyArea();
}
