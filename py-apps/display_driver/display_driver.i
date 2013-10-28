%module display_driver
%include "stdint.i"
%include "carrays.i"
%array_functions(char, charArray)
%{
    #include "../../c-apps/display-driver/display_driver.h"
%}
%include "../../c-apps/display-driver/display_driver.h"
