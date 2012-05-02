/*BogieController.h
 *Authors: Mike Fortner, Chris Newman
 */

#ifndef BOGIE_CONTROLLER_H
#define BOGIE_CONTROLLER_H

#include "BogieCommon.h"
#include "Sabertooth.h"
#include "Encoders.h"

#include "../general/xmegaDrivers/Timer/TC_driver.h"
#include "../general/xmegaDrivers/Quadrature/qdec_driver.h"

 void bogie_controller_init(void);

 void parse_command();

 int main(void);

#endif