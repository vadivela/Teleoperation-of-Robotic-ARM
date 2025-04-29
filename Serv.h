#ifndef Serv_h
#define Serv_h

#include<Arduino.h>
#include<Servo.h>

class Serv {
public:
int angle, mini, maxi, hyst; //variables to store the current angle, minimum limit, maximum limit and hysteresis constant for the serv object
Servo s; //The servo object corresponding to the serv object
Serv(); //default constructor
Serv(Servo s1,int initangle,int minlim, int maxlim, int hyster=0); //Parameterised constructor
pos(int ang, int del); //Function to handle the request for writing servo to a particular angle
};

#endif