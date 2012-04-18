/*Author: Mike Fortner
  */

#include <stdio.h>
#include "stillcamera.h"

int main(int argc, char *argv[])
{ 
	Serial *serial = new Serial();
	serial->open_port("/dev/ttyACM0");
	serial->configure_port();

    StillCamera *sc = new StillCamera(serial);

    printf("%x\n", sc->getMode());

    return 0;
}
