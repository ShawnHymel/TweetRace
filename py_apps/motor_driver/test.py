import time
import random
import motor_driver

NUM_HOSSES = 5

# Create a new hoss system (motor driver)
hp = motor_driver.hoss_system()

# Reset hosses
hp.find_home()
time.sleep(1)

# Race!
while not hp.is_any_at_far_end():
    for i in range(0, NUM_HOSSES):
        hp.set_race_value(i, int(random.random() * 5))
    hp.race()
    time.sleep(1)

# Determine winner
winner = hp.get_winner()
print 'Winner is', winner
time.sleep(5)
