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
const float MODIFY_TEMPERATURE_DELTA = 25.0;

struct GeoCoords {
    float geoHeight, geoLength, geoWidth;
};

struct coords {
    int x, y;
};

struct vector3 {
    float x, y, z;
};


#endif // TYPES_H_INCLUDED
