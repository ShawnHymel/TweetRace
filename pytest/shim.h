#ifndef _SHIM_H_
#define _SHIM_H_

// Some c-linkage functions to test the python binding...

#include "spi_mover.h"

bool shim_init(char *);

bool shim_transfer(int, char*, char*);

void shim_uninit();

#endif // keepout