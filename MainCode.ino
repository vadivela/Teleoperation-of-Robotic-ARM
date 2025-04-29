//Initialising Servo objects
#include<Servo.h>
#include<Serv.h>
Servo ss[4];
Serv s[4];
int ll[4]={0,0,0,0};
int ul[4]={180,180,180,45};
int hysteris[4]={5,5,5,5}; //to reduce stuttering when servo reaches angle limits

//pins and values for potentiometer mapping and indicator LEDs
int pot[4]={A0,A1,A2,A3};
int map0[4]={0,0,0,0}, map1[4]={180,180,180,45};
int pl[4]={0,244,193,693},pu[4]={550,930,835,0},led[4]={46,48,50,52};

//Initialising keypad
#include<Keypad.h>
const byte ROWS = 4;
const byte COLS = 4;
byte rowPins[ROWS] = {10,11,12,13}; 
byte colPins[COLS] = {6,7,8,9};
char hexaKeys[4][4] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};
Keypad customKeypad = Keypad( makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS); 


//EEPROM
#include<EEPROM.h>
//reference state
int ref[4];


void setup() {
  Serial.begin(9600); 
  // loading reference state
  for(int i=0;i<4;i++){ref[i]=EEPROM.read(50+i);}

  //Initialising Serv objects
  for(int i=0;i<4;i++){pinMode(2+i,OUTPUT); pinMode(led[i],OUTPUT); digitalWrite(led[i],LOW); ss[i].attach(2+i); s[i]=Serv(ss[i],ref[i],ll[i],ul[i],hysteris[i]);}

}


void loop() {

char k=padpoll(); //listening for keypad input
if(k!=' '){fromhome(k);}  //calling corresponding functions when key is pressed
mirror(); //default mirror mode
}


/*POLLING FOR KEYPAD INPUT*/
char padpoll(){
  char k = customKeypad.getKey();
  if(k){
    return k; //returns the value of key pressed
  }
  else{return ' ';} // returns ' ' when no key is pressed 
}


/*FUNCTION TO MIRROR THE PUPPET*/
void mirror(){
for(int i=0;i<4;i++){s[i].pos(readpot(i),2); //Writing servos according to the mapped potentiometer readings
if((s[i].angle==ll[i])||(s[i].angle==ul[i])){digitalWrite(led[i],HIGH);}else{digitalWrite(led[i],LOW);} //Lighting up the LED when servo's angle limit is reached, else turning it off
}
}


/* FUNCTION TO READ POTENTIOMETER VALUES AND FILTER FOR A ROBUST ANGLE INPUT */
int readpot(int p){
int readings[10], dev[10], avg=0, avgdev=0, val=0,count=0;
for(int i=0;i<10;i++){  //taking 10 readings to reject noisy readings
  readings[i]=analogRead(pot[p]);
  avg+=readings[i];
  delayMicroseconds(100); 
}
avg/=10;
//outlier rejection
for(int i=0;i<10;i++){
 dev[i] = abs(avg-readings[i]);
 avgdev += dev[i]; 
}
avgdev/=10;
for(int i=0;i<10;i++){
if(dev[i]<=avgdev){val+=readings[i]; count+=1;}
}
val/=count;
val=map(val,pl[p],pu[p],map0[p],map1[p]); //maps potentiometer reading to physical angle
return  val; //returns the mapped robust estimate of potentiometer angle
}

/*FUNCTION TO HANDLE KEYPAD INPUT WHILE IN MIRROR MODE*/
void fromhome(char k){
switch(k){
  case 'A': states(); break; //calling function to display the list of saved state numbers
  case 'B': edit(); break;   //calling function to edit saved states or save new states
  case 'C': cycle(); break;  //calling function to cycle through saved states
  case 'D': del(); break;    //calling function to delete a saved state 
  case '*': break;            
  case '#': refstate(); break;//calling  function to move the robot to a saved reference state  
  default: discrete(String(k).toInt()); break; //calling function to move the robot to a saved state if it exists
}
return;
}


/*FUNCTION TO DISPLAY SAVED STATES*/
void states(){
  Serial.println("Saved states: ");
  for(int i=0; i<=9;i++){if(EEPROM.read(5*i)!=255){Serial.print(i);Serial.print(" ");}}
  Serial.println(" ");
}


/*FUNCTION TO EDIT STATES*/
void edit(){
  Serial.println("Enter state to edit");
  while(true){
    delayMicroseconds(100);
    //listening for keypad input
    char c = padpoll();
    if((c==' ') || (c=='A') || (c=='B') || (c=='C') || (c=='D') || (c=='#') ){} //when number or cancel button is not pressed
    else{
      if(c=='*'){return;} //when cancel button  is pressed
      else{ //when number is pressed
        int st = String(c).toInt();
        Serial.println(c);
        if(EEPROM.read(5*st)!=255){//when state exists
          Serial.println("State exists. Overwrite?");
          bool notchosen=true,EDIT=true;
          while(notchosen){ //waiting for confirmation
            if(padpoll()=='B'){
              notchosen=false;
            }else if(padpoll()=='*'){return;}
          }
        }
        //When state save or edit is confirmed
        Serial.println("Mirroring Puppet...");
        bool notedited=true;
        while(notedited){ //mirroring puppet and waiting for save button press
          delayMicroseconds(100);
          mirror();
          char k=padpoll();
          if(k=='*'){ return;} //when cancel button is pressed
          else if(k=='B'){     //when save button is pressed
            for(int i=0;i<=3;i++){EEPROM.write(5*st+i+1,s[i].angle);} //writing the measured angles to EEPROM
            EEPROM.write(st*5,0);
           Serial.print("State ");
           Serial.print(st);
           Serial.println(" recorded");
            return;      
          }
        }
      }
    }
  }
 }


/*FUNCTION TO CYCLE THROUGH THE SAVED STATES*/
void cycle(){
  Serial.println("Cycling through: ");
  int statestate[10];   
  for(int i=0;i<=9;i++){statestate[i]=EEPROM.read(5*i);}
  for(int i=0;i<=9;i++){if(statestate[i]!=255){Serial.print(i); Serial.print(" ");}} 
  Serial.println(" ");
  int state=0;
  while(true){ //listening to 
    delayMicroseconds(100);
    if(statestate[state]!=255){
      for(int i=0;i<4;i++){s[i].pos(EEPROM.read(5*state+i+1),200);}
      if(customdelay()){return;}; //waits for 2s while listening to keypad input for cancel
    }
    state=(state+1)%10;
  }
}

/*FUNCTION TO DELETE STATE*/
void del(){
  delstep:
  Serial.println("Enter state to delete ");
  while(true){
    char c=padpoll();
    delay(2);
    if((c==' ') || (c=='A') || (c=='B') || (c=='C') || (c=='D') || (c=='#') ){} //when number or cancel is not pressed
    else{
      if(c=='*'){return;} //when cancel is pressed
      else{ // number is pressed
        int st = String(c).toInt();
        Serial.println(c);
        if(EEPROM.read(5*st)==255){Serial.print("State "); Serial.print(st); Serial.println(" is empty"); goto delstep;} //when a state that has not been recorded is pressed
        else{
          Serial.println("Confirm deletion?");
          while(true){ //waiting for deletion confirmation
            char k = padpoll();
            delay(2);
            if(k=='*'){goto delstep; } 
            else if(k=='D'){
              EEPROM.write(5*st,255);
              for(int i=1;i<=4;i++){EEPROM.write(5*st+i,0);}
              Serial.print("State ");
              Serial.print(st);
              Serial.println(" deleted");
              return;
            }
          }
        } 
      }
    }
  }
}


/*FUNCTION TO BRING ROBOT TO REFERENCE STATE*/
void refstate(){
  for(int i=0;i<4;i++){s[i].pos(ref[i],10);}
  Serial.println("In reference state");
  while(true){ //In reference state, waiting for cancel button press
    char c=padpoll();
    delayMicroseconds(100);
    if(c=='*'){
      return;
    }
  }
}


/*FUNCTION TO ACCESS DISCRETE STATE*/
void discrete(int st){
if(EEPROM.read(5*st)==255){return;} //when the state doesn't exist
for(int i=0;i<4;i++){s[i].pos(EEPROM.read(5*st+1+i),50);} //positioning the manipulator in saved position
Serial.print("State ");
Serial.println(st);
while(true){ //listening for cancel button press
  char c=padpoll();
    delayMicroseconds(500);
    if(c=='*'){
      return;
    }
}
}

/*FUNCTION FOR WAITING WHILE LISTENING TO KEYPAD INPUT*/
bool customdelay(){
  int d=0;
  while(d<=2000){char kp = padpoll();
    delay(1);
    if(kp == '*'){return true;} //cancel is pressed
    d++;
  }
  return false; //cancel is not pressed
}
