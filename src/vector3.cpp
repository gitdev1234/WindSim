#include "vector3.h"

//constructor
vector3::vector3(double x_, double y_, double z_) : x(x_), y(y_), z(z_) {
    setPrintPrecision();
};

// copy-constructor
vector3::vector3(const vector3& other_) :  x(other_.x), y(other_.y), z(other_.z), vector3PrintPrecision(other_.getPrintPrecision()) {};

// assignment operator
vector3& vector3::operator=(const vector3& other_) {
    if (this == &other_) {
        return *this;
    } else {
        x = other_.x;
        y = other_.y;
        z = other_.z;
        return *this;
    }
}

// logical operators
bool vector3::operator==(const vector3& other_) const {
    return (this == &other_) ||
           (x == other_.x && y == other_.y && z == other_.z);
}

bool vector3::operator!=(const vector3& other_) const {
    return ! (*this == other_);
}

bool vector3::operator<(const vector3& other_) const {
    return (x < other_.x && y < other_.y && z < other_.z);
}

bool vector3::operator<=(const vector3& other_) const {
    return (x <= other_.x && y <= other_.y && z <= other_.z);
}

bool vector3::operator>(const vector3& other_) const {
    return (x > other_.x && y > other_.y && z > other_.z);
}

bool vector3::operator>=(const vector3& other_) const {
    return (x >= other_.x && y >= other_.y && z >= other_.z);
}

//mathematical operators
vector3 vector3::operator*(const double& other_) const {
    vector3 temp;
    temp.x = x * other_;
    temp.y = y * other_;
    temp.z = z * other_;
    return temp;
}

vector3 vector3::operator*(const vector3& other_) const {
    vector3 temp;
    temp.x = x * other_.x;
    temp.y = y * other_.y;
    temp.z = z * other_.z;
    return temp;
}

vector3 operator*(const double& d_, const vector3& v_){
    return v_ * d_;
};

void vector3::operator*=(const vector3& other_) {
    *this = *this * other_;
}

void vector3::operator*=(const double& other_) {
    *this = *this * other_;
}

vector3 vector3::operator/(const double& other_) const {
    vector3 temp;
    temp.x = x / other_;
    temp.y = y / other_;
    temp.z = z / other_;
    return temp;
}

vector3 vector3::operator/(const vector3& other_) const {
    vector3 temp;
    temp.x = x / other_.x;
    temp.y = y / other_.y;
    temp.z = z / other_.z;
    return temp;
}

vector3 operator/(const double& d_, const vector3& v_){
    return v_ / d_;
};

void vector3::operator/=(const vector3& other_) {
    *this = *this / other_;
}

void vector3::operator/=(const double& other_) {
    *this = *this / other_;
}

vector3 vector3::operator+(const double& other_) const {
    vector3 temp;
    temp.x = x + other_;
    temp.y = y + other_;
    temp.z = z + other_;
    return temp;
}

vector3 vector3::operator+(const vector3& other_) const {
    vector3 temp;
    temp.x = x + other_.x;
    temp.y = y + other_.y;
    temp.z = z + other_.z;
    return temp;
}

vector3 operator+(const double& d_, const vector3& v_){
    return v_ + d_;
};

void vector3::operator+=(const vector3& other_) {
    *this = *this + other_;
}

void vector3::operator+=(const double& other_) {
    *this = *this + other_;
}

vector3 vector3::operator-(const double& other_) const {
    vector3 temp;
    temp.x = x - other_;
    temp.y = y - other_;
    temp.z = z - other_;
    return temp;
}

vector3 vector3::operator-(const vector3& other_) const {
    vector3 temp;
    temp.x = x - other_.x;
    temp.y = y - other_.y;
    temp.z = z - other_.z;
    return temp;
}

vector3 operator-(const double& d_, const vector3& v_){
    return v_ - d_;
};

void vector3::operator-=(const double& other_) {
    *this = *this - other_;
}

void vector3::operator-=(const vector3& other_) {
    *this = *this - other_;
}


// print operator
ostream& operator<<(ostream& o_, const vector3& v_) {
    int tempPrintPrecision = v_.getPrintPrecision();
    o_ << setprecision(tempPrintPrecision) << fixed;
    o_ << "[x:" << v_.x << ",y:" << v_.y << "z:" << v_.z << "]";
    return o_;
}

