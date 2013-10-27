%module spi_mover
%include "stdint.i"
%include "cpointer.i"
%include "carrays.i"
%pointer_functions(uint8_t, uint8_tp)
%array_functions(uint8_t, uint8_tArray)
%{
    #include "spi_mover.h"
%}
%include "spi_mover.h"
