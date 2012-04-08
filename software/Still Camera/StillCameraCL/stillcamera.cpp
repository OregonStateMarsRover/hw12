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

uint8_t StillCamera::_setMode(Mode mode)
{
    IdCommand id;
    id.plen = 0x01;
    id.id = SET_MODE;

    uint8_t params[1] = {mode};

    this->_sendCommand((uint8_t*)&id, 2);
    this->_sendCommand(params, 1);

    //todo: ack = waitforAck()

    return 0x00; //return ack code
}

uint8_t StillCamera::_setBaudRate(Baudrate rate)
{
    IdCommand id;
    id.plen = 0x01;
    id.id = SET_BAUDRATE;

    uint8_t params[1] = {rate};

    this->_sendCommand((uint8_t*)&id, 2);
    this->_sendCommand(params, 1);

    //todo: ack = waitforAck()

    return 0x00; //return ack code
}

uint8_t StillCamera::_setResComp(Resolution res, uint8_t comp)
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

    return 0x00; //return ack code

}

//public functions
void StillCamera::init()
{
    return;
}

uint8_t StillCamera::downloadFile(uint16_t file_id)
{

    //initiate download
    IdCommand id;
    id.plen = 0x02;
    id.id = DOWNLOAD_JPG;

    uint8_t params[2] = {(uint8_t)(file_id >> 8), (uint8_t)(file_id & 0x0F)};

    this->_sendCommand((uint8_t*)&id, 2);
    this->_sendCommand(params, 2);
    //wait to receive Ack, store the info
    uint8_t ack[38] = {0x12, 0x79, 0x00, 0x1a, 0x7d, 0x6e, 0x00, 0x1d, 0x50, 0x49, 0x43, 0x54, 0x30, 0x30, 0x30, 0x33, 0x2e, 0x41, 0x56, 0x49}; // = waitForAck();
    uint8_t packet_size = ack[0];
    uint32_t size = (ack[2]<<24) | (ack[3]<<16) | (ack[4]<<8) | (ack[5]);
    uint16_t n = (ack[6]<<8) | (ack[7]);
    char name[24] = {0x00};
    strncpy(name, (char*)(ack + 8), packet_size - 6);
    printf("%s", name);

    //create file pointer
    FILE *fp = fopen(name,"w");

    //wait receive packets, writing them into file

    uint16_t received = 0x0000;
    while (received < n)
    {
        uint8_t data[] = {0x41, 0x41}; // = waitForData();
        //write data
        fwrite(data, 1, sizeof(data), fp);
        //TODO:send ack

        received++;
    }

    //close file pointer

    return 0x00;
}

Mode StillCamera::getMode()
{
    IdCommand id;
    id.id = GET_MODE;
    id.plen = 0x00;


    this->_sendCommand((uint8_t*)&id, 2);

    //todo: ack = waitforAck()

    return (Mode)0x00; //return ack code
}


uint8_t StillCamera::setResolution(Resolution res)
{
    uint8_t code = this->_setResComp(res, this->compression);
    this->resolution = res;
    return code;
}

uint8_t StillCamera::setCompression(uint8_t comp)
{
    uint8_t code = this->_setResComp(this->resolution, comp);
    this->compression = comp;
    return code;
}
