#include <sqlite3.h>
#include <vector>
#include <string>
#include "Cube.h"
#include "Types.h"


#ifndef SAVESQLITE_H
#define SAVESQLITE_H

using namespace std;


sqlite3* openDataBase(string path_ = "cache.db");
void closeDataBase(sqlite3* db_);
void writeToDataBase(sqlite3* db_, vector3, coords);
void writeToDataBaseFast(sqlite3* db_, vector<vector <Cube>> Cubes_);

void readParametersFromDataBase(sqlite3* db_);
void readTemperatureChangesFromDataBase(sqlite3* db_);

#endif /* SAVESQLITE_H */
