import Queue
import threading
from twython import Twython, TwythonStreamer, TwythonError

# Twitter streamer class to collect bet entries for the race
class BetStreamer(TwythonStreamer):

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

    # Callback from streamer when tweet matches the search term
    def on_success(self, data):
        if 'text' in data:
            print data['text'].encode('utf-8') #***TEST
        #    for hashtag in data['entities']['hashtags']:
        #        self.parent.add_entry(hashtag['text'].lower())
        #        break # only add first term

    # Callback from streamer if error occurs
    def on_error(self, status_code, data):
        print status_code, data
        
    # Called from main thread to stop the streamer
    def stop(self):
        self.disconnect()

# TwitFeed class sets up the streamer and provides access to tweets and score
class TwitFeed:

    # [Constructor] Setup streamer thread
    def __init__(self, twitter_auth):

        # Setup Queues
        self.tweet_queue = Queue.Queue()
        self.score_queue = Queue.Queue()
        self.entry_queue = Queue.Queue()

        # Extract authentication tokens
        app_key = twitter_auth['app_key']
        app_secret = twitter_auth['app_secret']
        oauth_token = twitter_auth['oauth_token']
        oauth_token_secret = twitter_auth['oauth_token_secret']
        self.auth_args = (  app_key, 
                            app_secret, 
                            oauth_token, 
                            oauth_token_secret)
    
        # Setup Twitter object to send tweets
        self.twitter = Twython( app_key, 
                                app_secret, 
                                oauth_token, 
                                oauth_token_secret)

    # [Private] Setup bet streamer
    def __create_bet_streamer(  self, 
                                app_key, 
                                app_secret, 
                                oauth_token, 
                                oauth_token_secret ):
        self.bet_stream = BetStreamer(  self, 
                                    app_key, 
                                    app_secret,
                                    oauth_token,
                                    oauth_token_secret )
        self.bet_stream.statuses.filter(track=['#blargz'])#self.bet_term)

    # [Public] Start bet streamer
    def start_bet_streamer(self, bet_term):

        # Setup term needed to make an entry
        self.bet_term = bet_term

        # Start streamer inside thread
        self.thread_betting = threading.Thread( \
                target=self.__create_bet_streamer, args=self.auth_args)
        self.thread_betting.daemon = True
        self.thread_betting.start()

    # [Public] Adds a bet entry to the queue
    def add_entry(self, entry):
        self.entry_queue.put(entry)

    # [Public] Get entries to the race
    def get_entries(self):

        entry_list = []

        # Add delimiter to the queue
        self.entry_queue.put(-1)

        # Pull entries from queue and add to the list
        while not self.entry_queue.empty():
            entry = self.entry_queue.get()
            if entry == -1:
                break
            else:
                entry_list.append(entry)
        return entry_list

    # [Public] Stop betting and wait for thread to end
    def stop_betting(self):
        self.bet_stream.stop()
        print 'Stopping thread'
        #self.thread_betting.join()        
        while self.thread_betting.is_alive():
            pass
        del self.thread_betting
