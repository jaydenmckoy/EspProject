// FtpManager.cpp
#include "FileManager.h"

#include "ESP-FTP-Server-Lib.h"
#include "FTPFilesystem.h"

#define SD_CS_PIN 13  // Use the GPIO pin number you've connected to the CS pin


#define FTP_USER     "esp32ftp"
#define FTP_PASSWORD "esp32ftp"

#define ROOT_DIR "/EspServer/"

FTPServer FileManager::ftpServer;

void FileManager::Task(void)
{
   // FTP server handle
   ftpServer.handle();
}

void FileManager::WriteFile(uint8_t* buf, size_t size, std::string filename)
{
   // Serial.println("Writing file: size= = %ld\n",size);
   // Save image to SD card
   // File file = SD.open(filename.c_str(), FILE_WRITE);
   File file = SD.open("/test_image.jpg", FILE_WRITE);
   if (file)
   {
      Serial.printf("-Writing file: %s (size=%ld)\n",filename.c_str(),size);
      file.write(buf, size);
      Serial.println("Closing file");
      file.close();
      Serial.printf("File saved: %s\n");;
   }
   else
   {
      Serial.printf("FileManager::WriteFile error: %s\n");
   }
}

bool FileManager::MakeDir(std::string dir)
{
   // Check if the directory exists
   if (!SD.exists(dir.c_str()))
   {
      // Create a new directory
      return SD.mkdir(dir.c_str());
   }
   else
   {
      Serial.printf("Directory already exists: %s\n",dir);
   }
   return true;
}

void FileManager::SdInit(void)
{
   Serial.print("-Mounting SD Card: ");
   SPIFFS.begin(true);
   SPI.begin(14, 2, 15);
   if (!SD.begin(SD_CS_PIN))
   {
      Serial.println("Failed");
      return;
   }
   else
   {
      Serial.println("Success");
   }
}

void FileManager::FtpInit(void)
{
   Serial.println("-Initializing FTP");
   ftpServer.addUser(FTP_USER, FTP_PASSWORD);
   ftpServer.addFilesystem("SD", &SD);
   ftpServer.begin();
}

void FileManager::Setup()
{
   Serial.println("Initializing FileManager:");
   
   // SD card init
   SdInit();
   // FTP init
   FtpInit();

   Serial.println("FileManager: Complete.");
}

void FileManager::WriteTestFile(void)
{
   Serial.println("Creating file");
   File file = SD.open("/testfile", FILE_WRITE);
   if (file)
   {
      file.close();
      Serial.printf("File saved: %s\n");;
   }
   else
   {
      Serial.printf("FileManager::WriteFile error: %s\n");
   }
}