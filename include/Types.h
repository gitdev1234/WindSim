/**
 * Types.h
 * Purpose: defines structs, constants and types
 *
 * @author Wall.Of.Death
 * @version 1.0
 * @since 20151120
*/

#ifndef TYPES_H_INCLUDED
#define TYPES_H_INCLUDED

#include <math.h>
#include "vector3.h"


// needed for Area::GetTimeMs64()
#ifdef _WIN32
#include <Windows.h>
#else
#include <sys/time.h>
#include <ctime>
#endif
typedef long long int64; typedef unsigned long long uint64;

// const:
const bool SHOW_IN_DETAIL = false;
const bool PRINT_PRETTY  = true;
const double MODIFY_TEMPERATURE_DELTA = 0.001;
const double BOLTZMANN_CONST = 1.38 * pow(10,-23); //BOLTZMANN_CONST = 1.38064852 * pow(10,-23);
const double INDIVIDUAL_GAS_CONST = 287.05; // [J / (Kg * K)]
const double VISCOSITY_AIR = 13.21 * pow (10,-6); // [m^2 / s] is constant for pressure 0.1 to 10 Bar
const double THERMAL_CONDUCTIVITY_COEFFICIENT_AIR = 0.0262;  // lambda
const double SPECIFIC_THERMAL_CAPACITY_AIR = 1.005; // c_v
const double TODO_LIMITOR_FACTOR = 1; // TODO
const int PRINT_PRECISION = 10; // decimal places that are printed in cout

enum programMode {START, ABORT, STAY_IN_LOOP, LOAD_STANDARD, CONFIGURE_AREA, LOAD_AREA_TEMPLATE, MODIFY_VAL, PRINT_AREA, SIMULATION,  STOP};

enum surfaceRoughnessType {WATER, MEADOW, MEADOW_WITH_MANY_HEDGES, PARK_LANDSCAPE, DENSELY_BUILT_UP_AREA, SKYSCRAPER_CITY};
const double HELLMANN_EXPONENT_WATER                   = 0.12;
const double HELLMANN_EXPONENT_MEADOW                  = 0.16;
const double HELLMANN_EXPONENT_MEADOW_WITH_MANY_HEDGES = 0.16;
const double HELLMANN_EXPONENT_PARK_LANDSCAPE          = 0.27;
const double HELLMANN_EXPONENT_DENSELY_BUILT_UP_AREA   = 0.28;
const double HELLMANN_EXPONENT_SKYSCRAPER_CITY         = 0.37;

struct GeoCoords {
    double geoHeight, geoLength, geoWidth;
};

struct coords {
    int x, y;
};

struct airDelta {
    coords newCoords;
    double moleculesCount;
    double temperature;
};

struct temperatureDelta {
    coords newCoords;
    double temperatureDelta;
};


#endif // TYPES_H_INCLUDED
