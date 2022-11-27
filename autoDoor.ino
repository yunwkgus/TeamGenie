#include <SoftwareSerial.h>
#include<Servo.h>
SoftwareSerial btSerial(2,3);
extern volatile unsigned long timer0_millis;
const unsigned int servor_motor_pin =12;
int sensor =4;
int sensor2 =5;
int detect = 0;
int start_time = 0;

int ledPin = 13;
unsigned long current_time;
Servo servo;
void setup(){
  
  Serial.begin(9600);
  btSerial.begin(9600);
  pinMode(sensor,INPUT);
  servo.attach(servor_motor_pin);
  servo.write(90);
  pinMode(ledPin, OUTPUT);
}
void loop(){
  
  
  current_time = millis()/1000; 

  Serial.println(current_time);
  delay(1000);
  if(Serial.available()){
    btSerial.write(Serial.read());
  }
  
  if(btSerial.available()){
    Serial.write(btSerial.read());
  }

  if(digitalRead(sensor) ==LOW){
    
    
    digitalWrite(ledPin, HIGH);
    servo.write(180);
    delay(4000);
    servo.write(90);
    Serial.print(detect);
    Serial.print("\n");
    btSerial.print(detect);
     btSerial.print("\n");
     delay(1000);
     noInterrupts();
  timer0_millis = 0;
    interrupts();
     detect++;
     
    }

    if(digitalRead(sensor2) ==LOW){
    detect++;
    servo.write(0);
    delay(4000);
    digitalWrite(ledPin, LOW);
    servo.write(90);
    Serial.print(detect);
    Serial.print("\n");
    btSerial.print(detect);
     btSerial.print("\n");
     delay(1000);
     noInterrupts();
  timer0_millis = 0;
    interrupts();
     
    }
  
   if (current_time> 15 && detect > 0){
      btSerial.print("a");
      Serial.print("a");
      
      detect=0;
    }
  }


   
