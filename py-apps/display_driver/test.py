import time
import display_driver

# Parameters
out_strs = ["HashTest123", "MileyCyrus", "OMG", "Thisis", "test"]

# Declare character buffers
print out_strs[1]
out_chars = [   display_driver.new_charArray(len(out_strs[0])),
                display_driver.new_charArray(len(out_strs[1])),
                display_driver.new_charArray(len(out_strs[2])),
                display_driver.new_charArray(len(out_strs[3])),
                display_driver.new_charArray(len(out_strs[4])) ]

# Create display object
disp_p = display_driver.display_driver()

# Fill character buffers and send to SPI
for str_index, s in enumerate(out_strs):
    print 'String ', str_index, ': ', s
    for ch_index, c in enumerate(bytearray(s.upper())):
        display_driver.charArray_setitem(out_chars[str_index], ch_index, c)
        print ch_index, ' : ', c
    if not disp_p.update_string(str_index, out_chars[str_index]):
        print 'Update string failed'

# Create display object
# disp_p = display_driver.display_driver()

# t1 = display_driver.new_charArray(10)
# display_driver.delete_charArray(t1)

# Loop through each string
# for str_index, s in enumerate(out_strs):   

    # Define char array
#    out_chars = display_driver.new_charArray(len(s))
#    display_driver.delete_charArray(out_chars)

    # Fill char array
#    print 'String ', str_index, ': ', s
#    for i, c in enumerate(bytearray(s.upper())):
#        display_driver.charArray_setitem(out_chars,i,c)
#        print i, ' : ', c

    # Send out string
#    if not disp_p.update_string(str_index, out_chars):
#        print 'Update string failed'

#    print out_chars


# Sleep to let displays cycle
time.sleep(30)

# Cleanup
# display_driver.delete_charArray(out_chars)
