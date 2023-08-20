// FtpManager.cpp
#include "FtpManager.h"

#include <SD.h>
#include <SPIFFS.h>
#include <WiFi.h>

#include "ESP-FTP-Server-Lib.h"
#include "FTPFilesystem.h"

#define WIFI_SSID     "SSID"
#define WIFI_PASSWORD "PASSWORD"

#define FTP_USER     "esp32ftp"
#define FTP_PASSWORD "esp32ftp"

FTPServer ftp;

void FtpHandle(void)
{
   ftp.handle();
}

void WriteFile(uint8_t* buf, size_t size)
{
   // Save image to SD card
   File file = SD.open("/image.jpg", FILE_WRITE);
   if (file)
   {
      file.write(buf, size);
      file.close();
      Serial.println("Image saved to SD card.");
   }
   else
   {
      Serial.println("Image save to SD card failed.");
   }
}

void FtpSetup(void)
{
   // SD card init
   // SPIFFS.begin(true);
   SPI.begin(14, 2, 15);
   if (!SD.begin(13)) {
      Serial.println("SD Card Mount Failed");
   }
   // // Initialize SD card
   // if (!SD.begin(SS, SPI, 4000000)) {
   //    Serial.println("SD Card initialization failed!");
   //    while (1);
   // }
   Serial.println("SD Card initialized.");
   
   // FTP init
   ftp.addUser(FTP_USER, FTP_PASSWORD);
   ftp.addFilesystem("SD", &SD);
   // ftp.addFilesystem("SPIFFS", &SPIFFS);
   ftp.begin();
}