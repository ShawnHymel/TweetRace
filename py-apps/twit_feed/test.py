# Test for twit_feed.py
# Call with 'python test.py'

import time
import twit_feed

# Game parameters
terms = ['#omg', '#hello', '#diy', '#robot', '#fun']    # Search terms
game_time = 10                                          # Game time (seconds)

# Twitter authentication
twitter_auth = {    'app_key': '0SGK64SYL8Y0R4tCbcm6SA',
                    'app_secret': 'f1RFySmg0RVRVtTGnvu9NvrWm73IT7C8Z1gpcYZAwo',
                    'oauth_token': '1969690717-lDn8UEoAIp5h2zITLyPtfGjY8EGhazHTzUVQBvH',
                    'oauth_token_secret': 'gv9vGWluxgMtMY77evxainwolbFhxIiCJsGTd79N2I' }

# Create a TwitFeed object
tf = twit_feed.TwitFeed(twitter_auth, terms)

# Wait for game to end and display tweets/score
print 'START OF GAME!'
time.sleep(game_time)
tweet_list = tf.get_tweets()
for m in tweet_list:
    print m, '\n'
print 'END OF GAME!'
print tf.tally_tweets()

# Try another game
print 'START OF GAME!'
time.sleep(game_time)
tweet_list = tf.get_tweets()
for m in tweet_list:
    print m, '\n'
print 'END OF GAME!'
print tf.tally_tweets()
