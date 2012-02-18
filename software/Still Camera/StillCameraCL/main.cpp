/*Author: Mike Fortner
  */

#include <QtCore/QCoreApplication>
#include <stdio.h>
#include "stillcamera.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    
    StillCamera *sc = new StillCamera();
    printf("%d\n", sc->getMode());

    return a.exec();
}
