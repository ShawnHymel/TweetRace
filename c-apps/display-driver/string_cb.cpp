
#include <iostream>

#include "string_cb.h"
		  
string_cb::string_cb(uint32_t window)
{
	my_window = window;

	my_string = NULL;
	my_len = 0;
	my_locn = 0;
}

string_cb::~string_cb()
{
	if(my_string != NULL)
	{
		delete[] my_string;
		my_string = NULL;
	}
}

void string_cb::replace_string(char* new_string)
{
	uint32_t len;

	// strlen only gives actual length, not the null
	len = strlen(new_string);
		
	// if we had an old strong, purge it
	if(my_string != NULL)
	{
		delete[] my_string;
		my_string = NULL;
	}
		
	// suffix the string with a dislay full of empty characters
	// so we can scroll nicely if needed
	my_string = new char[len + my_window];
	memset(my_string, ' ',  len + my_window);
	
	strncpy(my_string, new_string, len);
	my_len  = len + my_window;
	my_locn = 0;
	
	std::cout << "Got new string: \"" << my_string << std::endl;

}