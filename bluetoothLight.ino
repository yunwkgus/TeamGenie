#include <SimpleTimer.h>

#include <SoftwareSerial.h>
#include <virtuabotixRTC.h>
#include <TimeLib.h>

#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
 #include <avr/power.h>
#endif

#define PIN        12 
#define NUMPIXELS 64

const int bluetoothRX = 10; // module 기준
const int bluetoothTX = 8;

Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);
#define DELAYVAL 500
SoftwareSerial bluetooth ( bluetoothTX, bluetoothRX );


SimpleTimer timer;

int LED = 12;
String bleInputString = ""; //블루투스로부터 받는 문자열
String daykey = "day";
String nightkey = "night";
String offkey = "off";
boolean alarm = false;
boolean light = false;

int shut_down = 10000;

char *array[3];
int hour_t;
int minute_t;

#define PIN_CLK 5
#define PIN_DATA 6
#define PIN_RST 7
virtuabotixRTC myRTC(PIN_CLK, PIN_DATA, PIN_RST);


tmElements_t tm;

int _year = 2022,
_month = 2,
_day = 8,
_dayofweek = 1,
_hour = 17,
_min = 44,
_sec = 00;
int seconds_time = _sec;


void setup(){
  //시리얼 시작
  Serial.begin(9600);
  
  //블루투스 시리얼 포트 시작
  bluetooth.begin(9600);

//  myRTC.setDS1302Time(_sec,_min,_hour,_dayofweek,_day,_month,_year);


    // put your setup code here, to run once:
  #if defined(__AVR_ATtiny85__) && (F_CPU == 16000000)
    clock_prescale_set(clock_div_1);
  
  #endif
    pixels.begin();
  //LED핀 설정f
//  pinMode(LED,OUTPUT);
  }

  void lightonDay(){
  for(int i=0; i<NUMPIXELS; i++) { 

    pixels.setPixelColor(i, pixels.Color(224, 255, 255));
//    pixels.setPixelColor(i, pixels.Color(200, 150, 0));


    pixels.show();   
    delay(25); 
  }
      light = true;

  }

    void lightonNight(){
  for(int i=0; i<NUMPIXELS; i++) { 
    pixels.setPixelColor(i, pixels.Color(255, 69,0));

    pixels.show();    
    delay(25); 
  }
      light = true;

  }

  void lightAlarm(){
    
  for(int i=0; i<NUMPIXELS; i++) { 
    pixels.setPixelColor(i, pixels.Color(230, 70,210));
    
    pixels.show();   
    delay(25); 
  }
  light = true; 
  }

void lightoff(){

  for(int i = 0; i<NUMPIXELS; i++){
    pixels.setPixelColor(i, pixels.Color(0, 0, 0));
    pixels.show();
    delay(25);
}
    light = false;

  
}

void save(){


    int i = 0;
    char *p = strtok (bleInputString.c_str(), ":");

    while (p != NULL)
    {
        array[i++] = p;
        p = strtok (NULL, ":");
    }
    
      hour_t = atoi(array[0]); 
      minute_t = atoi(array[1]);

      Serial.print("Time saved on ");
      Serial.print(hour_t);
      Serial.print(":");
      Serial.print(minute_t);
      Serial.print("\n");
    
}


  void checkTheAlarmTime(int alarmHour, int alarmMinute) {
    

      if(alarmHour == myRTC.hours && alarmMinute == myRTC.minutes) {
            lightAlarm();
            alarm = false;
            timer.setTimeout(shut_down, lightoff);

  }

}



  void on (){
  for(int i=0; i<NUMPIXELS; i++) { 

    pixels.setPixelColor(i, pixels.Color(255, 31, 0));

    pixels.show();   

    delay(50); 
  }
  
  }


  void loop() {
    
  myRTC.updateTime();
 
  timer.run();

//  lightonDay();

//  lightAlarm();

//    lightoff();



    
//  if(myRTC.seconds != seconds_time){
//    seconds_time = myRTC.seconds;
//      Serial.print(myRTC.hours);
//      Serial.print(":");
//      Serial.print(myRTC.minutes);
//      Serial.print(":");
//      Serial.print(myRTC.seconds);
//      Serial.print("\n");
//      if(alarm){
//        checkTheAlarmTime(hour_t, minute_t);
//      }
//      
//  }



//    on();
//   
//  delay(1000);
//
//  off();
//
//
//  on2();
//  
//    delay(1000);
//  

  while (bluetooth.available()) {

    
    char inputChar = (char)bluetooth.read(); //bluetooth 로 부터 읽어온 int 형식의 값을 char 로 변환
    if (inputChar != '\n') {
      //1 바이트씩 전송되는 것을 모두 붙임. 다만 아래에서 input 값 비교할때를 위해 \n은 제외.
      bleInputString += inputChar;
//          Serial.print("POSSIBLE");

    }

    delay(5); // 수신 문자열 끊김 방지
  }

  while(Serial.available() ){
    byte data = Serial.read();
    bluetooth.write(data);
  }

  if (!bleInputString.equals("")) {

//    Serial.print("not empty");
    
    Serial.println(bleInputString); //디버그용 출력
    if (bleInputString == daykey) {
        lightonDay();
//        timer.setTimeout(shut_down, lightoff);

    } 
    else if(bleInputString == nightkey){

        lightonNight();
//        timer.setTimeout(shut_down, lightoff);
    }
     
    else if(strstr(bleInputString.c_str(),"timeJram") != NULL){
      alarm = true;
      save();
    }
    else if(bleInputString == "Purple"){
      lightAlarm();
//      timer.setTimeout(shut_down, lightoff);

    }
      
     else {
      lightoff();
      } 
  }   

//    Serial.write( "END LOOP ");
    

      
    bleInputString = "";

  }

