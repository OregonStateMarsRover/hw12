/*Author: Mike Fortner
  */

#ifndef SC_COMMANDS_H
#define SC_COMMANDS_H

//command IDs
#define SET_MODE 0x1e //select current operating mode
#define GET_MODE 0x1f //request current operating mode

#define SET_BAUDRATE 0x9f //set baud rate, 0x04 = 57600, 0x05 = 115200

#define SET_RES_AND_COMP 0x32  //set picture resolution and compression ratio

#endif // SC_COMMANDS_H