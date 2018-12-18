#include <TinyGPS.h>
#include <SoftwareSerial.h>

#include <Wire.h>
#include <Firebase.h>
#include <FirebaseArduino.h>
#include <FirebaseCloudMessaging.h>
#include <FirebaseError.h>
#include <FirebaseHttpClient.h>
#include <FirebaseObject.h>

#include <ArduinoJson.h>

#include <ESP8266WiFi.h>

#define Lat 5
#define Long 4
#define ir 1

#define FIREBASE_HOST "iot-7b9fd.firebaseio.com"

#define FIREBASE_AUTH "H2hErfO9THOSk7tfxZA2IJkyCx7HDuVDHI5Hzdqt"
#define WIFI_SSID "Mi8"

#define WIFI_PASSWORD "emre1234"

int state_1 = 1;
int state_2 = 1;
int state_3 = 1;

///DENMEE// 

/// IR PİN BACAĞI VE DEĞERİ KONTROLÜ ///
int IR_Pin = D6;  
int engel = HIGH;  
/// ARDUİNO ÜZERİNDEKİ LED ///
int  LED_Pin = 13 ; 
/// TİTREŞİM MOTORU PİN BACAĞI ///
int  TITRESIM_Pin = D7 ; 
/// TİLT SENSÖR PİN BACAĞI X Y Z ///
const int X_Pin = D4;                  
const int Y_Pin = D3;                  
const int Z_Pin = D2; 
/// GPS PİN ///
float lat = 28.5458,lon = 77.1703; // create variable for latitude and longitude object 
SoftwareSerial gpsSerial(D0,D1);
TinyGPS gps; 
String latitude,longitude;

void setup() {
    pinMode(LED_Pin, OUTPUT);
  pinMode(IR_Pin, INPUT);
  pinMode(TITRESIM_Pin, INPUT); 
 
  pinMode(LED_BUILTIN,HIGH);
 Serial.begin(115200);
 pinMode(Lat, OUTPUT);
 pinMode(Long, OUTPUT);

 digitalWrite(Lat, state_1);
 digitalWrite(Long, state_2);

 WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
 Serial.print("connecting");
 while (WiFi.status() != WL_CONNECTED) {
 Serial.print(".");
 delay(200);
 }
 Serial.println();
 Serial.print("connected: ");
 Serial.println(WiFi.localIP());
 
 Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
}
int n = 0;

void loop() {

   state_1 =Firebase.getInt("Lat");
 state_2 =Firebase.getInt("Long");
 state_3 =Firebase.getInt("ir");

  Firebase.setInt("ir",0);

 Serial.print("Lat: ");
 Serial.println(state_1);
 digitalWrite(Lat, !state_1);
 
  /// İR SENSÖR DEĞER ALMA VE KONTROL ///
  
  int a = ir_kontrol();
 
  /// TİTREŞİM DEĞERLERİ ALMA VE KONTROL ///
  
  long titresim =titresim_kontrol();
  Serial.println(titresim);
  /// X - Y - Z DEĞERLERİ ALMA ///
  
  Serial.print(analogRead(X_Pin));
  int x_pin =analogRead(X_Pin);
  Serial.println(x_pin);
  Serial.print("\t");
  
  Serial.print(analogRead(Y_Pin));
  int y_pin =analogRead(Y_Pin);
    Serial.println(y_pin);
  Serial.print("\t");
  
  Serial.print(analogRead(Z_Pin));
  int z_pin =analogRead(Z_Pin);
    Serial.println(z_pin);
  Serial.println();

  /// X - Y - Z DEĞERLERİ KONTROL ETME ///
  
  //if ((x_pin >330) && (x_pin <360) && (y_pin >338) && (y_pin <368) && (z_pin >265) && (z_pin <295) && (titresim<1000) && (a==1)){
  if ((titresim>10) ){

  
        
         Serial.println("Kaza Oldu");                      
         Firebase.setFloat("Lat",lat);
         Firebase.setFloat("Long",lon);    
         delay(2000);
    

    
      }
      
      else if (a==0){
      
          Firebase.setInt("ir",1);
          
      }
    
    else  {
        Serial.println("Normal"); 
          
      }
        /// GPS BAĞLANDIĞINDA LAT LONG GÜNCELLEME ///
     while(gpsSerial.available()){ 
                          if(gps.encode(gpsSerial.read()))
                          { 
                          gps.f_get_position(&lat,&lon); 
                         }
                        }
                        
                        latitude = String(lat,6);
                        longitude = String(lon,6);
                        Serial.println(latitude+";"+longitude);
                        delay(1000);
}

long ir_kontrol(){
  
   engel = digitalRead(IR_Pin); 
  if (engel == LOW) 
  {
    Serial.println("Engel Var");
    digitalWrite(LED_Pin, HIGH);
    return 0;
  }
  else
  {
    Serial.println("Yol Açık");
    digitalWrite(LED_Pin, LOW);
    return 1;
  }
}
long titresim_kontrol(){
  delay(10);
  long titresim=pulseIn (TITRESIM_Pin, HIGH);
  return titresim;
}

/*void loop() {
  
 state_1 =Firebase.getInt("Lat");
 state_2 =Firebase.getInt("Long");
 

 Serial.print("Lat: ");
 Serial.println(state_1);
 digitalWrite(Lat, !state_1);
 
 // get value 
 Serial.print("Long: ");
 Serial.println(state_2);
 digitalWrite(Long, !state_2);
 
Firebase.setInt("Lat",10);
Firebase.setInt("Long",15);

 Serial.println("________________________");
 delay(100);
}*/
