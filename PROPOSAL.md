# Project Proposal
## General Summary
I'm planning to use Spotify's API to find data on top tracks played based on user.
I also want users to give me a playlist of songs they don't like, so
I can compare the different styles of songs they prefer. Given this data, 
I want to use openFrameworks to display the data in a visually appeasing way. Users can search for playlists
and the application will summarize the type of music and the audio feautres of
their songs. Then I want to artistically express the data with an interactive feature
for users. They can analyze their data and then input another set of audio features
of an unknown track, and my application will predict whether they'll like the song
or not.

## Libraries
I'll be using openFrameworks for the interactive windowed application that
allows users to understand their data and other music data from Spotify. To get
this information I'm planning to use [Spotify's API](https://developer.spotify.com/web-api/)
and Postman to simulate HTTP requests for the user
and get JSON data from Spotify. Then I'll use [JsonCpp](https://github.com/open-source-parsers/jsoncpp)
to parse the data and get the relevant information such as artist names, album/song names, popularity,
and other data. Using openFrameworks, I'll output the information in a set of
histograms, which users can navigate with left and right keys.

## Background
I've looked in Spotify's API before, but I was using Python to parse through
the JSON to get artist and song data. Parsing JSON will involve similar techniques
used in the CourseGrades assignment. Other than these past backgrounds, this
project will involve a lot of learning how to use API's and how to read
documentation of the libraries that I want to use.

## Extra Addons
If I have time, I hope I can implement a search feature for users that will allow
them to specify a specific artist and get more information about them with
background music tuned to their music taste. I also want to add in a feature
where all their favorite artists are from in an interactive map on the openFrameworks
application. Lastly, I hope I can visualize sound waves based on the type of music
for an extra meditative feature.

## Changes
Instead of looking at data about the artist, I decided to use information about the tracks
because I found that each track has a lot of cool features that describe it such as
"Danceability" or "Instrumentalness". Then I want users to understand more about their
music preferences, so I showed them the differences between their liked songs' features and
disliked songs' features in histograms. I also decided to add in a prediction feature, which takes
in their playlists' data and figure out whether the user will like a song or not based on the
given features.
