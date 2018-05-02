# Development Logs
## Integrating Libraries, Parsing JSON data : 04/15/18
For the past two days, I've been trying to authenticate Spotify with HTTP request,
so far I haven't been able to accomplish this because the
[C++ Rest SDK's](https://github.com/Microsoft/cpprestsdk) documentation isn't
very specific on how to use their library. I was able to integrate this library
as well as [JsonCpp](https://github.com/open-source-parsers/jsoncpp) and
openFrameworks. I've been using [Spotify's API's](https://developer.spotify.com/web-api/)
web console to get JSON data that I can parse. While I'm still trying to figure
out how to make HTTP requests in C++, my next goal is to aggregate all the data
from the sample JSON files that I temporarily added to the repository.

## Standardizing Data : 04/18/18
I've been working on functions that will help me standardize the data to have a
mean of 0 and standard deviation of 0, but some of the functions weren't working
properly. I still need to create more tests for these functions and understand
what's happening with my standardized data function for it to compile.

## OpenFrameworks Addons, Visualizing Data : 04/21/18
I added tests for standardizing data and fixed some bugs in those functions. I also
temporarily got data from Spotfiy's web console and parsed the JSON data to get
id and audio features of tracks from my Liked and Disliked Playlist. Then I played
around with two addons for openFrameworks ([ofxDatGui](https://braitsch.github.io/ofxDatGui/)
and [ofxGrafica](https://github.com/jagracar/ofxGrafica)]. I plotted my data
with a histogram and added a textfield to get user input.

## Creating Histograms, Standard Deviation Issue : 04/24/18
Right now, I'm working on creating histograms with my sample datasets of my liked and
disliked songs. I added overlapping layers to show the differences between the two sets.
I realized that my standard deviation has overflow problems, so I need to fix that. I'll
probably continue working on the histograms for the next couple days to make it look
nice on the openFrameworks application.

## Working with the API using Postman : 04/26/18
Instead of using the C++ Rest SDK or libcurl library, I decided to use Postman and have the
user go to a link that has my collection of requests to give me access to their playlists and
tracks' ids. Then I'll have the user place the JSON data into the corresponding text file
called liked_songs_features.json and disliked_songs_features.json. I'll give instructions on this
in the README.md and in the application.

## Drawing Screens in Application : 04/29/18
Currently, I'm working on the graphics of the openFrameworks application. I'm laying out the four
different screens people will see, which are the start, instruction, histogram, and prediction
screens. I'm using the Montserrat font, which looks like Spotify's font, and then mimicking their
colors as well. I've added buttons to help with navigation.

## Finished : 05/01/18
I've written all the instructions in how to use Postman to make HTTP requests with Spotify's API. Then,
I've also written instructions on how to get track's features that can be used in the prediction
screens. I've added sliders too for a user-friendly way to input the features. They can also type in
the numbers as well. To finish things off, I just need to update the README.md to inform the user
the tools they need to build the project.
