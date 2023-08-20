// FtpManager.h
#ifndef FTP_MANAGER_H
#define FTP_MANAGER_H
#include <Arduino.h>

void FtpHandle(void);
void WriteFile(uint8_t* buf, size_t size);
void FtpSetup(void);

#endif