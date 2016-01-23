#include <sqlite3.h>
#include "Types.h"

#ifndef SAVESQLITE_H
#define SAVESQLITE_H


sqlite3* openDataBase(string path_ = "cache.db");
void closeDataBase(sqlite3*);
void writeToDataBase(sqlite3*, vector3, coords);

void readParametersFromDataBase(sqlite3*);
void readTemperatureChangesFromDataBase(sqlite3*);

#endif /* SAVESQLITE_H */
