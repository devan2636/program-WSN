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

//[Setting nilai dan deklarasi variable
int Frequency = 915.0; // Frekuensi pada Lora
int TxPower = 23; // kemampuan daya pancar pada perangkat wireless ; 5 s/d 23
int thisAddress = 201; // Pengalamatan pada gateway
int headerFrom = 201; // Pengalamatan dari gateway
String dataIn;
String cmd_balik = "OK";

//[3] Setting format pengiriman data dengan LoRa
char buff [200];
uint8_t len = sizeof(buff); //menyimpan panjang char array

void setup()
{
  Serial.begin(9600);
  while (!Serial);
  //[4] Setting LoRa
  while (!rf95.init())
    Serial.println("init failed");
  rf95.setFrequency(Frequency);
  rf95.setTxPower(TxPower);

  //[5] Setting Alamat
  rf95.setThisAddress(thisAddress); //alamat gateway
  //rf95.setHeaderTo(headerTo); //Setting alamat node tujuan
  rf95.setHeaderFrom(headerFrom); //alamat gateway

  //[6] Configurasi LoRa
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
  //[7] Program ketika gateway mode receive
  while (rf95.available()) {
    uint8_t buf[RH_RF95_MAX_MESSAGE_LEN]; 
    uint8_t len = sizeof (buf);           
    if (rf95.recv(buf, &len)) {           
      dataIn = (char*) buf;              
      
      //menampilkan nilai RSSI
      float rssi = rf95.lastRssi();
      Serial.print ("Data dari Sensor Node: ");
      Serial.println (dataIn);
      Serial.print ("RSSI : ");
      Serial.println (rssi);
    }
  }

}
