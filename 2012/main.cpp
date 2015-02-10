#include <iostream>
#include <allegro.h>
#include <winalleg.h>
#include <stdio.h>
#include <math.h>
#include "Projectile.h"
#define GREY          makecol(128, 128, 128)
#define WHITE         makecol(255, 255, 255)
#define RED           makecol(255,   0,   0)
#define GREEN         makecol(  0, 255,   0)
#define BLUE          makecol(  0,   0, 255)
#define YELLOW        makecol(255, 255,   0)
#define ORANGE        makecol(255, 128,   0)
#define CYAN          makecol(  0, 255, 255)
#define PURPLE        makecol(255,   0, 255)
#define LIGHT_BLUE    makecol(128, 128, 255)
#define PINK          makecol(255, 128, 128)
#define LIGHT_GREEN   makecol(128, 255, 128)

//Variables:
const double uGc = 6.673e-011;
const double pi = 3.14159265358979;
double Time = 0;
BITMAP* Buffer;
double Space_Scale = 1000; // 1 pixel = .1 kilometer
double Time_Scale = 25;
bool scope = 0;

volatile double elapsed_time = 0;

using namespace std;

//Functions:
void __inc_elapsed_time() {
    elapsed_time += .001;
}
END_OF_FUNCTION(__inc_elapsed_time);


void Refresh_Pos(Projectile &P){
    P.x += P.xVel;
    P.xVel += P.xAccel;
    P.xAccel += P.force_applied / P.mass * cos(P.thetaForce_applied * pi / 180);

    P.y += P.yVel;
    P.yVel += P.yAccel;
    P.yAccel += P.force_applied / P.mass * sin(P.thetaForce_applied * pi / 180);

}

void Check_Collision(Projectile &P1, Projectile &P2, Projectile &PIE){
    int distance = sqrt((P1.x-P2.x)*(P1.x-P2.x) + (P2.y-P1.y)*(P2.y-P1.y));
    if((distance <= (P1.radius + P2.radius)) && (distance >= P1.radius) && (distance >= P2.radius)){}   //Minor Collision
    else if((distance <= P1.radius) ^ (distance <= P2.radius)){}                                        //Intermediate Collision
    else if((distance <= P1.radius) && (distance <= P2.radius)){}                                       //Major Collision
    else if(distance > (P1.radius + P2.radius)){}                                                       //No Collision
    if((distance <= P1.radius) || (distance <= P2.radius)){                                             //Intermediate OR Major collision
        PIE.mass = P1.mass + P2.mass;
        PIE.xVel = P1.xVel + P2.xVel;
        PIE.yVel = P1.yVel + P2.yVel;
        PIE.x = P2.x;//TBChanged
        PIE.y = P2.y;//TBChanged
        PIE.radius = P1.radius + P2.radius;
        PIE.Hue = makecol(
                          ((P1.mass*getr(P1.Hue)+P2.mass*getr(P2.Hue))/PIE.mass),
                          ((P1.mass*getg(P1.Hue)+P2.mass*getg(P2.Hue))/PIE.mass),
                          ((P1.mass*getb(P1.Hue)+P2.mass*getb(P2.Hue))/PIE.mass));
    }
    }

void CalculateTheta(Projectile &P1, Projectile &P2){                                //Relative to P1__Assuming straight-line travel (thetaVel = thetaAccel = thetaForceG
        if((P2.x > P1.x) && (P1.y > P2.y)) P1.thetaForce_applied = (                    atan((P1.y-P2.y)/(P2.x-P1.x))        * 180 / pi);                // QUADRANT ONE
        if((P1.x > P2.x) && (P1.y > P2.y)) P1.thetaForce_applied = ((pi -               atan((P1.y-P2.y)/(P1.x-P2.x)))       * 180 / pi);                // QUADRANT TWO
        if((P1.x > P2.x) && (P2.y > P1.y)) P1.thetaForce_applied = (((3 * pi / 2) -     atan((P1.x-P2.x)/(P2.y-P1.y)))       * 180 / pi);                // QUADRANT THREE
        if((P2.x > P1.x) && (P2.y > P1.y)) P1.thetaForce_applied = (((2 * pi) -         atan((P2.y-P1.y)/(P2.x-P1.x)))       * 180 / pi);                // QUADRANT FOUR
        if((P2.x == P1.x) && (P1.y > P2.y)) P1.thetaForce_applied = pi / 2                                       * 180 / pi;             // Label 90
        if((P2.y == P1.y) && (P1.x > P2.x)) P1.thetaForce_applied = pi                                           * 180 / pi;             // Label 180
        if((P2.x == P1.x) && (P2.y > P1.y)) P1.thetaForce_applied = 3 * pi / 2                                   * 180 / pi;             // Label 270

        P2.thetaForce_applied = P1.thetaForce_applied + pi                       * 180 / pi;
        while(P1.thetaForce_applied >= 360) P1.thetaForce_applied -= 360;
        while(P2.thetaForce_applied >= 360) P2.thetaForce_applied -= 360;
        P1.thetaForce_applied = 360 - P1.thetaForce_applied;        //Because Allegro reverses the x-axis
        P2.thetaForce_applied = 360 - P2.thetaForce_applied;        //Because Allegro reverses the x-axis
}

void Gravity(Projectile &P1, Projectile &P2){
        P1.force_applied = (uGc * P1.mass * P2.mass) / ((P2.x - P1.x)*fabs(P2.x - P1.x) + fabs(P2.y - P1.y)*(P2.y - P1.y));
        P2.force_applied = (uGc * P1.mass * P2.mass) / ((P2.x - P1.x)*fabs(P2.x - P1.x) + fabs(P2.y - P1.y)*(P2.y - P1.y));
}


//main() function
int main()
{
    //Generic allegro initialization
    allegro_init();
    install_keyboard();
    install_timer();
    set_gfx_mode(GFX_AUTODETECT_WINDOWED,640,480,0,0);
    Buffer = create_bitmap(640, 480);

    readkey();

    //Variables
    LOCK_VARIABLE(elapsed_time);
    LOCK_FUNCTION(__inc_elapsed_time);
    install_int_ex(__inc_elapsed_time, BPS_TO_TIMER(1000));


//########################################################################################################################
    Projectile Comet;

    Comet.radius = 4440 / Space_Scale;
    Comet.Hue = PINK;
    Comet.mass = 220000000000000; //previously m=250000
    Comet.force_applied = 0;
    Comet.thetaForce_applied = 0;

    Comet.x = 240;
    Comet.xVel = 0 / Space_Scale / Time_Scale;                //Meters per second, accounting for Time_ and Space_ Scale.
    Comet.xAccel = 0 / Space_Scale / Time_Scale;              //Meters per second per secpnd "    "

    Comet.y = 36;
    Comet.yVel = 0 / Space_Scale / Time_Scale;
    Comet.yAccel = 0 / Space_Scale / Time_Scale;
//########################################################################################################################
    Projectile Deimos;

    Deimos.radius = 6200 / Space_Scale;//111
    Deimos.Hue = RED;                                       //previously makecol(200, 150, 137)
    Deimos.mass = 1480000000000000;                               //previously m=800000; 100000000
    Deimos.force_applied = 0;
    Deimos.thetaForce_applied = 0;

    Deimos.x = 300;//11700
    Deimos.xVel = 0 / Space_Scale / Time_Scale;             //n pixels/sec is proportional to 10n m/s
    Deimos.xAccel = 0 / Space_Scale / Time_Scale;           // Meters per second, once more

    Deimos.y = 242;
    Deimos.yVel = 0 / Space_Scale / Time_Scale;
    Deimos.yAccel = 0 / Space_Scale / Time_Scale;
//########################################################################################################################
    Projectile Combo;

    Combo.radius = 1;//111
    Combo.Hue = WHITE;                                       //previously makecol(200, 150, 137)
    Combo.mass = 1;                               //previously m=800000; 100000000
    Combo.force_applied = 0;
    Combo.thetaForce_applied = 0;

    Combo.x = 1;//11700
    Combo.xVel = 0 / Space_Scale / Time_Scale;             //n pixels/sec is proportional to 10n m/s
    Combo.xAccel = 0 / Space_Scale / Time_Scale;           // Meters per second, once more

    Combo.y = 1;
    Combo.yVel = 0 / Space_Scale / Time_Scale;
    Combo.yAccel = 0 / Space_Scale / Time_Scale;
//########################################################################################################################

    while(!key[KEY_ESC]){
        //Drawing:
        clear_to_color(Buffer,makecol(0, 0, 0));
        textprintf(Buffer, font, 0, 10, WHITE, "%4.2f", elapsed_time);
        circlefill(Buffer, Deimos.x, Deimos.y, Deimos.radius, Deimos.Hue);
        circlefill(Buffer, Comet.x, Comet.y, Comet.radius, Comet.Hue);
        //if(Combo.mass > 0) circlefill(Buffer, Combo.x, Combo.y, Combo.radius, Combo.Hue);

        line(Buffer, Comet.x, 0, Comet.x, 480, WHITE);
        line(Buffer, 0, Comet.y, 640, Comet.y, WHITE);
        line(Buffer, Comet.x, Comet.y, Deimos.x, Deimos.y, WHITE);
        line(Buffer, Deimos.x, 0, Deimos.x, 480, WHITE);
        line(Buffer, 0, Deimos.y, 640, Deimos.y, WHITE);

        //Calling functions:
        Refresh_Pos(Comet);
        Refresh_Pos(Deimos);
        CalculateTheta(Comet, Deimos);
        Gravity(Comet, Deimos);
        //Check_Collision(Comet, Deimos, Combo);

        //couts as needed
        //cout << Comet.thetaForce_applied << "\t\t" << sin(Comet.thetaForce_applied * pi / 180) << "\t\t" << Comet.force_applied << endl;
//        cout << getr(Combo.Hue) << "\t\t" << getg(Combo.Hue) << "\t\t" << getb(Combo.Hue) << endl;

cout << Comet.force_applied << endl;

        //Closing remarks:
        blit(Buffer, screen, 0, 0, 0, 0, Buffer->w, Buffer->h);
        Sleep(1000 / Time_Scale);                           //All other values are increased by a value of 1000/n
        Time += 1/Time_Scale;
    }
    cout << endl << endl << Time << " seconds passed." << endl;

    readkey();

    return 0;
}
END_OF_MAIN()
