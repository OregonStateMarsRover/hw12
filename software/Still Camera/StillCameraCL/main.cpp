/*Author: Mike Fortner
  */

#include <stdio.h>
#include "stillcamera.h"

int main(int argc, char *argv[])
{ 
    StillCamera *sc = new StillCamera();
    sc->setCompression(0x2c);
    sc->setResolution(RES640);
    sc->setCompression(0x18);
    printf("%x\n", sc->getMode());

    sc->downloadFile(0x0001);

    return a.exec();
}
