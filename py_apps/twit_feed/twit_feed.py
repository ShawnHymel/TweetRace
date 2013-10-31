import Queue
import threading
from twython import TwythonStreamer

# Twitter streamer class tallies tweets based on given hashtags
class TwitStreamer(TwythonStreamer):

    # [Constructor] Inherits a Twython streamer. Pass in queues.
    def __init__(self, parent, app_key, app_secret, oauth_token, 
                    oauth_token_secret, timeout=300, retry_count=None, 
                    retry_in=10, client_args=None, handlers=None, 
                    chunk_size=1):
        TwythonStreamer.__init__(self, app_key, app_secret, oauth_token, 
                    oauth_token_secret, timeout=300, retry_count=None, 
                    retry_in=10, client_args=None, handlers=None, 
                    chunk_size=1)
        self.parent = parent
        self.score_terms = self.parent.get_score_terms()

    # Callback from streamer when tweet matches at least one search term
    def on_success(self, data):
        if 'text' in data:
            msg = data['text'].encode('utf-8')
            self.parent.add_tweet(msg)
            for hashtag in data['entities']['hashtags']:
                for term in self.score_terms:
                    if term == hashtag['text'].lower():
                        self.parent.add_score(self.score_terms.index(term))         

    # Callback from streamer if error occurs
    def on_error(self, status_code, data):
        print status_code, data
        
    # Called from main thread to stop the streamer
    def stop(self):
        self.disconnect()

# TwitFeed class sets up the streamer and provides access to tweets and score
class TwitFeed:

    # [Constructor] Setup streamer thread
    def __init__(self, twitter_auth, search_terms):

        # Setup Queues
        self.tweet_queue = Queue.Queue()
        self.score_queue = Queue.Queue()

        # Setup tracking and scoring term arrays
        self.search_terms = search_terms
        self.track_terms = [''.join([x,' ']) for x in search_terms]
        self.score_terms = [x[1:].lower() for x in search_terms]

        # Create thread to filter the stream
        app_key = twitter_auth['app_key']
        app_secret = twitter_auth['app_secret']
        oauth_token = twitter_auth['oauth_token']
        oauth_token_secret = twitter_auth['oauth_token_secret']
        auth_args = (app_key, app_secret, oauth_token, oauth_token_secret)
        self.thread_stream = threading.Thread( \
                        target=self.__create_twitter_streamer, args=auth_args)
        self.thread_stream.daemon = True
        self.thread_stream.start()

    # [Private] Setup streamer and filter(s)
    def __create_twitter_streamer(  self, 
                                    app_key, 
                                    app_secret, 
                                    oauth_token, 
                                    oauth_token_secret ):
        self.stream = TwitStreamer( self, 
                                    app_key, 
                                    app_secret,
                                    oauth_token,
                                    oauth_token_secret )
        self.stream.statuses.filter(track=self.track_terms)

    # [Public] Adds a score to the queue
    def add_score(self, score):
        self.score_queue.put(score)

    # [Public] Adds a tweet to the queue
    def add_tweet(self, tweet):
        self.tweet_queue.put(tweet)

    # [Public] Get score terms
    def get_score_terms(self):
        return self.score_terms

    # [Public] Tally score
    def tally_tweets(self): 
        
        score_card = [0] * len(self.search_terms)

        # Add delimiter to the queue
        self.score_queue.put(-1)

        # Tally up the scores from the queue
        while not self.score_queue.empty():
            score_point = self.score_queue.get()
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

    # [Public] Get Tweets
    def get_tweets(self):

        tweet_list = []

        # Add delimiter to the queue
        self.tweet_queue.put(-1)

        # Pull tweets from queue and add to the list
        while not self.tweet_queue.empty():
            msg = self.tweet_queue.get()
            if msg == -1:
                break
            else:
                tweet_list.append(msg)
        return tweet_list
        
    # [Public] Stop the streamer and wait for thread to end
    def stop(self):
        self.stream.stop()
        while self.thread_stream.is_alive():
            pass
