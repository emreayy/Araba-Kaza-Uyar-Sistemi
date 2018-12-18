#include <TinyGPS.h>
#include <SoftwareSerial.h>
#include <LiquidCrystal.h>
#include <Wire.h>

/// LCD PİN BACAKLARI ///
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
/// IR PİN BACAĞI VE DEĞERİ KONTROLÜ ///
int IR_Pin = 8;  
int engel = HIGH;  
/// ARDUİNO ÜZERİNDEKİ LED ///
int  LED_Pin = 13 ; 
/// TİTREŞİM MOTORU PİN BACAĞI ///
int  TITRESIM_Pin = 7 ; 
/// TİLT SENSÖR PİN BACAĞI X Y Z ///
const int X_Pin = A5;                  
const int Y_Pin = A4;                  
const int Z_Pin = A3; 
/// GPS PİN ///
float lat = 28.5458,lon = 77.1703; // create variable for latitude and longitude object 
SoftwareSerial gpsSerial(1,0);
TinyGPS gps; 

String latitude,longitude;



void setup() {
  // put your setup code here, to run once:
  lcd.begin(16, 2);
  lcd.print("IOT PROJESI!");
  delay(2000);
  lcd.clear();
  lcd.print("TRAVELLER DURING");
  lcd.setCursor(4, 1);
  lcd.print("ACCIDENT");
  delay(1000);
  pinMode(LED_Pin, OUTPUT);
  pinMode(IR_Pin, INPUT);
  pinMode(TITRESIM_Pin, INPUT); 
  Serial.begin(9600);
  pinMode(LED_BUILTIN,HIGH);
  delay(100);

}

void loop() {
  
  /// İR SENSÖR DEĞER ALMA VE KONTROL ///
  
  int a = ir_kontrol();

  /// TİTREŞİM DEĞERLERİ ALMA VE KONTROL ///
  
  long titresim =titresim_kontrol();
  Serial.println("Titreşim"+titresim);
  /// X - Y - Z DEĞERLERİ ALMA ///
  
  Serial.print(analogRead(X_Pin));
  int x_pin =analogRead(X_Pin);
  Serial.print("\t");
  
  Serial.print(analogRead(Y_Pin));
  int y_pin =analogRead(Y_Pin);
  Serial.print("\t");
  
  Serial.print(analogRead(Z_Pin));
  int z_pin =analogRead(Z_Pin);
  Serial.println();

  /// X - Y - Z DEĞERLERİ KONTROL ETME ///
  
  if ((x_pin >330) && (x_pin <360) && (y_pin >338) && (y_pin <368) && (z_pin >265) && (z_pin <295) && (titresim<1000) && (a==1)){
      lcd.clear();
      lcd.setCursor(5, 2);
      lcd.print("NORMAL");
      Serial.println("Normal");
      }
      else if (a==0){
          lcd.clear();
          lcd.print("IR AKTIF ");
          lcd.setCursor(5, 2);
          lcd.print("ENGEL VAR");
      }
         else  {
          lcd.clear();
          lcd.print("TILT AKTIF");
          lcd.setCursor(5, 2);
          lcd.print("KAZA OLDU");
          Serial.println("Kaza Oldu");                      
          ///    LAT LONG EKRANA YAZMA  /// 
           lcd.clear();
           lcd.print("lat  "+latitude);                
           lcd.setCursor(2, 1);
           lcd.print("long   "+longitude);       
             delay(2000);
      }
        /// GPS BAĞLANDIĞINDA LAT LONG GÜNCELLEME ///
     while(gpsSerial.available()){ 
                          if(gps.encode(gpsSerial.read()))
                          { 
                          gps.f_get_position(&lat,&lon); 
                         
                          lcd.clear();
                          lcd.setCursor(1,0);
                          lcd.print("GPS Signal");
                        
                          lcd.setCursor(1,0);
                          lcd.print("LAT:");
                          lcd.setCursor(5,0);
                          lcd.print(lat);
                          //Serial.print(lat);
                          //Serial.print(" ");
                          
                          lcd.setCursor(0,1);
                          lcd.print(",LON:");
                          lcd.setCursor(5,1);
                          lcd.print(lon);
                          
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
