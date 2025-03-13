//[1] Deklarasi Library LoRa
#include <SPI.h>
#include <RH_RF95.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <DHT.h>
RH_RF95 rf95;
DHT dht(2, DHT11);

//[2] Setting nilai dan deklarasi variable 
int Frequency = 915.0; // frekuensi carrier pada LoRa
int TxPower = 23; // kemampuan daya pancar dengan nilai 5 mW s/d 23 mW
int thisAddress = 203; // alamat yang digunakan oleh node
int headerFrom = 204; // alamat yang digunakan oleh node
int headerTo = 200; //alamat yang akan dituju
String dataIn, dataIns;
String cmd_balik = "OK";
String kirim;
float suhu, kelembapan;

//[3] Setting Format pengiriman data dengan LoRa
char buff [200]; 
uint8_t len = sizeof(buff);


void setup()
{
  Serial.begin (9600); //baud rate pada arduino
//  while (!Serial); // Apabila tidak ada serial komunikasi antar lora dengan arduino maka init failed
  if (!rf95.init())
  {
    Serial.print("init failed"); // tampilan pada text " init failed " pada serial monitor jika LoRa tidak terdeteksi
  }
//[4] setting LoRa
  rf95.setFrequency(Frequency);
  rf95.setTxPower(TxPower);

//[5] setting alamat 
  rf95.setThisAddress(thisAddress); // setting alamat gateway
  rf95.setHeaderTo(headerTo); // setting alamat node tujuan 
  rf95.setHeaderFrom(headerFrom); // setting alamat gateway

//[6] configurasi LoRa
  boolean longRange = false;
  if (longRange) {
    RH_RF95::ModemConfig modem_config = {
      0x78, // Reg 0x1D: BW=125kHz, Coding=4/8, Header=explicit
      0xC4, // Reg 0x1E: Spread=4096chips/symbol, CRC=enable
      0x08  // Reg 0x26: LowDataRate=On, Agc=Off.  0x0C is LowDataRate=ON, ACG=ON
    };
    rf95.setModemRegisters(&modem_config);
    if (!rf95.setModemConfig(RH_RF95::Bw125Cr48Sf4096)) {
      Serial.println("set config failed");
    }
  }
// [7] set DHT
  dht.begin();
}

void loop()
{
//[8] Program dht membaca suhu dan kelembapan
  suhu = dht.readTemperature();
  kelembapan = dht.readHumidity();
  
//[9] Program ketika gateway mode receive
  uint8_t buf[RH_RF95_MAX_MESSAGE_LEN];
  uint8_t len = sizeof (buf);
   
   if (rf95.waitAvailableTimeout(3000)){
    if (rf95.recv(buf, &len)) {  //jika Gateway menerima data, maka data yang masuk disimpan ke variable buf         
      dataIns = (char*) buf;   // konversi data yang masuk dari charArray ke String
      float rssi = rf95.lastRssi(); // menampilkan nilai indikator kekuatan sinyal (RSSI)
      kirim = dataIns + (String)rssi;  
      Serial.println (kirim);
    }
    delay(1000);
  }
  
  
  if (rf95.recv(buf, &len)){
    dataIn = (char*) buf;
    if (dataIn == "OK"){
      rf95.setHeaderTo(headerTo);
      String message = kirim + (String)suhu + "E" + (String)kelembapan + "F"; // Data yang akan dikirimkan
      message.toCharArray(buff, len);
      rf95.send(buff, sizeof(buff)); //perintah mengirim data
      rf95.waitPacketSent(); // menunggu data termessage semua
      rf95.setModeRx();
      Serial.println("To Gateway");
    }
  }

}
