/* Author: Mike Fortner
  */

#ifndef STILLCAMERA_H
#define STILLCAMERA_H

#define STRICT

#include "stdint.h"
#include "serial.h"

#define DATA_PACKET_BYTES 614343

struct IdCommand;
enum Mode {FAILED = 0x01, USB = 0x02, IDLE = 0x03, CAPTURE_JPG = 0x04, CAPTURE_AVI = 0x05, PLAYBACK = 0x06};
enum Baudrate {BAUD57600 = 0x04, BAUD115200 = 0x05};
enum Resolution {RES1280 = 0x00, RES640 = 0x01};

class StillCamera
{
    Serial *serial;
    Mode mode;
    Resolution resolution;
    uint8_t compression;


    uint16_t pict_count;


    uint8_t _checksum8(uint8_t* command, int n);
    uint16_t _checksum16(uint8_t* command, int n);
    uint8_t _waitForAck(uint8_t* ack, int n);
    uint8_t _waitForData(uint8_t* data, int n);

    void _sendCommand(uint8_t* command, int n);
    uint8_t _setMode(Mode mode);
    uint8_t _setBaudRate(Baudrate rate);
    uint8_t _setResComp(Resolution res, uint8_t comp);

public:
    explicit StillCamera(Serial *serial);
    void init();
    Mode getMode();
    uint8_t setResolution(Resolution res);
    uint8_t setCompression(uint8_t comp);
    uint8_t takePicture();
    uint8_t downloadFile(uint16_t file_id);

//signals:
    
//public slots:
    
};

#endif // STILLCAMERA_H
