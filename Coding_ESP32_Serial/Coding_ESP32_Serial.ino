
/**
* [Pengmas] Online Monitoring Kualitas Air Sungai Citarum 
*           Kerma dengan Dinas Lingkungan Hidup Jabar
*           
* Main Coding in ESP32
* Modify by :
*           1.Devandri Suherman
*           2.Daris Muhammad Rahman
*           3.Takhva Aininaim
*           4.Dini Rahmawati, S.ST., M.Sc.
*           Student Research from Electronics Engineering Departement, Bandung State of Polytechnic(1,2,3)
*           Lecturer Research from Electronics Engineering Departement, Bandung State of Polytechnic(4)
*           Bandung, 27 Agustus 2022
* Notes     : Serial Read from Communication with Arduino Mega
* Copyright 2022 
*/

#define RXD2 16
#define TXD2 17

void setup() {
  Serial.begin(9600);
  Serial2.begin(9600);

}

void loop() {
  // put your main code here, to run repeatedly:
  if (Serial2.available()){
  Serial.println(Serial2.read());}

}
