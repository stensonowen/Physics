#pragma once

#include <math.h>
#include <assert.h>

#define PI 3.14159265
//#define G 6.67384*10**-11
#define UGC 0.0000000000667384

//using namespace std;

class Vector{
public:
	//Constructors
	Vector() : x(0), y(0), theta(0), r(0) {}
	Vector(double x_, double y_) : x(x_), y(y_) {
		calculate_r();
	}
	Vector(double r_, double theta_, char unit) : r(r_), theta(theta_) {
		//if (unit == "degree" || unit == "degrees" || unit == "deg" || unit == "degs" || unit == "d") theta = theta_ * 180 / PI;
		//else assert(unit == "radian" || unit == "radians" || unit == "rad" || unit == "rads" || unit == "r");
		if (unit == 'd') theta = theta_ * 180 / PI;
		else assert(unit == 'r');
		calculate_c();
	}


	//Update values
	void calculate_r(){
		//find the resultant and angle from the components
		r = sqrt(x*x + y*y);
		theta = atan(y / x);
	}
	void calculate_c(){
		//find the component values from the resultant and angle
		x = r * cos(theta);
		y = r * sin(theta);
	}

	void clear(){
		x = 0;
		y = 0;
		r = 0;
		theta = 0;
	}
	//Should these be called all the time, or just when necessary?
	void set_x(double x_){
		x = x_;
		calculate_r();
	}
	void set_y(double y_){
		y = y_;
		calculate_r();
	}
	void set_cs(double x_, double y_){
		x = x_;
		y = y_;
		calculate_r();
	}
	void set_r(double r_){
		r = r_;
		calculate_c();
	}
	void set_theta(double theta_){
		theta = theta_;
		calculate_c();
	}
	void increment_theta(double theta_){
		theta += theta_;
		calculate_c();
	}
	void decrement_theta(double theta_){
		theta -= theta_;
		calculate_c();
	}
	void set_r(double r_, double theta_, char unit){
		r = r_;
		if (unit == 'd') theta = theta_ * 180 / PI;
		else assert(unit == 'r');
		calculate_c();
	}

	//Variable retrieval shouldn't involve recalculation, because you're unsure of which set is accurate
	/*
	double get_x(){
		calculate_c();
		return x;
	}
	double get_y(){
		calculate_c();
		return y;
	}
	double get_r(){
		calculate_r();
		return r;
	}
	double get_theta(){
		calculate_r();
		return theta;
	}
	*/

	//Overloads
	Vector operator+(const Vector &other){
		double x_sum = this->x + other.x;
		double y_sum = this->y + other.y;
		return Vector(x_sum, y_sum);
	}
	Vector& operator+=(const Vector &other){
		this->x += other.x;
		this->y += other.y;
		this->calculate_r();
		return *this;
	}
	Vector& operator-=(const Vector &other){
		this->x -= other.x;
		this->y -= other.y;
		this->calculate_r();
		return *this;
	}
	Vector operator-(const Vector &other){
		double x_dif = this->x - other.x;
		double y_dif = this->y - other.y;
		return Vector(x_dif, y_dif);
	}
	Vector operator/(double divisor){
		double new_x = this->x / divisor;
		double new_y = this->y / divisor;
		return Vector(new_x, new_y);
	}
	Vector operator*(double multiplier){
		double new_x = this->x * multiplier;
		double new_y = this->y * multiplier;
		return Vector(new_x, new_y);
	}
	//bool operator<(const Vector &other){ return this->r < other.r; }

	double x;
	double y;
	double theta;	//0 radians at horizontal
	double r; //resultant
};

