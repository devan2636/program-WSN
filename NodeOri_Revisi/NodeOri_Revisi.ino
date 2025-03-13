/**
* Coding 1 Node to Gateway 
* Modify by :
*           1.Mochamad Ravi Adia Pramesti (181354014)
*           2.Muharammi Dzaki Pasya Sundava (18135422)
*           3.Devandri Suherman (191354007)
*           Bandung, 31 Agustus 2022
* Copyright 2022 
*/

//[1] Deklarasi Library LoRa
#include <SPI.h>
#include <RH_RF95.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
RH_RF95 rf95;


//[2]Setting nilai dan deklarasi variable
int Frequency = 915.0; // Set Frekuensi
int TxPower = 23; // Kemampuan daya pancar dengan nilai 5 mW s/d 23 mW
int thisAddress = 102; //alamat yang digunakan oleh node
int headerFrom = 102; // alamat yang digunakan oleh node
int headerTo = 101; //alamat yang akan dituju

//[3]Setting format pengiriman data dengan LoRa
char buff [200]; 
uint8_t len = sizeof(buff);

String dataIn;

void setup()
{
  //[4]Setting serial komunikasi
  Serial.begin (9600); //baud rate pada arduino
  //while (!Serial); // Apabila tidak ada serial komunikasi antar lora dengan arduino maka init failed
  while (!rf95.init())
  {
    Serial.print("init failed");
  }

  //[4.1]Setting LoRa
  rf95.setFrequency(Frequency);
  rf95.setTxPower(TxPower);

  //[5] Setting Alamat
  rf95.setThisAddress(thisAddress); //setting alamat gateway
  rf95.setHeaderTo(headerTo); //setting alamat node tujuan
  rf95.setHeaderFrom(headerFrom); //setting alamat gateway

  //[6] Konfigurasi LoRa
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
{
  //[7] Program mengirimkan pesan
  String message = "D";
  message.toCharArray(buff, len);  
  rf95.send(buff, sizeof(buff));
  rf95.waitPacketSent();

  //Send Data
  Serial.print("Data dikirim ke Gateway: ");
  Serial.println(String (buff));

}
