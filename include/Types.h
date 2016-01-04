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
const double TODO_LIMITOR_FACTOR = 1; //0.001;
const int PRINT_PRECISION = 10; // decimal places that are printed in cout

struct GeoCoords {
    double geoHeight, geoLength, geoWidth;
};

struct coords {
    int x, y;
};

struct vector3 {
    double x, y, z;
};

struct airDelta {
    coords newCoords;
    double moleculesCount;
    double temperature;
};


#endif // TYPES_H_INCLUDED
