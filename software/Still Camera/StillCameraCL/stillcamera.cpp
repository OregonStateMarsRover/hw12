/*Author: Mike Fortner
*/

#include "stillcamera.h"
#include "sc_commands.h"

StillCamera::StillCamera(QObject *parent) :
    QObject(parent)
{
    this->mode = IDLE;
}

int StillCamera::getMode()
{
    return (int)this->mode;
}
