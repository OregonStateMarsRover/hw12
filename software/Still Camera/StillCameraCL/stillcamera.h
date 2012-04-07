/* Author: Mike Fortner
  */

#ifndef STILLCAMERA_H
#define STILLCAMERA_H

#include <QObject>
#define STRICT
#include <tchar.h>
#include <windows.h>
#include "stdint.h"
//#include "Serial.h"

struct IdCommand;
enum Mode {FAILED = 0x01, USB = 0x02, IDLE = 0x03, CAPTURE_JPG = 0x04, CAPTURE_AVI = 0x05, PLAYBACK = 0x06};
enum Baudrate {BAUD57600 = 0x04, BAUD115200 = 0x05};
enum Resolution {RES1280 = 0x00, RES640 = 0x01};

class StillCamera : public QObject
{
    Q_OBJECT
    Mode mode;
    Resolution resolution;
    uint8_t compression;
    //CSerial *serial;

    uint8_t _checksum8(uint8_t* command, int n);
    uint16_t _checksum16(uint8_t* command, int n);
    void _sendCommand(uint8_t* command, int n);
    void _setMode(Mode mode);
    void _setBaudRate(Baudrate rate);
    void _setResComp(Resolution res, uint8_t comp);

public:
    explicit StillCamera(QObject *parent = 0);
    void init();
    int getMode();
    void setResolution(Resolution res);
    void setCompression(uint8_t comp);
    int takePicture();

signals:
    
public slots:
    
};

#endif // STILLCAMERA_H
