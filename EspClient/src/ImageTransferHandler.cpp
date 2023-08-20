#include "ImageTransferHandler.h"
// #include <websocketpp/client.hpp>


ImageTransferHandler::ImageTransferHandler(/* args */)
{
}

ImageTransferHandler::~ImageTransferHandler()
{
}

void ImageTransferHandler::ProcessImageTelemetry(ImagePacket_t *imgPkt)
{
   // // Unpack image packet
   // ImagePacket_t imgPkt = {0};
   // memcpy((uint8_t *) &imgPkt, tm_pkt->data, sizeof(ImagePacket_t));
   // imgPkt = (ImagePacket_t) tm_pkt->data;
   // Accumulate received data in a buffer
   imageBuffer.insert(imageBuffer.end(), imgPkt->data, imgPkt->data + CHUNK_SIZE_BYTES);
   // Check if you have received the entire image
   printf("Received packet %d of %d\n",imgPkt->seqNum, imgPkt->seqLen-1);
   if(imgPkt->seqNum == imgPkt->seqLen-1)
   {
      // Reconstruct and process the image
      ProcessReceivedImage(imageBuffer.data(), imageBuffer.size());
      // Clear the buffer for the next image
      imageBuffer.clear();
   }
}

// void ImageTransferHandler::HandleImageData(const uint8_t* data, size_t size) {
//    // Accumulate received data in a buffer
//    imageBuffer.insert(imageBuffer.end(), data, data + size);

//    // Check if you have received the entire image
//    if (imageBuffer.size() >= expectedImageSize) {
//       // Reconstruct and process the image
//       ProcessReceivedImage(imageBuffer.data(), imageBuffer.size());

//       // Clear the buffer for the next image
//       imageBuffer.clear();
//    }
// }

void ImageTransferHandler::ProcessReceivedImage(const uint8_t* imageData, size_t imageSize) {
   printf("Processing Image\n");

   // Decode and process the image using OpenCV or another library
   cv::Mat image = cv::imdecode(cv::Mat(imageSize, 1, CV_8U, (void*)imageData), cv::IMREAD_COLOR);

   if (!image.empty()) {
      // Process the image (display, save, etc.)
      cv::imwrite("./output_image.jpg",image);
   }
}
