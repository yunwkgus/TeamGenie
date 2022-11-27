#include <SoftwareSerial.h>
#include<Servo.h>
#include <Stepper.h>
#define STEPS 1024 // 스텝모터의 180도

SoftwareSerial btSerial (2,3); 
extern volatile unsigned long timer0_millis; 

int sensor =4; //
int sensor2 =5; //
int detect 0; // 감지 횟수
int start_time= 0; //
int ledPin 13; // LED
unsigned long current_time;
Stepper myStepper(STEPS, 11, 9, 10, 8);

void setup() {
Serial.begin(9600);
btSerial.begin(9600);
pinMode (sensor, INPUT);
pinMode(ledPin, OUTPUT);
myStepper.setSpeed (30);
}


void loop() {
current_time = millis()/1000;

if (Serial.available()) {
btSerial.write (Serial.read());
}

if (btSerial.available()) {
Serial.write(btSerial.read());
}

if (digitalRead (sensor) == LOW){ //
digitalWrite (ledPin, HIGH);
myStepper.step (STEPS/2);
delay (4000);
myStepper.step (-STEPS/2);
Serial.print (detect);
Serial.print("\n");
btSerial.print (detect);
btSerial.print("\n");
noInterrupts (); //millis
timer0_millis = 0;
interrupts();
detect++;
}


if (digitalRead (sensor2) == LOW){ 
digitalWrite (ledPin, LOW);
myStepper.step (-STEPS/2);
delay (4000);
myStepper.step (STEPS/2);
Serial.print (detect);
Serial.print("\n");
btSerial.print (detect);
btSerial.print("\n");
delay(1000);
noInterrupts (); 
timer0_millis = 0;
interrupts();
detect++;
}

if (current_time> 15 && detect > 0) {
btSerial.print ("a"); 
Serial.print ("a");
detect=0; 
}


