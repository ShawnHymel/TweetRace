import spi_mover

out_chars = [97, 98, 99, 100, 101, 102, 103, 104, 105, 106]
in_chars = []

spi_out = spi_mover.new_uint8_tArray(len(out_chars))
spi_in = spi_mover.new_uint8_tArray(len(out_chars))

for i, c in enumerate(out_chars):
    spi_mover.uint8_tArray_setitem(spi_out,i,c)

s = spi_mover.spi_mover("/dev/spidev0.0")
if not s.transfer(10, spi_out, spi_in):
    print 'SPI transfer failed'

for i in range(0,9):
    in_chars.append(spi_mover.uint8_tArray_getitem(spi_in,i))
    print in_chars[i]
