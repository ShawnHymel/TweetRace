import display_driver

# Parameters
spidev = "/dev/spidev0.0"
out_str = "HashTest123"
index = 0

# Define char array
out_chars = display_driver.new_charArray(len(out_str))

# Fill char array
for i, c in enumerate(bytearray(out_str.upper())):
    print i, ' : ', c
    display_driver.charArray_setitem(out_chars,i,c)

# Create display driver object and send out string
disp_p = display_driver.display_driver()
if not disp_p.update_string(index, out_chars):
    print 'Update string failed'

# Cleanup
# display_driver.delete_charArray(out_chars)
