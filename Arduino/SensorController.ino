#include <SoftwareSerial.h>
#include <NewPing.h>
#include <TinyGPS.h>
#define MAX_DISTANCE 300 // Maximum sensor distance is rated at 400-500cm.
#define DATA_VERSION "1.0" // version of the current string sent to the raspberry
int TRIGGER_PIN[] = {8, 10, 12, 22, 24, 26};
int ECHO_PIN[] = {9, 11, 13, 23, 25, 27}; // pins used for the ultra sound sensors
byte PWM_PIN[] = {4, 5, 6, 7}; // remote controller pins
int pwm_value[4];
int GPSRxTx[] = {2,3};
/////////////////////////////////////////////////////////
NewPing sonar1(TRIGGER_PIN[0], ECHO_PIN[0], MAX_DISTANCE); // NewPing setup of pins and maximum distance.
NewPing sonar2(TRIGGER_PIN[1], ECHO_PIN[1], MAX_DISTANCE);
NewPing sonar3(TRIGGER_PIN[2], ECHO_PIN[2], MAX_DISTANCE);
NewPing sonar4(TRIGGER_PIN[3], ECHO_PIN[3], MAX_DISTANCE);
NewPing sonar5(TRIGGER_PIN[4], ECHO_PIN[4], MAX_DISTANCE);
NewPing sonar6(TRIGGER_PIN[5], ECHO_PIN[5], MAX_DISTANCE);
SoftwareSerial GpsSerial(GPSRxTx[0], GPSRxTx[1]);
TinyGPS gps;
void setup() {
 Serial.begin(9600);
 //GpsSerial.begin(4800);
 for(int i=0; i<4; i++) {
  pinMode(PWM_PIN[i], INPUT);
 }
}
void loop() {
 String tmp = String(DATA_VERSION) + ";" +
 RemRadController() + UltrasoundSensor() + GPSSensor();
 Serial.println(tmp);
 delay(500);
}
//////////// remote controller !
String RemRadController() {
 String tmpData = "";

 for(int i=0; i<4;i++) {
 pwm_value[i] = pulseIn(PWM_PIN[i], HIGH);
 if (pwm_value[i]>0) {
 tmpData += String(pwm_value[i]) + ";";
 }
 else {
 tmpData += "0;";
 }
 }
 return tmpData;
 // 1) eliche - 2) Sx DX - 3) Su Giú - 4) roto Sx Dx (1 - 2 - 3 - 5 )
}
/// ultrasound sensors
String UltrasoundSensor() {
 String TmpSensorsData = "";
 unsigned int uS1 = sonar1.ping(); // Send ping, get ping time in microseconds (uS).
 unsigned int uS2 = sonar2.ping(); // Send ping, get ping time in microseconds (uS).
 unsigned int uS3 = sonar3.ping(); // Send ping, get ping time in microseconds (uS).
 unsigned int uS4 = sonar4.ping(); // Send ping, get ping time in microseconds (uS).
 unsigned int uS5 = sonar5.ping(); // Send ping, get ping time in microseconds (uS).
 unsigned int uS6 = sonar6.ping(); // Send ping, get ping time in microseconds (uS).

 if ((uS1 / US_ROUNDTRIP_CM) > 0 && (uS1 / US_ROUNDTRIP_CM) < MAX_DISTANCE){
 TmpSensorsData += String(uS1 / US_ROUNDTRIP_CM) + ";"; // All data is gonna be in centimenters !
 } else { TmpSensorsData += "0;"; }
 if ((uS2 / US_ROUNDTRIP_CM) >0 && (uS2 / US_ROUNDTRIP_CM) < MAX_DISTANCE){
 TmpSensorsData += String(uS2 / US_ROUNDTRIP_CM) + ";";
 } else { TmpSensorsData += "0;"; }

 if ((uS3 / US_ROUNDTRIP_CM) >0 && (uS3 / US_ROUNDTRIP_CM) < MAX_DISTANCE){
 TmpSensorsData += String(uS3 / US_ROUNDTRIP_CM) + ";";
 } else { TmpSensorsData += "0;"; }

 if ((uS4 / US_ROUNDTRIP_CM) >0 && (uS4 / US_ROUNDTRIP_CM) < MAX_DISTANCE){
TmpSensorsData += String(uS4 / US_ROUNDTRIP_CM) + ";";
 } else { TmpSensorsData += "0;"; }

 if ((uS5 / US_ROUNDTRIP_CM) >0 && (uS5 / US_ROUNDTRIP_CM) < MAX_DISTANCE){
 TmpSensorsData += String(uS5 / US_ROUNDTRIP_CM) + ";";
 } else { TmpSensorsData += "0;"; }

 if ((uS6 / US_ROUNDTRIP_CM) >0 && (uS6 / US_ROUNDTRIP_CM) < MAX_DISTANCE){
 TmpSensorsData += String(uS6 / US_ROUNDTRIP_CM) + ";";
 } else { TmpSensorsData += "0;"; }
 return TmpSensorsData;
}
////// GPS
String GPSSensor() {
 unsigned long chars;
 bool newData = false;
 String TmpData = "";
 // For 1/2 second we parse GPS data and report some key values
 for (unsigned long start = millis(); millis() - start < 500;) {
 while (GpsSerial.available()) {
 char c = GpsSerial.read();
 if (gps.encode(c)) // Did a new valid sentence come in?
 newData = true;
 }
 }
 if (newData) {
 float flat, flon;
 unsigned long age;
 gps.f_get_position(&flat, &flon, &age);
 TmpData += (flat == TinyGPS::GPS_INVALID_F_ANGLE ? 0.0 : flat, 6) + ";";
 TmpData += (flon == TinyGPS::GPS_INVALID_F_ANGLE ? 0.0 : flon, 6) + "~";
 }
 else { TmpData = "NaN;NaN~"; }

 if (chars == 0)
 TmpData = "NaR;NaR~";
 return TmpData;
}
