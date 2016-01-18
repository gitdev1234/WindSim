#ifndef NUMERICINTEGRATOR_H
#define NUMERICINTEGRATOR_H

#include <map>
#include <iostream>
#include "vector3.h"
#include "Cube.h"

using namespace std;

enum IntegrationType {EULER_CAUCHY, HEUN, RUNGE_KUTTA, DORMAND_PRINCE};

class MyClass {
    public:
    typedef vector3 (MyClass::*FPTR)(double, vector3);
    double x;
};

class NumericIntegrator {
    public:
        NumericIntegrator() {};
        ~NumericIntegrator() {};

        vector3 integrate(double lengthOfTimeStepInSeconds_, double startTimeInSeconds_, double endTimeInSeconds_,
                          IntegrationType integrationType_, bool createValueMap = false, vector3 previousValue_ = {.x = 0.0, .y = 0.0, .z = 0.0});
        vector3 integrateOneTimeStep(double lengthOfTimeStepInSeconds_, IntegrationType integrationType_, vector3 previousValue_);

        // function Pointer
        void setFunctionToIntegrate(Cube::FPTR val_);
        void setObjectOfFunctionToIntegrate(Cube *val_);
        vector3 callFunctionToIntegrate(double param1_, vector3 param2_);

        // value Map
        map<double,vector3> getValueMap() {return values;};
        int writeValueMapToCSV(string path);



    private:
        vector3 integrateOneTimeStepEulerCauchy        (double lengthOfTimeStepInSeconds_, vector3 previousValue_);
        vector3 integrateOneTimeStepHeun               (double lengthOfTimeStepInSeconds_, vector3 previousValue_);
        vector3 integrateOneTimeStepRungeKutta         (double lengthOfTimeStepInSeconds_, vector3 previousValue_);
        vector3 integrateOneTimeStepRungeKuttaNStepped (double lengthOfTimeStepInSeconds_, vector3 previousValue_, int N);
        vector3 integrateOneTimeStepDormandPrince      (double lengthOfTimeStepInSeconds_, vector3 previousValue_);

        map<double,vector3> values; // <IntegrationTime,value>

        Cube::FPTR functionToIntegrate;
        Cube *objectOfFunctionToIntegrate;

};

#endif // NUMERICINTEGRATOR_H
