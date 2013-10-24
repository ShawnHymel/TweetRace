
#include "spi_mover.h"

#include "shim.h"


static spi_mover * instance_p = NULL;


bool shim_init(char * path)
{
	if(instance_p != NULL)
	{
		return false;
	}

	instance_p = new spi_mover(path);
	
	return true;
}


bool shim_transfer(int num, char* out,char* in)
{
	if(instance_p == NULL)
	{
		return false;
	}

	return(instance_p->transfer(num,  out, in));
}


void shim_uninit()
{
	if(instance_p != NULL)
	{
		delete instance_p;
		instance_p = NULL;
	}
}
