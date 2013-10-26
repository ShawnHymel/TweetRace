#ifndef _SHIM_H_
#define _SHIM_H_

// Some c-linkage functions to test the python binding...

#include "spi_mover.h"

bool shim_init(const char *);

bool shim_transfer(int, uint8_t*, uint8_t*);

void shim_uninit();

#endif // keepout
