import display_driver

# Parameters
out_strs = ["HashTest123", "MileyCyrusTrainWreck", "OMG", "This is", "A test"]

# Loop through each string
for str_index, s in enumerate(out_strs):   

    # Define char array
    out_chars = display_driver.new_charArray(len(s))

    # Fill char array
    print 'String ', str_index, ' : ', s
    for i, c in enumerate(bytearray(s.upper())):
        display_driver.charArray_setitem(out_chars,i,c)

    # Create display driver object and send out string
    disp_p = display_driver.display_driver()
    if not disp_p.update_string(str_index, out_chars):
        print 'Update string failed'

# Cleanup
# display_driver.delete_charArray(out_chars)
