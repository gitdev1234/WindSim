#include "NumericIntegrator.h"
#include <fstream>

vector3 NumericIntegrator::integrate(double lengthOfTimeStepInSeconds_, double startTimeInSeconds_, double endTimeInSeconds_,
                                    IntegrationType integrationType_, bool createValueMap, vector3 previousValue_) {
    if (createValueMap) {
        values.clear();
    }
    vector3 nextValue;
    double integrationTime = startTimeInSeconds_;
    while (integrationTime <= endTimeInSeconds_) {
        nextValue = integrateOneTimeStep(lengthOfTimeStepInSeconds_,integrationType_,previousValue_);
        if (createValueMap) {
            values[integrationTime] = nextValue;
        }
        previousValue_ = nextValue;
        integrationTime += lengthOfTimeStepInSeconds_;
    }

    return nextValue;
};

vector3 NumericIntegrator::integrateOneTimeStep(double lengthOfTimeStepInSeconds_, IntegrationType integrationType_, vector3 previousValue_) {
    vector3 nextValue;
    switch(integrationType_) {
        case EULER_CAUCHY   : nextValue = integrateOneTimeStepEulerCauchy  (lengthOfTimeStepInSeconds_,previousValue_); break;
        case HEUN           : nextValue = integrateOneTimeStepHeun         (lengthOfTimeStepInSeconds_,previousValue_); break;
        case RUNGE_KUTTA    : nextValue = integrateOneTimeStepRungeKutta   (lengthOfTimeStepInSeconds_,previousValue_); break;
        case DORMAND_PRINCE : nextValue = integrateOneTimeStepDormandPrince(lengthOfTimeStepInSeconds_,previousValue_); break;
    }
    return nextValue;
};

vector3 NumericIntegrator::integrateOneTimeStepEulerCauchy(double lengthOfTimeStepInSeconds_, vector3 previousValue_) {
    vector3 nextValue = previousValue_ + lengthOfTimeStepInSeconds_ * callFunctionToIntegrate(lengthOfTimeStepInSeconds_, previousValue_);
    return nextValue;
};

vector3 NumericIntegrator::integrateOneTimeStepHeun         (double lengthOfTimeStepInSeconds_, vector3 previousValue_) {
    vector3 nextValue = previousValue_ + lengthOfTimeStepInSeconds_ *
                        (0.5 * ( callFunctionToIntegrate(lengthOfTimeStepInSeconds_, previousValue_) +
                                 callFunctionToIntegrate(lengthOfTimeStepInSeconds_, previousValue_ + lengthOfTimeStepInSeconds_ *
                                                         callFunctionToIntegrate(lengthOfTimeStepInSeconds_, previousValue_)
                                                        )
                               )
                        );
    return nextValue;
};

vector3 NumericIntegrator::integrateOneTimeStepRungeKutta   (double lengthOfTimeStepInSeconds_, vector3 previousValue_) {
    vector3 k1 = callFunctionToIntegrate(lengthOfTimeStepInSeconds_, previousValue_);
    vector3 k2 = callFunctionToIntegrate(lengthOfTimeStepInSeconds_, previousValue_ + (lengthOfTimeStepInSeconds_ / 2.0) * k1);
    vector3 k3 = callFunctionToIntegrate(lengthOfTimeStepInSeconds_, previousValue_ + (lengthOfTimeStepInSeconds_ / 2.0) * k2);
    vector3 k4 = callFunctionToIntegrate(lengthOfTimeStepInSeconds_, previousValue_ + lengthOfTimeStepInSeconds_ * k3);
    vector3 nextValue = previousValue_ + lengthOfTimeStepInSeconds_ * (1.0/6.0 * (k1 + 2*(k2 + k3) + k4));
    return nextValue;
};

vector3 NumericIntegrator::integrateOneTimeStepDormandPrince(double lengthOfTimeStepInSeconds_, vector3 previousValue_) {
    return 0;
};

vector3 NumericIntegrator::integrateOneTimeStepRungeKuttaNStepped(double lengthOfTimeStepInSeconds_, vector3 previousValue_, int N) {
    vector3 sumOfBjKj = {.x = 0.0, .y = 0.0, .z = 0.0};
    /*for (int j = 1; j < N; j++) {
        double Bj =
        double Kj = previousValue_ + (lengthOfTimeStepInSeconds_ * sumOfAjl_Kl);
        sumOfBj_Kj += (Bj * Kj)
    } */
    vector3 nextValue = previousValue_;
    return nextValue = previousValue_; //+ (lengthOfTimeStepInSeconds_ * sumOfBj_Kj);
};

// function pointer
void NumericIntegrator::setFunctionToIntegrate(Cube::FPTR val_) {
    functionToIntegrate = val_;
};

void NumericIntegrator::setObjectOfFunctionToIntegrate(Cube *val_) {
    objectOfFunctionToIntegrate = val_;
};

vector3 NumericIntegrator::callFunctionToIntegrate(double param1_, vector3 param2_) {
    vector3 temp = (*objectOfFunctionToIntegrate.*functionToIntegrate)(param1_,param2_);
    return temp;
};


// value Map

int NumericIntegrator::writeValueMapToCSV(string path) {
    if (values.empty()) {
        return 0;
    } else {
        ofstream ofStr(path);
        if(ofStr) {
            for( const auto &p : values) {
                cout << p.first << "," << p.second << endl;
                ofStr << p.first << "," << p.second << endl;
            }
        }
        return 1;
    }
};
