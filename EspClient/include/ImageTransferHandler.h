#ifndef IMAGE_TRANSFER_HANDLER_H
#define IMAGE_TRANSFER_HANDLER_H

#include "main.h"
#include <opencv2/opencv.hpp> // For image processing

// void ProcessImageTelemetry(TelemetryPacket_t *tm_pkt);

#define CHUNK_SIZE_BYTES 1000

#pragma pack(1)
struct ImagePacket_t
{
   uint8_t  id;
   uint32_t seqNum;
   uint32_t seqLen;
   uint32_t dataLen;
   uint8_t  data[CHUNK_SIZE_BYTES];
};

class ImageTransferHandler {
   public:
      ImageTransferHandler();
      ~ImageTransferHandler();
      void ProcessImageTelemetry(ImagePacket_t *imgPkt);
      // void HandleImageData(const uint8_t* data, size_t size);
      void ProcessReceivedImage(const uint8_t* imageData, size_t imageSize);

   private:
      std::vector<uint8_t> imageBuffer;
};

#endif