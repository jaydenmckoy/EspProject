// camera.h
#ifndef CAMERA_H
#define CAMERA_H

#include <esp_camera.h>
#include <Arduino.h>

void TakeImage(void);
void TransferImage(camera_fb_t *fb);
void CameraSetup(void);

#endif // CAMERA_H
