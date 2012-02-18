/* Author: Mike Fortner
  */

#ifndef STILLCAMERA_H
#define STILLCAMERA_H

#include <QObject>
enum Mode {FAILED = 0x01, USB = 0x02, IDLE = 0x03, CAPTURE_JPG = 0x04, CAPTURE_AVI = 0x05, PLAYBACK = 0x06};
class StillCamera : public QObject
{
    Q_OBJECT
    Mode mode;
public:
    explicit StillCamera(QObject *parent = 0);
    int getMode();
signals:
    
public slots:
    
};

#endif // STILLCAMERA_H
