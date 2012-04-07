/*Author: Mike Fortner
*/

#include "stillcamera.h"
#include "sc_commands.h"

#include <cstdio>



struct IdCommand { //also has a checksum after id and syncs (0xaa) on each end for every packet
    uint8_t plen;  //length of parameter command
    uint8_t id;    // command id number (from sc_commands.h)
};



//constructor
StillCamera::StillCamera(QObject *parent) :
    QObject(parent)
{
    this->mode = IDLE;
    this->resolution = RES1280;
    this->compression = 0x18;
}

//private functions

/* return lowest byte of the sum of the given bytes
  */
uint8_t StillCamera::_checksum8(uint8_t* command, int n)
{
    uint32_t sum = 2 * 0x000000aa;
    for(int i = 0; i < n; i++)
    {
        sum += (uint32_t)command[i];
    }
    sum = sum & 0x000000ff;
    return (uint8_t)sum;
}

/* return lowest two bytes of the sum of the given bytes
  */
uint16_t StillCamera::_checksum16(uint8_t* data, int n)
{
    uint32_t sum = 0;
    for(int i = 0; i < n; i++)
    {
        sum += (uint32_t)data[i];
    }
    sum = sum & 0x0000ffff;
    return (uint16_t)sum;
}

/*generates checksum and adds sync bytes on each end of the given array
  before sending over associated serial port
*/
void StillCamera::_sendCommand(uint8_t* command, int n)
{
    uint8_t *message = (uint8_t*)malloc((n+3)*sizeof(uint8_t));
    message[0] = message[n+2] = 0xaa;
    message[n+1] = this->_checksum8(command, n);
    for (int i = 0; i < n; i++)
    {
        message[i+1] = command[i];
    }

    //todo serial->send(message)
    for(int i = 0; i < n+3; i++)
    {
        printf("%02x", message[i]);
    }
    printf("\n");
}

void StillCamera::_setMode(Mode mode)
{
    IdCommand id;
    id.plen = 0x01;
    id.id = SET_MODE;

    uint8_t params[1] = {mode};

    this->_sendCommand((uint8_t*)&id, 2);
    this->_sendCommand(params, 1);

    //todo: ack = waitforAck()
}

void StillCamera::_setBaudRate(Baudrate rate)
{
    IdCommand id;
    id.plen = 0x01;
    id.id = SET_BAUDRATE;

    uint8_t params[1] = {rate};

    this->_sendCommand((uint8_t*)&id, 2);
    this->_sendCommand(params, 1);

    //todo: ack = waitforAck()
}

void StillCamera::_setResComp(Resolution res, uint8_t comp)
{
    IdCommand id;
    id.plen = 0x02;
    id.id = SET_RES_AND_COMP;

    if((comp < 0x00) || (comp > 0x2c))
    {
        printf("Bad value for compression ratio\n");
    }

    uint8_t params[2] = {res, comp};

    this->_sendCommand((uint8_t*)&id, 2);
    this->_sendCommand(params, 2);

    //todo: ack = waitforAck()

}

//public functions
void StillCamera::init()
{
    return;
}

int StillCamera::getMode()
{
    IdCommand id;
    id.id = 0x07;
    id.plen = 0x03;
    return this->_checksum8((uint8_t*)&id, 2);
}


void StillCamera::setResolution(Resolution res)
{
    this->_setResComp(res, this->compression);
    this->resolution = res;
}

void StillCamera::setCompression(uint8_t comp)
{
    this->_setResComp(this->resolution, comp);
    this->compression = comp;
}
