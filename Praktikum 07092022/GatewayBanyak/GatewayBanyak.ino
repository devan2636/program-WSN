//[1] Deklarasi Library LoRa
#include <SPI.h>
#include <RH_RF95.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
RH_RF95 rf95;

//[2] Setting nilai dan deklarasi variable 
int Frequency = 915.0; // frekuensi carrier pada LoRa
int TxPower = 23; // kemampuan daya pancar dengan nilai 5 mW s/d 23 mW
int thisAddress = 201; // alamat yang digunakan oleh gateway
int headerFrom = 201; // alamat yang digunakan oleh gateway
String dataIn;
String cmd_balik = "OK";

//[3] Setting Format pengiriman data dengan LoRa
char buff [200]; 
uint8_t len = sizeof(buff); //menyimpan panjang char Array

void setup()
{
  Serial.begin(9600); //baud rate arduino
  while (!Serial); 
  if (!rf95.init()) // test koneksi lora 
    Serial.println("init failed"); // tampilan pada text " init failed " pada serial monitor jika LoRa tidak terdeteksi

//[4] input nilai frequensi dan TxPower LoRa
  rf95.setFrequency(Frequency);
  rf95.setTxPower(TxPower);

//[5] setting alamat 
  rf95.setThisAddress(thisAddress); // setting alamat gateway
  //rf95.setHeaderTo(headerTo); // setting alamat node tujuan 
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
}

void loop()
//[7] Program ketika gateway mode receive 
{
  for (int ToNode = 202; ToNode <= 205; ToNode++){
    rf95.setHeaderTo(ToNode);

    String message = cmd_balik;
    message.toCharArray(buff, len);
    rf95.send(buff, sizeof(buff));
    rf95.waitPacketSent();

    rf95.setModeRx();
    uint8_t buf[RH_RF95_MAX_MESSAGE_LEN];
    uint8_t len = sizeof (buf); 
    if (rf95.waitAvailableTimeout(3000)){
      if (rf95.recv(buf, &len)) {  //jika Gateway menerima data, maka data yang masuk disimpan ke variable buf         
        dataIn = (char*) buf;   // konversi data yang masuk dari charArray ke String
        float rssi = rf95.lastRssi(); // menampilkan nilai indikator kekuatan sinyal (RSSI)
        String kirim = dataIn + rssi;
        Serial.println (kirim);
      }
      else {
        int data = ToNode - 201;
        dataIn = (char*) buf;
        String kirim = dataIn + "999";
        Serial.print("error data sensor ke");
        Serial.println(data);
      }
      delay(1000);
    }
  }
}
