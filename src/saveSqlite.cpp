#include <stdio.h>
#include <iostream>
#include <string.h>
#include <sqlite3.h>
#include <sstream>
#include "../include/saveSqlite.hpp"
#include "../include/changes.hpp"

using namespace std;

sqlite3* openDataBase(string path_) {
	sqlite3 *db;
	int rc;
	rc = sqlite3_open(path_.c_str(), &db);

	if( rc ){
		cout << "Can't open database: %s\n" << sqlite3_errmsg(db);
	}else{
		cout << "Opened database successfully\n";
	}

	return db;
}

void closeDataBase(sqlite3 *db_) {
	sqlite3_close(db_);
}

void writeToDataBase(sqlite3 *db_, vector3 value_, coords c_) {
	char *zErrMsg = 0;
	const char* data = "Callback function called";
	int rc = 0;
	char* sql;
	sqlite3_stmt *stmt;

	int x = c_.x;
	int y = c_.y;
	double valueX = value_.x;
	double valueY = value_.y;
	double valueZ = value_.z;

	sqlite3_prepare(db_, "DELETE FROM windData WHERE xPOS=? AND yPOS=?",-1,&stmt,0);
	sqlite3_bind_int(stmt,1,c_.x);
	sqlite3_bind_int(stmt,2,c_.y);
	rc = sqlite3_step(stmt);

	if( rc != SQLITE_OK && rc != SQLITE_DONE){
		cout <<  "SQL-Delete error: " << rc << "\n";
		sqlite3_free(zErrMsg);
	}else{
		sqlite3_reset(stmt);
		sqlite3_prepare(db_, "INSERT INTO windData (xPos,yPos,xVal,yVal,zVal) VALUES (?,?,?,?,?)",-1,&stmt,0);
		sqlite3_bind_int(stmt,1,c_.x);
		sqlite3_bind_int(stmt,2,c_.y);
		sqlite3_bind_double(stmt,3,value_.x);
		sqlite3_bind_double(stmt,4,value_.y);
		sqlite3_bind_double(stmt,5,value_.z);

		rc = sqlite3_step(stmt);

		if( rc != SQLITE_OK && rc != SQLITE_DONE){
			cout <<  "SQL-Insert error: " << rc << "\n";
			sqlite3_free(zErrMsg);
		}else{
			cout << "writeToDataBase successfully\n";
		}
	}
}

void writeToDataBaseFast(sqlite3 *db_, vector<vector <Cube>> Cubes_) {
    if (Cubes_.size() > 0) {

        char *zErrMsg = 0;
        const char* data = "Callback function called";
        int rc = 0;
        char* sql;
        sqlite3_stmt *stmt;

        sqlite3_prepare(db_, "DELETE FROM windData",-1,&stmt,0);
        rc = sqlite3_step(stmt);

        stringstream valuesStringStream;

        valuesStringStream << "INSERT INTO windData (xPos,yPos,xVal,yVal,zVal) VALUES ";
        for (int y = 0; y < Cubes_.size(); y++) {
            for (int x = 0; x < Cubes_[y].size(); x++) {
                coords tempCoordsInArea = Cubes_[y][x].getCoordsInArea();
                vector3 tempSpeed = Cubes_[y][x].getSpeed();
                valuesStringStream << "(" << tempCoordsInArea.x << "," << tempCoordsInArea.y;
                valuesStringStream << "," << tempSpeed.x << "," << tempSpeed.y << "," << tempSpeed.z << ")";
                if (!( (y == Cubes_.size() - 1) && (x == Cubes_[y].size() - 1) )) {
                    valuesStringStream << ",";
                }
            }
        }
        //cout << valuesStringStream.str();

        sqlite3_reset(stmt);
        sqlite3_prepare(db_, valuesStringStream.str().c_str(),-1,&stmt,0);

        rc = sqlite3_step(stmt);

        if( rc != SQLITE_OK && rc != SQLITE_DONE){
            cout <<  "SQL-Insert error: " << rc << "\n";
            sqlite3_free(zErrMsg);
        }else{
            cout << "writeToDataBase successfully\n";
        }
    }
}


static int readParametersFromDataBaseCallback(void *data, int argc, char **argv, char **azColName){
	int CubesCountsWidth=-1,CubesCountsHeight=-1;
	double widthArea=-1.0,
		heightArea=-1.0,
		lengthArea=-1.0,
		lengthGeoCoords=-1.0,
		widthGeoCoords=-1.0,
		heightGeoCoords=-1.0;
    stringstream convertSS;

	for(int i=0; i<argc; i++){
	    convertSS.str( std::string() );  // clear
        convertSS.clear();               // -||-
		if(strcmp(azColName[i],"CubesCountsWidth")==0){
			convertSS << argv[i];
			convertSS >> CubesCountsWidth;
		}else if(strcmp(azColName[i],"CubesCountsHeight")==0){
			convertSS << argv[i];
			convertSS >> CubesCountsHeight;
		}else if(strcmp(azColName[i],"widthArea")==0){
			convertSS << argv[i];
			convertSS >> widthArea;
		}else if(strcmp(azColName[i],"heightArea")==0){
			convertSS << argv[i];
			convertSS >> heightArea;
		}else if(strcmp(azColName[i],"lengthArea")==0){
			convertSS << argv[i];
			convertSS >> lengthArea;
		}else if(strcmp(azColName[i],"lengthGeoCoords")==0){
			convertSS << argv[i];
			convertSS >> lengthGeoCoords;
		}else if(strcmp(azColName[i],"widthGeoCoords")==0){
			convertSS << argv[i];
			convertSS >> widthGeoCoords;
		}else if(strcmp(azColName[i],"heightGeoCoords")==0){
			convertSS << argv[i];
			convertSS >> heightGeoCoords;
		}
	}
	setParameters(CubesCountsWidth,CubesCountsHeight,widthArea,heightArea,lengthArea,lengthGeoCoords,widthGeoCoords,heightGeoCoords);
	return 0;
}
static int readTemperatureChangesFromDataBaseCallback(void *data, int argc, char **argv, char **azColName){
	int x=-1,y=-1;
	double delta=-1.0;
    stringstream convertSS;
	for(int i=0; i<argc; i++){
	    convertSS.str( std::string() );  // clear
        convertSS.clear();               // -||-
		if(strcmp(azColName[i],"xPos")==0){
			convertSS << argv[i];
			convertSS >> x;
		}else if(strcmp(azColName[i],"yPos")==0){
			convertSS << argv[i];
			convertSS >> y;
		}else if(strcmp(azColName[i],"delta")==0){
			convertSS << argv[i];
			convertSS >> delta;
		}
	}
	addTemperatureDelta({.x = x, .y = y},delta);
	return 0;
}

void readFromDataBase(sqlite3 *db_,char* sql,int (*pFunc)(void *, int, char **, char **)) {
	char *zErrMsg = 0;
	const char* data = "Callback function called";
	int rc = 0;

	rc = sqlite3_exec(db_, sql, pFunc, (void*)data, &zErrMsg);

	if( rc != SQLITE_OK ){
		 cout <<  "SQL error: " << rc << "\n";
		 sqlite3_free(zErrMsg);
	}else{
		 cout << "readFromDataBase successfully\n";
	}
}

void readParametersFromDataBase(sqlite3 *db_) {
	readFromDataBase(db_,"SELECT * FROM params;",readParametersFromDataBaseCallback);
}

void readTemperatureChangesFromDataBase(sqlite3 *db_) {
	readFromDataBase(db_,"SELECT * FROM tempChangingData;",readTemperatureChangesFromDataBaseCallback);
}
