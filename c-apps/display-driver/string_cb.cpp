
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
	// so we can scroll nicely 
	
	//std::cout << "Replace len: " << len << "string: " << new_string << std::endl;
	
	
	// OK - the trick to all this:
	// We allocate a biffer that's (strlen + window size) long.
	// We fill it with the string, then a bunch of spaces.
	//
	// When we display the string, we go from my_locn to my_locn + window_sz, and fetch characters.
	// In the char fetch routine, it will simply wrap around when the request index goes off the end.
	// When the current pointer gets advanced, it will walk to len+window before resetting to 0.
	//
	// All of this means is that the string will walk in the window, with a
	// completely blank window between each sequence.
	
	my_string = new char[len + my_window + 1];
	memset(my_string, ' ',  len + my_window);
	my_string[len + my_window] = NULL;
	
	strncpy(my_string, new_string, len);
	my_len  = len;
	my_locn = 0;
	
	//std::cout << "Got new string: " << my_string << std::endl;

}

char string_cb::get_current(uint32_t offset)
{
#if 0
	if(offset > (my_len + my_window))
	{
		return ' ';
	}
#endif

	// No string stored?  Just return blank
	if(my_len == 0)
	{
		return ' ';
	}

	int index = (my_locn + offset) % (my_len + my_window);

	//std::cout << "string returning index: " << offset << " char: " << my_string[my_locn + offset] << std::endl;
	
	return(my_string[index]);
}

void string_cb::next_step()
{
	my_locn++;
	
	if(my_locn >= (my_len + my_window) )
	{
		my_locn = 0;
	}
}