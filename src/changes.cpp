#include <stdio.h>
#include <iostream>
#include <sqlite3.h>

#include "changes.hpp"
#include "Types.h"

using namespace std;


void addTemperatureDelta(coords c_,double delta){
	cout << "addTempDelta (x: " << c_.x << ",y: " << c_.y<< ") value: " << delta << endl;
}
void setParameters(int CubesCountsWidth, int CubesCountsHeight,double widthArea, double heightArea,double lengthArea,double lengthGeoCoords,double widthGeoCoords,double heightGeoCoords){
	cout << "CubesCountsWidth: " << CubesCountsWidth<< endl;
	cout << "CubesCountsHeight: " << CubesCountsHeight<< endl;
	cout << "widthArea: " << widthArea<< endl;
	cout << "heightArea: " << heightArea<< endl;
	cout << "lengthArea: " << lengthArea<< endl;
	cout << "lengthGeoCoords: " << lengthGeoCoords<< endl;
	cout << "widthGeoCoords: " << widthGeoCoords<< endl;
	cout << "heightGeoCoords: " << heightGeoCoords<< endl;
}
