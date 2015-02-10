#ifndef PROJECTILE_H
#define PROJECTILE_H
#include <math.h>

using namespace std;

class Projectile
{
    public:
        Projectile();               //I don't know what these are
        virtual ~Projectile();      //I don't know what these are

        double x;               double y;
        double velocity;        double xVel;        double yVel;        double thetaVel;
        double acceleration;    double xAccel;      double yAccel;      double thetaAccel;
        double force_applied;   double thetaForce_applied;
        unsigned long long int mass;
        double density;
        double volume;

        double Momentum;            //With pointers
        double KineticEnergy;       //With pointers

        double radius;
        int Hue;

        //void Momentum_Calc(double &Momentum, unsigned long long mass, double velocity);
        //void KineticEnergy_Calc(double &KineticEnergy, unsigned long long mass, double velocity);
        void Resolve_Vel_Vector(double velocity, double &xVel, double &yVel, double thetaVel){
            xVel = velocity * cos(thetaVel);
            yVel = velocity * sin(thetaVel);
        }
        void Resolve_Accel_Vector(double acceleration, double &xAccel, double &yAccel, double thetaAccel){
            xAccel = acceleration * cos(thetaAccel);
            yAccel = acceleration * sin(thetaAccel);
        }




    protected:
    private:
};

#endif // PROJECTILE_H
