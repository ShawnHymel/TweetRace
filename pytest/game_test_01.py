import time
import Queue
import threading
from twython import TwythonStreamer

# Game parameters
terms = ['#omg', '#hello', '#diy', '#robot', '#fun']    # Search terms
game_time = 10                                          # Game time (seconds)

# Twitter authentication
APP_KEY = '0SGK64SYL8Y0R4tCbcm6SA'
APP_SECRET = 'f1RFySmg0RVRVtTGnvu9NvrWm73IT7C8Z1gpcYZAwo' 
OAUTH_TOKEN = '1969690717-lDn8UEoAIp5h2zITLyPtfGjY8EGhazHTzUVQBvH'
OAUTH_TOKEN_SECRET = 'gv9vGWluxgMtMY77evxainwolbFhxIiCJsGTd79N2I'

# Setup Queues
tweet_queue = Queue.Queue()
score_queue = Queue.Queue()

# Twitter streamer class
class MyStreamer(TwythonStreamer):
    def on_success(self, data):
        if 'text' in data:
            msg = data['text'].encode('utf-8')
            tweet_queue.put(msg)
            for hashtag in data['entities']['hashtags']:
                for term in score_terms:
                    if term == hashtag['text'].lower():
                        score_queue.put(score_terms.index(term))         

    def on_error(self, status_code, data):
        print status_code, data

# Tally score
def tally_tweets():
    
    score_card = [0] * len(terms)

    # Add delimiter to the queue
    score_queue.put(-1)

    # Tally up the scores from the queue
    while not score_queue.empty():
        score_point = score_queue.get()
        if score_point == -1:
            break
        else:
            score_card[score_point] += 1

    # Algorithm for determining move values
    prev = 0
    pts = 0
    move_card = [0] * len(score_card)
    for c in sorted(score_card):
        if c != prev:
            pts += 1
        i = score_card.index(c)
        score_card[i] = -1
        move_card[i] = pts
        prev = c
    return move_card

# Get Tweets
def get_tweets():

    tweet_list = []

    # Add delimiter to the queue
    tweet_queue.put(-1)

    # Pull tweets from queue and add to the list
    while not tweet_queue.empty():
        msg = tweet_queue.get()
        if msg == -1:
            break
        else:
            tweet_list.append(msg)
    return tweet_list
        


# Setup tracking and scoring term arrays
track_terms = [''.join([x,' ']) for x in terms]
score_terms = [x[1:].lower() for x in terms]

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
print 'START OF GAME!'
time.sleep(game_time)
print 'END OF GAME!'
tweet_list = get_tweets()
for m in tweet_list:
    print m, '\n'
score_board = tally_tweets()
print score_board



