#ifndef NUMERICINTEGRATOR_H
#define NUMERICINTEGRATOR_H

#include <map>
#include <iostream>
#include "vector3.h"
#include <vector>
#include "Cube.h"

using namespace std;

enum IntegrationType {EULER_CAUCHY, HEUN, RUNGE_KUTTA, DORMAND_PRINCE};


class NumericIntegrator
{
    public:
        NumericIntegrator() {};
        ~NumericIntegrator() {};
        //vector<vector<Cube> >& , double, vector3

        vector3 integrate(vector3 (*functionToIntegrate_)(vector3), double lengthOfTimeStepInSeconds_, double startTimeInSeconds_, double endTimeInSeconds_,
                          IntegrationType integrationType_, bool createValueMap = false, vector3 previousValue_ = {.x = 0.0, .y = 0.0, .z = 0.0});
        vector3 integrateOneTimeStep(vector3 (*functionToIntegrate_)(vector3), double lengthOfTimeStepInSeconds_, IntegrationType integrationType_, vector3 previousValue_);
        map<double,vector3> getValueMap() {return values;};
        int writeValueMapToCSV(string path);

    private:
        vector3 integrateOneTimeStepEulerCauchy  (vector3 (*functionToIntegrate_)(vector3),double lengthOfTimeStepInSeconds_, vector3 previousValue_);
        vector3 integrateOneTimeStepHeun         (vector3 (*functionToIntegrate_)(vector3),double lengthOfTimeStepInSeconds_, vector3 previousValue_);
        vector3 integrateOneTimeStepRungeKutta   (vector3 (*functionToIntegrate_)(vector3),double lengthOfTimeStepInSeconds_, vector3 previousValue_);
        vector3 integrateOneTimeStepRungeKuttaNStepped(vector3 (*functionToIntegrate_)(vector3),double lengthOfTimeStepInSeconds_, vector3 previousValue_, int N);
        vector3 integrateOneTimeStepDormandPrince(vector3 (*functionToIntegrate_)(vector3),double lengthOfTimeStepInSeconds_, vector3 previousValue_);

        map<double,vector3> values; // <IntegrationTime,value>
};

#endif // NUMERICINTEGRATOR_H
