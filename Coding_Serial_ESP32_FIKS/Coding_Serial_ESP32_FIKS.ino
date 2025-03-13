/*
 * Nama   : Devandri Suherman
 * NIM    : 191354007
 * Judul  : Pemantauan Suhu dan Kelembaban Permukaan Bumi
 * Keg    : WSN
 * Tanggal: 28-09-2022
 * Revisi : 0
 */


//GPIO17 itu tx2
//GPIO16 itu rx2
String data_Received;

//Konfigurasi Wi-Fi
#ifdef ESP32
  #include <WiFi.h>
  #include <HTTPClient.h>
#else
  #include <ESP8266WiFi.h>
  #include <ESP8266HTTPClient.h>
  #include <WiFiClient.h>
#endif

// Replace with your network credentials
const char* ssid     = "Astroboy-MiFi";
const char* password = "Astronaut26@";

const char* serverNodeA = "http://wsn.astroboy-engineer.my.id/data_node/post-dataNodeA.php";
String apiKeyValue = "tPmAT5Ab3j7F3";

String respon = "{}";

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial2.begin(9600);

  //Connect WiFi
  WiFi.begin(ssid, password);
  Serial.println("Connecting");
  while(WiFi.status() !=WL_CONNECTED){
    delay(500);
    Serial.print(".");
   }
   Serial.println("");
   Serial.print("Connected to WiFi network with IP Address: ");
   Serial.println(WiFi.localIP());

}
void loop() {
while(Serial2.available() > 0){
  delay(10);
  char d = Serial2.read();
  data_Received += d;
}
if (data_Received.length() > 0){
  Serial.println(data_Received);
  Serial.println("DATA PEMILAHAN DARI MASING-MASING NODE");
  //Memparsing Data
  String subdata_Received0 = getValue(data_Received, ' ', 0);
  String subdata_Received1 = getValue(data_Received, ' ', 1);
  String subdata_Received2 = getValue(data_Received, ' ', 2);
  String subdata_Received3 = getValue(data_Received, ' ', 3);
  String subdata_Received4 = getValue(data_Received, ' ', 4);
  String subdata_Received5 = getValue(data_Received, ' ', 5);
  String subdata_Received6 = getValue(data_Received, ' ', 6);
  String subdata_Received7 = getValue(data_Received, ' ', 7);

  //Menampilkan data parsing
  Serial.print("Dari Node : ");Serial.println(subdata_Received0);
  Serial.print("Temperature : ");Serial.println(subdata_Received1);
  Serial.print("Humidity (%) : ");Serial.println(subdata_Received2);
  Serial.print("RSSI : ");Serial.println(subdata_Received3);

  //Apabila masih ada
  Serial.print("Dari Node : ");Serial.println(subdata_Received4);
  Serial.print("Temperature : ");Serial.println(subdata_Received5);
  Serial.print("Humidity (%) : ");Serial.println(subdata_Received6);
  Serial.print("RSSI : ");Serial.println(subdata_Received7);

  //Kirim Data Node A
  if(WiFi.status()== WL_CONNECTED && subdata_Received0 == "A"){
    WiFiClient client;
    HTTPClient http;
    http.begin(client, serverNodeA);
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");

    String httpRequestData = "api_key=" + apiKeyValue + "&par_suhu=" + String(subdata_Received1) + 
                "&par_humidity=" + String(subdata_Received2) + "&par_RSSI=" + String(subdata_Received3) + "";
    
    Serial.print("httpRequestData: ");
    Serial.println(httpRequestData);
    int httpResponseCode = http.POST(httpRequestData);

    if (httpResponseCode>0) {
          Serial.print("HTTP Response code: ");
          Serial.println(httpResponseCode);
          respon = http.getString();
          Serial.println(respon);
    }else{
                Serial.print("Error code: ");
          Serial.println(httpResponseCode);
          respon = http.getString();
          Serial.println(respon);
    }
    http.end();
  }
  //Mengosongkan data
  data_Received = "";
  }
}
