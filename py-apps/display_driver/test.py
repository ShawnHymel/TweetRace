import time
import display_driver

# Parameters
out_strs = ["HashTest123", "MileyCyrus", "OMG", "Thisis", "test"]

# Create display object
disp_p = display_driver.display_driver()

# Loop through each string
for str_index, s in enumerate(out_strs):   

    # Define char array
    out_chars = display_driver.new_charArray(len(s))

    # Fill char array
    print 'String ', str_index, ': ', s
    for i, c in enumerate(bytearray(s.upper())):
        display_driver.charArray_setitem(out_chars,i,c)
        print i, ' : ', c

    # Send out string
    if not disp_p.update_string(str_index, out_chars):
        print 'Update string failed'

    print out_chars
    display_driver.delete_charArray(out_chars)

# Sleep to let displays cycle
time.sleep(30)

# Cleanup
# display_driver.delete_charArray(out_chars)
