// FtpManager.h
#ifndef FILE_MANAGER_H
#define FILE_MANAGER_H

#include <main.h>
#include <FS.h>
#include <SD.h>
#include <SPIFFS.h>
#include "ESP-FTP-Server-Lib.h"
#include "FTPFilesystem.h"

class FileManager
{
   public:
      static void Setup(void);
      static void SdInit(void);
      static void FtpInit(void);
      static void Task(void);
      static void WriteFile(uint8_t* buf, size_t size, std::string filename);
      static bool MakeDir(std::string dir);
      static void WriteTestFile(void);
   private:
      static FTPServer ftpServer;
};

#endif