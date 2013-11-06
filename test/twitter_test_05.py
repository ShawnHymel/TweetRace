# Twitter API Test
# Execute: $ python twitter_test_02.py

import time
import Queue
import threading
from twython import TwythonStreamer

# Game parameters
terms = ['#omg', '#iphone'] # Search terms (hashtags only right now)
game_time = 30                       # Game time (seconds)

# Twitter authentication
APP_KEY = 'li8wn8Tb7xBifCnNIgyqUw'
APP_SECRET = 'vcwq36w4C4VXamlqWBDKM2E8etsOoangDoMhxNDU'
OAUTH_TOKEN = '1969690717-rGw3VkRQ8IyL4OcPWtv5Y2CeBdVn8ndJrjGKraI'
OAUTH_TOKEN_SECRET = 'KO7YIFMKWKaYTtz2zEyaSy044ixj5kIbWrDtZZL96ly0H'

# Twitter streamer class
class MyStreamer(TwythonStreamer):
    def on_success(self, data):
        if 'text' in data:
            msg = data['text'].encode('utf-8')
            print msg
            for hashtag in data['entities']['hashtags']:
                for term in score_terms:
                    if term == hashtag['text'].lower():
                        print term, ' : ', score_terms.index(term)
                        i = score_terms.index(term)
                        scoreboard[1][i] += 1
            print scoreboard            
            print '\n'

    def on_error(self, status_code, data):
        print status_code, data

# Setup tracking and scoring term arrays
track_terms = [''.join([x,' ']) for x in terms]
print 'Tracking: ', track_terms
score_terms = [x[1:].lower() for x in terms]
print 'Scoring: ', score_terms

# Setup scoreboard
scoreboard = [terms, [0]*len(terms)]
print 'Scoreboard: ', scoreboard
print '\n'

# Setup streamer and filter(s)
def create_twitter_streamer():
    stream = MyStreamer(APP_KEY, APP_SECRET,
                        OAUTH_TOKEN, OAUTH_TOKEN_SECRET)
    stream.statuses.filter(track=track_terms)

# Create thread to filter the stream
thread_stream = threading.Thread(target=create_twitter_streamer, args=())
thread_stream.daemon = True
thread_stream.start()

# Wait for game to end
time.sleep(game_time)
print 'END OF GAME! ', scoreboard



