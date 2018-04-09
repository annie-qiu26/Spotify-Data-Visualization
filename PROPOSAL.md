# Project Proposal
## General Summary
I'm planning to use Spotify's API to find data on top artists played and top
artists played based on user. I'm also taking into account the popularity of
each of their songs and albums. Given this data, I want to use openFrameworks
to display the data in a visually appeasing way. Users can search for playlists
and the application will summarize the type of music and popularity of the
artists. Then I want to artistically express the data with an interactive feature
for users. They can analyze their data and click on an artist "bubble" to listen
to one of their songs and get more statistics about them. To play songs on the
application, users will need to authenticate their accounts. Otherwise, I can
include popular artists' songs as .mp3 files in the data folder that can play
as well.

## Libraries
I'll be using openFrameworks for the interactive windowed application that
allows users to understand their data and other music data from Spotify. To get
this information I'm planning to use Spotify's API and C++ Rest SDK to make HTTP
requests and get JSON data from Spotify. Then I'll use JsonCpp to parse the data
and get the relevant information such as artist names, album/song names, popularity,
and other data. Using openFrameworks, I'll output the information in proportionally
sized colored artist "bubbles", which will basically just float around the screen
and the size will be based on an algorithm of popularity relative to the playlist
or in general.

## Background
I've looked in Spotify's API before, but I was using Python to parse through
the JSON to get artist and song data. Other than that, this project will involve
a lot of learning how to use API's and how to read documentation of the libraries
that I want to use.
