#pragma once

#include "vector.h"
#include <SFML/Graphics.hpp>


class entity{
public:
	entity(float size, double weight, const sf::Color &col = sf::Color::White);
	void update(double delta_t);
	void draw(sf::RenderWindow &hwnd);
	void entity::move(){ body.setPosition(r.x, r.y); } //???
	/*void setV(double x_, double y_);
	void setV(double r_, double theta, char unit);
	void setA(double x_, double y_);
	void setA(double r_, double theta, char unit);
	void set(double x_, double y_);
	void setV(double r_, double theta, char unit);*/
	float getRadius(){ return radius; }
	double getMass(){ return mass; }
	double distance_to(const entity &other);
	double angle_to(const entity &other);
	void merge(const entity &other);

//private:
	Vector r;	//position
	Vector v;	//velocity
	Vector a;	//acceleration
	Vector net_force;
private:
	float radius;
	double mass;
	bool elastic;
	sf::CircleShape body;
	sf::Color hue;
	
	void update_r(double delta_t);
	void update_v(double delta_t);
	void update_a();
	
	//net_force Vector will need to be updated differently
};