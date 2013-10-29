#ifndef _STRING_CB_H_
#define _STRING_CB_H_

#include <stdint.h>
#include <string.h>

class string_cb
{
	public:
		string_cb(uint32_t window);
		~string_cb();
		void replace_string(char*, uint32_t);
		
		char get_current(uint32_t offset);
		void next_step();
	
	private:
		string_cb();
	
		char * my_string;
		
		uint32_t my_window;
		uint32_t my_len;
		uint32_t my_locn;

};

#endif