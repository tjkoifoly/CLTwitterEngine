CLTwitterEngine
===============

A simple, ARC and blocks-based Twitter engine for Cocoa and CocoaTouch.  `CLTweetEngine` currently supports both [TweetMarker][tm] and Twitter's image upload service.

Basics
======

`CLTwitterEngine` is designed with the following tenets:

* Calls to and from Twitter should be asynchronous.
* Callbacks will favor blocks, rather than protocols and delegates.
* Clients of the library should be able to provide their own means of signing requests with OAuth information.
* Clients of the library should be able to provide a JSON parser of their choice.  This is to allow use on older versions of Mac OS X and iOS, which do not have [NSJSONSerialization][JSON].
* Objects should know how to get or post themselves, whenever possible.  For example, the `CLTweet` class is capable of getting a tweet or posting one. 
* The API should be as simple as possible.
* Yes, there are [other frameworks][mg], but sometimes you just want to roll your own.  For the hell of it.

API Coverage
============
At some point, the entire Twitter API may be covered.  However, the below are the API calls that are intended to be implemented.

### Timeline 
☑ Get home timeline<br />
☐ Get mentions<br />
☐ Get public timeline<br />
☐ Get retweets of me<br />
☐ Get user timeline<br />

### Tweets
☐ Get users whom retweeted a tweet<br />
☐ Get retweets of a tweet<br />
☑ Get single tweet<br />
☑ Delete tweet<br />
☐ Post retweet of a tweet<br />
☑ Post tweet<br />
☑ Post tweet with an image<br />

### Search
☑ Get search results<br />

### Saved searches
☑ Get saved searches<br />
☑ Get single saved search<br />
☑ Create a saved search<br />
☑ Delete a saved search<br />

### Direct Messages
☑ Get messages to me<br />
☑ Get messages by me<br />
☐ Delete a message<br />
☐ Post a message<br />
☑ Get single message<br />

### Followers
☑ Get my followers<br />
☑ Get my followees<br />
☐ Get incoming follow requests<br />
☐ Get outgoing follow requests<br />
☐ Follow a user<br />
☐ Unfollow a user<br />

### Users
☑ Get users in bulk<br />
☐ Get a user's profile image<br />
☐ Get search results for a user search<br />
☑ Get a user<br />

### Favorites
☐ Get favorites<br />
☐ Create a favorite<br />
☐ Remove a favorite<br />

### Account
☐ Get rate limit status<br />
☐ Post update to account<br />
☐ Post update of profile image<br />
☐ Get account totals<br />

### Block
☐ Get all my blocked users<br />
☐ Get information about one block<br />
☐ Create a new block<br />
☐ Unblock a user<br />

### Spam
☐ Report spam<br />

### Help
☑ Get configuration<br />


Requirements
============
Requires [GTMHttpFetcher][fetcher].

Because you are providing `CLTwitterEngine` with a block of code that will sign OAuth requests (see below), to sign into Twitter using OAuth is entirely your responsibility.  There are frameworks which will handle this exchange for you; `CLTwitterEngine` used [GTMOauthAuthentication][OAuth] during development.

Initialization
==============

To initialize: 

* You must specify a way to convert JSON data to Foundation objects; the assumed way of doing so is using [NSJSONSerialization][JSON], though presumably any framework will do, as long as `NSDictionary`s and `NSArray`s are returned where appropriate.
* You must specify a way to add OAuth authorization to a `NSMutableURLRequest`.  Again, any framework should work, though GTMOauthAuthentication was used during development.

Example:
    
    [[CLTwitterEngine sharedEngine] setAuthorizer:^(NSMutableURLRequest *request)
    {
        // Your authorizing code here
    }];
    [[CLTwitterEngine sharedEngine] setConverter:^(NSData *data)
    {
        // Your code here
    }];

Sample Initialization
=====================

For illustrative purposes, here are the authorizer and converter that are being used to test `CLTwitterEngine`:

    [[CLTwitterEngine sharedEngine] setAuthorizer:^(NSMutableURLRequest *request)
     {
         // ivar: GTMOAuthAuthentication *_auth;
         [_auth authorizeRequest:request];
     }];
    [[CLTwitterEngine sharedEngine] setConverter:^(NSData *data)
    {
        NSError *error;
        return [NSJSONSerialization JSONObjectWithData:data 
                                               options:kNilOptions
                                                 error:&error];
    }];
    
Optional Step: Set up for Network Access Callbacks
======================================================
Optionally, you can set up a handler for when network access begins or ceases.  This is using the
class `CLNetworkUsageController`.  For example:

    [[CLNetworkUsageController sharedController] setCallback:^(BOOL isAccessing)
     {
         NSLog(@"%@ using network.", isAccessing ? @"BEGIN" : @"END"); 
     }];
    
Use
===
    
Subsequent to initialization above, the engine is ready to use.  You can get the timeline by:

    [[CLTwitterEngine sharedEngine] getTimeLineWithCompletionHandler:^(NSArray *timeline, NSError *error) {
        if (error)
        {
            // Handle the error.
        }
        else
        {
            // The timeline is ready for use.  It is a NSArray of CLTweet objects.
        }
    }];
    
##Posting Tweets
    
You can post a text-only tweet:

    [CLTweet postTweet:@"Look at me; I'm tweeting!" completionHandler:^(CLTweet *tweet, NSError *error) {
        if (error)
        {
            // Handle the error.
        }
        else
        {
            // Your tweet has been posted; the result is provided for convenience.
        }
    }];
    
You can post a tweet with an image:

    [CLTweet postTweet:@"Some clever message." withImage:/*Some_nifty_NSImage*/ completionHandler:^(CLTweet *tweet, NSError *error) {
        if (error)
        {
            // Handle the error.
        }
        else
        {
            // The tweet and image have been posted; the result is provided for convenience.
        }
    }];

##Direct Messages
    
You can get recent direct messages (both sent and received are in this same array):

    [[CLTwitterEngine sharedEngine] getRecentDirectMessagesWithCompletionHandler:^(NSArray *messages, NSError *error) {
        if (error)
        {
            // Hanlde the error.
        }
        else
        {
            // The direct messages (sent and received) are ready for use.
            // It is a NSArray of CLDirectMessage objects.
        }
    }];
    
Or perhaps just an individual direct message:

    [CLDirectMessage getDirectMessageWithId:[NSNumber numberWithLongLong:123456789012341234] 
                          completionHandler:^(CLDirectMessage *message, NSError *error) {
        if (error)
        {
            // Handle the error.
        }
        else
        {
            // The direct messages is provided.
        }
    }];

##Users

You can get a user by handle/screen name:

    [CLTwitterUser getUserWithScreenName:@"SedgeApp" completionHandler:^(CLTwitterUser *user, NSError *error) {
        if (error)
        {
            // Handle error.
        }
        else
        {
            // The user is provided.
        }
    }
    
...or by ID:

    [CLTwitterUser getUserWithId:[NSNumber numberWithLongLong:123456789012341234] completionHandler:^(CLTwitterUser *user, NSError *error) {
                if (error)
                {
                    // Handle error.
                }
                else
                {
                    // The user is provided.
                }
    }];
    
##Search

Both regular searches and saved searches are supported.

###Regular Searches

To perform a search:

    [CLTwitterSearch beginSearchWithQuery:@"some_search_query" completionHandler:^(CLTwitterSearch *searchResult, NSError *error) {
        if (error)
        {
            // Handle error.
        }
        else
        {
            // Search result object is provided.  The actual results are in [searchResult results].
        }
        
Once you have a `CLTwitterSearch` object, you can use it to get the next/older batch of results:

    [searchResult getOlderSearchResultsWithCompletionHandler:^(CLTwitterSearch *searchResult, NSError *innerError) {
        if (error)
        {
            // Handle error.
        }
        else
        {
            // Now you have a new search result, which ostensibly could replace the one you have.
        }
        
Similarly, you can go the opposite direction and ask for new results since your last search:

    [searchResult getNewerSearchResultsWithCompletionHandler:^(CLTwitterSearch *searchResult, NSError *error) {
        if (error)
        {
            // Handle error.
        }
        else
        {
            // Now you have a new search result, which ostensibly could replace the one you have.
        }

###Saved Searches
    
Saved searches are also handled.  To get all of a user's saved searches:

    [CLTwitterSavedSearch getSavedSearchTermsWithHandler:^(NSArray *searches, NSError *error) {
        if (error)
        {
            // Handle error.
        }
        else
        {
            // searches is an array of CLTwitterSavedSearch objects.
        }
    }];

To get an individual saved search:

    [CLTwitterSavedSearch getSavedSearchWithId:[NSNumber numberWithLong:12345678]
                             completionHandler:^(CLTwitterSavedSearch *search, NSError *error) {
                                 if (error)
                                 {
                                     // Handle error.
                                 }
                                 else
                                 {
                                     // Saved search retrieved.
                                 }
                             }];

To create a new saved search:

    [CLTwitterSavedSearch createSavedSearchWithQuery:@"Testing" completionHandler:^(CLTwitterSavedSearch *search, NSError *error) {
        if (error)
        {
            // Handle error.
        }
        else
        {
            // Newly created saved search provided for convenience.
        }
    }];
    
To delete a saved search:

    // Local variable: CLTwitterSavedSearch *search
    [search deleteWithErrorHandler:^(NSError *innerError) {
        if (error)
        {
            // Handle error.
        }
    }];
    
##Configuration

To get the current Twitter configuration:

    [CLTwitterConfiguration getTwitterConfigurationWithHandler:^(CLTwitterConfiguration *configuration, NSError *error) {
        if (error)
        {
            // Handle error.
        }
        else 
        {
            // Configuration is provided.  Note:
            // [configuration photoSizes] is a dictionary of CLTwitterPhotoSizes
            // [configuration nonUsernamePaths] is an array of NSStrings.
        }
    }];


TweetMarker
===========

[Tweet Marker][tm] is, in short, a way for one or more Twitter clients to all cooperate and note a user's last read tweet.  It is opt-in, and requires its own API key–see [the website][tm] for details.

Tweet Marker allows several different "collections" to have marks.  For example, "timeline", or "mentions".  With that in mind...

To retrieve the current marker in the timeline for the user [SedgeApp][s]:

    [CLTweetMarker getLastReadForUsername:@"sedgeapp" 
                             inCollection:@"timeline" 
                               withApiKey:@">>>tweetmarker api key<<<" 
                        completionHandler:^(NSNumber *tweetId, NSError *error) {
                            if (error == nil)
                            {
                                // Handle error.
                            }
                            else
                            {
                                // Scroll your view to the tweet with ID == tweetId.
                            }
    }];

To set the current marker in the timeline:

    [CLTweetMarker markLastReadAsTweet:[NSNumber numberWithLongLong:168739424295854082]
                           forUsername:@"sedgeapp"
                          inCollection:@"timeline"
                            withApiKey:@">>>tweetmarker api key<<<"
                     completionHandler:^(BOOL success, NSError *error) {
                         if (!success)
                         {
                             // Handle error.
                         }
                         else
                         {
                             // The marker has been set.  Optionally, update your UI to show that.
                         }
                     }];
    
    

[fetcher]: http://code.google.com/p/gtm-http-fetcher/
[JSON]: https://developer.apple.com/library/mac/#documentation/Foundation/Reference/NSJSONSerialization_Class/Reference/Reference.html
[OAuth]: http://code.google.com/p/gtm-oauth/
[tm]: http://www.tweetmarker.net/
[s]: http://twitter.com/SedgeApp/
[mg]: https://github.com/mattgemmell/MGTwitterEngine