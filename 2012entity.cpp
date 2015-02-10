#include "entity.h"
#include <iostream>

//Non-member functions that are used
sf::Color mix1(const sf::Color &c1, const double m1, const sf::Color &c2, const double m2);
sf::Color mix2(const sf::Color &c1, const sf::Color &c2);

entity::entity(float size, double weight, const sf::Color &col){
	radius = size;
	mass = weight;
	elastic = true;
	body.setRadius(radius);
	hue = col;
	body.setFillColor(hue);
}

void entity::update_r(double delta_t){
	r += v * delta_t;
}

void entity::update_v(double delta_t){
	//the new velocity calculated from the acceleration
	//a = delta_v / delta_t = (v_f - v_i) / delta_t;
	//a * delta_t + v_i = v_f
	//v_f += a * delta_t
	v += a * delta_t;
	//should average between new and old acceleration?
}

void entity::update_a(){
	//Find the new net acceleration 
	a = net_force / mass;
}


void entity::update(double delta_t){
	//order of updating?
	update_a();
	update_v(delta_t);
	update_r(delta_t);

	//update_r(delta_t);
	//update_v(delta_t);
	//update_a();
}

void entity::draw(sf::RenderWindow &hwnd){
	//setPosition refers to the top left corner
	body.setPosition(r.x - radius, r.y - radius);
	hwnd.draw(body);
}

double entity::distance_to(const entity &other){
	//double distance(entity* a, entity* b){
	//draw a vector between the two objects, and return its resultant part
	//Vector v(abs(a.r.x - b.r.x), abs(a.r.y - b.r.y));
	//v.calculate_r();
	//return v.r;
	double delta_x = this->r.x - other.r.x;	//no need for abs() because **2
	double delta_y = this->r.y - other.r.y;
	return sqrt(delta_x * delta_x + delta_y * delta_y);
}

double entity::angle_to(const entity &other){
	// //find angle from this to other (i.e. the shortest path to other)
	double delta_x = other.r.x - this->r.x;	//should these values be negative?
	double delta_y = other.r.y - this->r.y;
	return atan2(delta_y, delta_x);
	// sin/cos incorporates direction
}

void entity::merge(const entity &other){
	
	sf::Color col = other.hue;

	//this->hue = mix1(this->hue, this->mass, other.hue, other.mass);
	sf::Color c_i = this->hue;
	this->hue = mix2(c_i, other.hue);
	std::cout << "(" << c_i.r << "," << c_i.g << "," << c_i.b << ") + (" << other.hue.r << "," << other.hue.g << "," << other.hue.b << ") = " << this->hue.r << "," << this->hue.g << "," << this->hue.b << ")" << std::endl;
	// TO BE DEBUGGED

	double volume = 4.0 / 3 * PI * (pow(this->radius, 3) + pow(other.radius, 3));
	std::cout << "Radii of " << this->radius << " and " << other.radius << " make ";
	this->radius = cbrt(.75 * volume / PI);
	std::cout << this->radius << "\t(" << volume << ")" << std::endl;
	body.setRadius(this->radius);
	
	double x = (this->r.x * this->mass + other.r.x * other.mass) / (this->mass + other.mass);
	double y = (this->r.y * this->mass + other.r.y * other.mass) / (this->mass + other.mass);
	this->r.x = x;
	this->r.y = y;

	//	v1*m1 + v2*m2 = (v)(m1+m2)
	//	v = (v1*m1 + v2*m2) / (m1+m2)
	this->v.x = (this->v.x * this->mass + other.v.x * other.mass) / (this->mass + other.mass);
	this->v.y = (this->v.y * this->mass + other.v.y * other.mass) / (this->mass + other.mass);
	this->v.calculate_r();
	this->a.clear();
	this->net_force.clear();
	
	this->mass += other.mass;
}

sf::Color mix1(const sf::Color &c1, const double m1, const sf::Color &c2, const double m2){
	// weighted wavelengths
	double total_mass = m1 + m2;
	unsigned int r = (c1.r * m1 + c2.r * m2) / total_mass;
	unsigned int g = (c1.g * m1 + c2.g * m2) / total_mass;
	unsigned int b = (c1.b * m1 + c2.b * m2) / total_mass;
	return sf::Color(r, g, b);
}

sf::Color mix2(const sf::Color &c1, const sf::Color &c2){
	// unweighted pigments
	int r = .5 * (c1.r + c2.r);
	int g = .5 * (c1.g + c2.g);
	int b = .5 * (c1.b + c2.b);
	return sf::Color(r, g, b);
}