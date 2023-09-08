// FtpManager.cpp
#include "FileManager.h"

#include "ESP-FTP-Server-Lib.h"
#include "FTPFilesystem.h"

#include "Telemetry.h"
#include "TelemetryManager.h"

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

void FileManager::GetFile(void)
{
   File file = SD.open("/test_image.jpg");

   if (file) {
      Serial.println("File opened successfully:");

      size_t fileSize = file.size();
      // uint8_t * buf;
      // size_t bytesRead = file.read(buf, fileSize);
      // Serial.printf("File size: %ld\n",fileSize);
      // Serial.printf("Bytes read: %ld\n",bytesRead);
      // Get instance of TelemetryManager
      TelemetryManager& tmMgr = TelemetryManager::GetInstance();
      // Start transfer
      TelemetryPacket_t pkt = {0};
      pkt.apid = FILE_TX_START_TM_APID;
      tmMgr.SendTelemetryPacket(&pkt);
      // Send data
      pkt.apid = GET_FILE_TM_APID;
      size_t bytesSent = 0;
      int numBytes;
      size_t pktSizeMax = TELEMETRY_PACKET_MAX_SIZE;
      Serial.printf("Sending data:\n");
      Serial.printf("File size: %ld\n",fileSize);
      size_t numPkts = fileSize/pktSizeMax;
      Serial.printf("Number of packets: %ld\n", numPkts);
      size_t i = 0;
      while(bytesSent < fileSize)
      {
         Serial.printf("Sending packet %ld of %ld\n",i,numPkts);
         numBytes = std::min(fileSize - bytesSent, pktSizeMax);
         file.read(pkt.data,numBytes);
         if(tmMgr.SendTelemetryPacket(&pkt) == false)
         {
            Serial.println("Error sending telemetry. Aborting.");
            break;
         }
         bytesSent += numBytes;
         delay(500);
         i++;
      }
      // SendTelemetry(GET_FILE_TM_APID,buf,fileSize);
      // End transfer
      delay(500);
      pkt.apid = FILE_TX_END_TM_APID;
      tmMgr.SendTelemetryPacket(&pkt);

      file.close();
      Serial.println("\nFile closed");
   } else {
      Serial.println("Error opening file");
   }
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