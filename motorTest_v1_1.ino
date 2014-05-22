//#include <millis.h>
#include <stdlib.h>
#include <TimerOne.h>
#include <string.h>

char array[24];
char array2[12];
int motorLPinA = 2;
int motorLPinB = 3;
int deadmanPin=7;
int dirsek1 = 0;
int dirsekZero = 0;
int motorpwm = 8;
double encoderlPos = 0;
double encoder1Pos1 = 0;
int motorStopPin = 11;
int count = 0;
double zeroPoint=0.0;
double epsilon = 1.8;
int i=0;
int total=0;
int swingUpTork=20;
int balanceTork=0;
int balanced=0;
int positionl=0;
int position2=0;
int rotate=0;
int deadmanVal=0;
//radyan olarak okunacak (q[2])
double readAngleFromEnc[2];
double prevReadAngleFromEnc[2];

//hesaplanan acisal hiz( Radyan/saniye ) (w[2])
double calculatedAngle[2];
double prevCalculatedAngle[2];

// acisal ivme matrisi. bunu ayri yazma sebebim sistem icerisinde hesaplanmasi a[2]
double accelerationMatrix[2];

double diffTime = 0.1;

/* 
 readAngleFromEnc[1] = 0;
 calculatedAngle[0] = 0;
 calculatedAngle[1] = 0;
 accelerationMatrix[0] = 0;
 accelerationMatrix[1] = 0;*/

void setup() {

  Serial.begin(9600);
  Timer1.initialize(70000);
  Timer1.attachInterrupt(sendFirstEncoderAngle); // blinkLED to run every 0.15 seconds
  pinMode(motorLPinA, INPUT);
  pinMode(motorLPinB, INPUT);
  pinMode(deadmanPin,INPUT);
  pinMode(motorStopPin,OUTPUT);
  pinMode(motorpwm,OUTPUT);
  pinMode(21,INPUT);
  pinMode(20,INPUT);
  interrupts();
  attachInterrupt(0, encoderlsvc,RISING);
  attachInterrupt(2,encoderDirsek,RISING);
  
  digitalWrite(motorStopPin,HIGH);
       
}
void loop()
{
    
   char command[5]=" ";
   do{
       
      Serial.readBytes(command,1);   
   
   }while(strcmp(command,"w"));

    if(!strcmp(command,"w")){
  
        Serial.write("fil ");
        
        while(1){ 
          
             deadmanVal=digitalRead(deadmanPin); 
             while(deadmanVal!=0){ 
               
                  while(count<50){
                    analogWrite(motorpwm,count++);
                    delay(50);
                  }
                  while(count>=0){
                    analogWrite(motorpwm,count--);
                    delay(50);
                  }  
                  count=0;
                  changeRotate();
                  rotate++;
                  deadmanVal=digitalRead(deadmanPin);
           } 
        }
    }
      
    
} 
void positionControl(){
  if(positionl==180)
    balanced=1;
  else 
    balanced=0;
}
void swingUp(){
  analogWrite(motorpwm,swingUpTork);
  delay(10);
}
void toBalanced(int tork,int delayT){
  analogWrite(motorpwm,tork);
  delay(delayT);
}
void sendFirstEncoderAngle(){
  
  positionl = ( ( ((int) (encoderlPos/15)) % 360 ) )  ;
  if(positionl<0){
    positionl=positionl+360;
  }
  
  strcpy(array,itoa(positionl,array,10));
  strcat(array,"-");
  
  position2 = ( ( ((int) (encoder1Pos1)) % 360 ) )  ;
  if(position2<0){
    position2=position2+360;
  }
  
  strcpy(array2,itoa(position2,array2,10));
  strcat(array2," ");
  strcat(array,array2);
  Serial.write(array);
}

void timeStopEngine(double time){
  
    analogWrite(motorpwm,0);
    delay(time*1000);  
}
void encoderDirsek(){
  
  int valA=digitalRead(21);
  int valB=digitalRead(20);
  
  if(valA!=valB)
    encoder1Pos1+=1;
  else
    encoder1Pos1-=1;
}
void encoderlsvc()
{
  if((rotate%2)==0)
    encoderlPos+=3.6;
  else
    encoderlPos-=3.6;
}
void changeRotate(){ 
  digitalWrite(motorStopPin,LOW);
  delay(5);
  digitalWrite(motorStopPin,HIGH);
  delay(5);
}
