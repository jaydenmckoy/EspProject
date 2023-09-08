// camera.cpp
#include "Camera.h"
#include "FileManager.h"
#include "Telemetry.h"
#include "TelemetryManager.h"
#include "Websocket.h"

#define CHUNK_SIZE_BYTES 1000
uint8_t imageId = 0;

#pragma pack(1)
struct ImagePacket_t
{
   uint8_t id;
   uint32_t seqNum;
   uint32_t seqLen;
   uint32_t dataLen;
   uint8_t data[CHUNK_SIZE_BYTES];
};

void TransferImage(camera_fb_t *fb)
{
   Serial.println("Initializing image transfer...");
   
   // Get instance of TelemetryManager
   TelemetryManager& tmMgr = TelemetryManager::GetInstance();

   // TM init
   TelemetryPacket_t tm_pkt = {0};
   tm_pkt.apid = TAKE_IMAGE_TM_APID;
   ImagePacket_t imgPkt;
   imgPkt.id = imageId;
   imgPkt.seqNum = 0;
   imgPkt.seqLen = fb->len / CHUNK_SIZE_BYTES;
   imgPkt.dataLen = sizeof(ImagePacket_t);
   uint8_t *dataBuf = fb->buf;
   bool txRes;

   Serial.println("Transfering image...");

   for (imgPkt.seqNum = 0; imgPkt.seqNum < imgPkt.seqLen; imgPkt.seqNum++)
   {
      Serial.printf("Sending packet %d of %d.\n", imgPkt.seqNum, imgPkt.seqLen-1);

      // Copy image data to image packet
      memcpy(imgPkt.data, dataBuf, CHUNK_SIZE_BYTES);
      // Copy image packet to telemetry packet
      memcpy(tm_pkt.data,(uint8_t *) &imgPkt, sizeof(ImagePacket_t));
      // Send telemetry packet
      txRes = tmMgr.SendTelemetryPacket(&tm_pkt);
      if (txRes == false) {
         Serial.println("Aborting image transfer.");
         return;
      }
      dataBuf += CHUNK_SIZE_BYTES;

      // Serial.printf("imgPkt.id        = %d\n", imgPkt.id);
      // Serial.printf("imgPkt.seqNum    = %d\n", imgPkt.seqNum);
      // Serial.printf("imgPkt.seqLen    = %d\n", imgPkt.seqLen);
      // Serial.printf("imgPkt.dataLen   = %d\n", imgPkt.dataLen);
      // int i;
      // for (i = 0; i < CHUNK_SIZE_BYTES; i++)
      //    Serial.printf("[%d]: %02X\n", i, imgPkt.data[i]);
      // return;

      delay(1000);
   }

   Serial.println("Image transfer complete.");
   imageId++;
}

void TakeImage(void)
{
   camera_fb_t *fb = esp_camera_fb_get();
   if (fb)
   {
      Serial.println("Successful image capture.");
      Serial.printf("Length of the buffer in bytes:  %d\n", fb->len);
      Serial.printf("Width of the buffer in pixels:  %d\n", fb->width);
      Serial.printf("Height of the buffer in pixels: %d\n", fb->height);
      Serial.printf("Format of the pixel data:       %d\n", fb->format);

      // WriteFile(fb->buf,fb->len);
      // FileManager::WriteFile(fb->buf,fb->len,"/test_image.jpg");
      // Save image to SD card
      File file = SD.open("/test_image.jpg", FILE_WRITE);
      if (file) {
         file.write(fb->buf, fb->len);
         file.close();
         Serial.println("Image saved to SD card.");
      }
      // TransferImage(fb);

      esp_camera_fb_return(fb);
   }
   else
   {
      Serial.println("Error capturing image.");
   }
}

void GetImage(void)
{
   File file = SD.open("/test_image.jpg");

   if (file) {
      Serial.println("File opened successfully:");

      size_t fileSize = file.size();
      camera_fb_t fb;
      // uint8_t buffer[fileSize];

      if (file.read(fb.buf, fileSize) == fileSize) {
         // Serial.write(buffer, fileSize);
         fb.len = fileSize;
         TransferImage(&fb);
      } else {
         Serial.println("Error reading file");
      }

      file.close();
      Serial.println("\nFile closed");
   } else {
      Serial.println("Error opening file");
   }
}

void CameraSetup(void)
{
   Serial.print("Setting up camera: ");
   // Initialize the camera
   camera_config_t config;
   config.ledc_channel = LEDC_CHANNEL_0;
   config.ledc_timer = LEDC_TIMER_0;
   config.pin_d0 = 5;
   config.pin_d1 = 18;
   config.pin_d2 = 19;
   config.pin_d3 = 21;
   config.pin_d4 = 36;
   config.pin_d5 = 39;
   config.pin_d6 = 34;
   config.pin_d7 = 35;
   config.pin_xclk = 0;
   config.pin_pclk = 22;
   config.pin_vsync = 25;
   config.pin_href = 23;
   config.pin_sccb_sda = 26;
   config.pin_sccb_scl = 27;
   config.pin_pwdn = 32;
   config.pin_reset = -1;
   config.xclk_freq_hz = 20000000;
   config.pixel_format = PIXFORMAT_JPEG;
   if (psramFound())
   {
      config.frame_size = FRAMESIZE_UXGA;
      config.jpeg_quality = 10;
      config.fb_count = 2;
   }
   else
   {
      config.frame_size = FRAMESIZE_SVGA;
      config.jpeg_quality = 12;
      config.fb_count = 1;
   }
   // Initialize the camera with the configuration
   esp_err_t err = esp_camera_init(&config);
   if (err != ESP_OK)
   {
      Serial.printf("Camera init failed with error 0x%x", err);
      return;
   }
   Serial.println("Complete.");
}
