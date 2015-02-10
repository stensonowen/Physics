// SFML_test_2.cpp : Defines the entry point for the console application.
//

//#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <stdlib.h>
#include <time.h>
#include "entity.h"
//G defined in vector.h for aesthetic and impulsive purposes
using namespace std;

int main(){
	int w = 640;
	int h = 480;
	sf::RenderWindow window(sf::VideoMode(w, h), "Phys Test 1");
	//sf::CircleShape c1(50);
	//c1.setPosition(-50, -50);
	int fps = 60;
	//window.setFramerateLimit(20);
	window.setFramerateLimit(fps);
	//srand(5);
	srand(time(NULL));

	entity ball(15, 1);
	ball.r.x = 320;
	ball.r.y = 240;

	ball.v.y = -50;
	ball.net_force.y = 9.806;
	
	
	//there should be a Vector of entities
	//each frame should cycle through each entity to calculate net force,
	// which should be found by calculating distance from each other object
	//O(n^2)
	std::vector<entity> objects;
	
	//Generate random objects
	int n = rand() % 10 + 10;
	for (int i = 0; i < n; i++){
		int rad	 = rand() % 30 + 10;		//10-40
		//int mass = rand() % 9 + 1;			//1-10
		//int mass = rad;
		//double mass = rand() % 1000000000000;//10e+12
		int p = rand() % 5 + 9;
		double mass = rad * pow(10, p);	// 10*10^9 - 40*10^13
		int r = rand() % 256;
		int g = rand() % 256;
		int b = rand() % 256;
		entity object = entity(rad, mass, sf::Color(r, g, b));
		object.r.x = rand() % (w - 2 * rad - 2) + rad + 1;	//radius to width - radius
		object.r.y = rand() % (h - 2 * rad - 2) + rad + 1;	//radius to height - radius
		//object.v.x = rand() % w/10;
		//object.v.y = rand() % h/10;
		//object.net_force.x = rand() % w/100;
		//object.net_force.y = rand() % h/100;
		
		//object.net_force.y = 9.806;
		objects.push_back(object);
	}

	//objects.push_back(ball);
	//objects.push_back(entity(5, 1, sf::Color::Blue));
	//objects.push_back(entity(40, 1e+15));
	//objects[0].r = Vector(300, 300);
	//objects[1].r = Vector(300, 400);

	int frames = 0;
	sf::Time elapsed;
	sf::Clock timer;
	
	while (window.isOpen()){
		sf::Event event;
		while (window.pollEvent(event)){
			if (event.type == sf::Event::Closed) window.close();
		}
		window.clear();	//Don't change
		
		//get the elapsed time and restart the timer;
		elapsed = timer.getElapsedTime();
		timer.restart();
		float delta_t = elapsed.asSeconds();
		frames++;	//To Be Deprecated
	
		//Draw shit here:
		//ball.update(delta_t);
		//ball.draw(window);
		for (int i = 0; i < objects.size(); i++){
			objects[i].update(delta_t);

			//cout << i << ":\t" << objects[i].net_force.r << endl;
			//if(i == 0) cout << objects[i].r.r << endl;

			//reflect objects from edges
			if (w - objects[i].r.x <= objects[i].getRadius() || objects[i].r.x <= objects[i].getRadius()) objects[i].v.x *= -1;
			if (h - objects[i].r.y <= objects[i].getRadius() || objects[i].r.y <= objects[i].getRadius()) objects[i].v.y *= -1;

			//Net force should be sum of forces, which is currently zero
			objects[i].net_force.clear();
			
			//object should interact with all other objects
			//would it make sense to define pointers to i and j that might speed up references? Pointers are slow, but indexing so much seems like a lot
			for (int j = i+1; j < objects.size(); j++){
				//object should not interact with itself
				if (i == j) continue;
				//if (i >= objects.size() || j >= objects.size()) break;

				//if objects touch, then they merge
				double d = objects[i].distance_to(objects[j]);
				if (d <= (objects[i].getRadius() + objects[j].getRadius())){
					objects[i].merge(objects[j]);
					swap(objects[j], objects.back());
					objects.pop_back();
					//if the last object is removed, then subscript is out of range
					continue; //necessary? IF objects collide, then the force between them is irrelevant

				}

				//The force of each object is related to its distance from another
				//The net force is simply the sum of each force
				// F_G = GmM/r^2
				double force = UGC * objects[i].getMass() * objects[j].getMass() / (d * d);
				//force should be f_g in the direction of the other entity
				//cout << "theta = " << objects[i].angle_to(objects[j]) << endl;
				Vector f(force, objects[i].angle_to(objects[j]), 'r');
				objects[i].net_force += f;
				objects[j].net_force -= f;

				if (i >= objects.size() || j >= objects.size()) break;
			}
			if (i >= objects.size()) break;

			objects[i].draw(window);
		}


		
		
		window.display();	//Don't change
	}
	std::cout << "";

	return 0;
}