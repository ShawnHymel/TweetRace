TweetRace
=========

The Great American Tweet Race

Required Packages
-----------------

```
sudo apt-get update
sudo apt-get install g++
sudo apt-get install python-dev
sudo apt-get install python-pygame
sudo apt-get install python-pip
sudo apt-get install swig
sudo pip install twython
```

Generate Twitter App Token
--------------------------

 - Log in to Twitter
 - Go to: https://dev.twitter.com/apps
 - User icon -> My Applications
 - Click "Create a new application" and fill out stuff
 - Once done, click "Create my access token"

How To Build
------------

```
sh build.sh
```

How To Run
----------

 - Fill out Twitter authentication in param.txt
 - Fill out hashtags in hashtags.txt

```
sudo python TweetRace.py
```
