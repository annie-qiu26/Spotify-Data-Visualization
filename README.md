# Spotify Visualization
## Introduction
This application is going to need data from your Spotify account. First, you'll
need to authenticate it, so Postman can make requests to the API. Instructions
are found in the app. You're going to need two playlists, one with songs you like, and
another with songs you dislike. When you get the JSON data, put them in the
`liked_songs_features.json` and `disliked_songs_features.json` files in the `data\`
folder.

## Building
This application uses [openFrameworks](http://openframeworks.cc/download/), and
after cloning this repository, build it with the given project generator. You need to
include these two addons:<br/>
* [ofxGrafica](https://github.com/jagracar/ofxGrafica)
* [ofxDatGui](https://github.com/braitsch/ofxDatGui)<br/>

Follow their instructions on how to install them.<br/>

To install JsonCpp, run the following line of code in the `jsoncpp/jsoncpp/json/`
folder:<br/>
```
python amalgamate.py
```

## Tweaks in Addons
Theses files **must** be changed for the code to compile:<br/>

In *ofxDatGui*, make the following tweaks:<br/>
* For `ofxDatGuiSlider.h` in the `addons\ofxDatGui\src\components` folder, add in the
following piece of code:<br/>
`void setSliderFillColor(const ofColor color) {
	mSliderFill = color;
}
`<br/>
* For `ofxDatGuiTextInput.h` in the `addons\ofxDatGui\src\components` folder, add in the
following piece of code:<br/>
`ofxDatGuiTextInputField* getTextInput() {
	return &mInput;
}
`<br/>

These files might need to be modified for Visual Studio to compile:<br/>

In *ofxGrafica*, make the following tweaks:<br/>
* For `ofxGLayer.cpp` in the `addons\ofxGrafica\src\` folder, change:<br/>
`line 209: distSq < max(pow(pointSizes[i % pointSizes.size()], 2), 25.0)` to <br/>
`line 209: distSq < pow(pointSizes[i % pointSizes.size()], 2) || distSq < 25.0`
* For `ofxGPlot.cpp` in the `addons\ofxGrafica\src\` folder, implement these changes:<br/>

*Original*
```
line 310: `lim = {0.1, 10};<br/>
line 365: `lim = {0.1, 10};
```
*Modified*
```
line 310: `lim = {0.1f, 10};<br/>
line 365: `lim = {0.1f, 10};
```

## Other Tools
You're also going to need to install Postman as an app or as an extension on Google
Chrome, so you can import a collection, which can be found in this [link](https://www.getpostman.com/collections/47dc5dade17a715c89b2).


## Tests
To build the tests, run the following lines in the `test/` folder:<br/>
```
make audio_test
./audio_test
```
```
make top_tracks_test
./top_tracks_test
```
```
make predictor_test
./predictor_test
```

To remove the files created from building the tests, run:<br/>
```
make clean
```
