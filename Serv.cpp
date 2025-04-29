#include "Serv.h"
#include "Arduino.h"
#include "Servo.h"

Serv :: Serv(){

}

Serv :: Serv(Servo s1, int initangle, int minlim, int maxlim, int hyster){
this->s = s1;
this->angle = initangle;
this->mini = minlim;
this->maxi = maxlim;
this->hyst = hyster;
this->pos(this->angle,20);
}

Serv :: pos(int ang,int del){
//To constrain the angle within limits
ang = max(ang, this->mini);
ang = min(ang,this->maxi);

//When the servo position is near limits
//Without this hysteresis, excess stuttering was observed when the angle approached close to the limits
if((this->angle == this->mini)||(this->angle == this->maxi)){
if(abs(ang-(this->angle))<=this->hyst){ang = this->angle;}
}

//positioning servo to the bound angle input and saving the servo's current angle
this->s.write(ang);
this->angle = ang;
delay(del);

}
