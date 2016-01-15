#ifndef VECTOR3_H
#define VECTOR3_H

#include <iostream>
#include <sstream>
#include <iomanip>

using namespace std;


class vector3 {
    public :
        double x, y, z;


        // constructor
        vector3(double x_ = 0.0, double y_ = 0.0, double z_ = 0.0);
        // copy-constructor
        vector3(const vector3& other_);
        // destructor
        ~vector3() {} ;

        // assignment operator
        vector3& operator=(const vector3& other_);

        // logical operators
        bool operator==(const vector3& other_) const;
        bool operator!=(const vector3& other_) const;
        bool operator< (const vector3& other_) const;
        bool operator<=(const vector3& other_) const;
        bool operator> (const vector3& other_) const;
        bool operator>=(const vector3& other_) const;

        //mathematical operators
        vector3 operator* (const double&  other_) const;
        vector3 operator* (const vector3& other_) const;
        void    operator*=(const vector3& other_);
        void    operator*=(const double&  other_);
        vector3 operator/ (const double&  other_) const;
        vector3 operator/ (const vector3& other_) const;
        void    operator/=(const vector3& other_);
        void    operator/=(const double&  other_);
        vector3 operator+ (const double&  other_) const;
        vector3 operator+ (const vector3& other_) const;
        void    operator+=(const vector3& other_);
        void    operator+=(const double&  other_);
        vector3 operator- (const double&  other_) const;
        vector3 operator- (const vector3& other_) const;
        void    operator-=(const double&  other_);
        void    operator-=(const vector3& other_);

        // print operator
        friend ostream& operator<<(ostream& o_, const vector3& v_);

        // getter and setter
        void setPrintPrecision(const int decimalPlaces_ = 2) {vector3PrintPrecision = decimalPlaces_;}
        int getPrintPrecision() const {return vector3PrintPrecision;}

    private :
        int vector3PrintPrecision;

};


#endif // VECTOR3_H
