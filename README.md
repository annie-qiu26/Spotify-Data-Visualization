# Spotify Visualization
## Building
This application uses [openFrameworks](http://openframeworks.cc/download/), and
after cloning this repository, build it with their given project generator. You need to
include these two addons:<br/>
* [ofxGrafica](https://github.com/jagracar/ofxGrafica)
* [ofxDatGui](https://github.com/braitsch/ofxDatGui)<br/>
Follow their instructions on how to install them.<br/>
To install JsonCpp, run the following line of code in the `jsoncpp/jsoncpp/json/`
folder:<br/>
`python amalgamate.py`

## Tweaks in Addons
Theses files **must** __strong__ be changed for the code to compile:
In *ofxDatGui* _emphasize_, make the following tweaks:<br/>
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
In *ofxGrafica* _emphasize_, make the following tweaks:<br/>
* For `ofxGLayer.cpp` in the `addons\ofxGrafica\src\` folder, change:<br/>
`209: distSq < max(pow(pointSizes[i % pointSizes.size()], 2), 25.0)` to <br/>
`209: distSq < pow(pointSizes[i % pointSizes.size()], 2) || distSq < 25.0`
* For `ofxGPlot.cpp` in the `addons\ofxGrafica\src\` folder, change:<br/>
`310: lim = {0.1, 10};
365: lim = {0.1, 10};` to <br/>
`310: lim = {0.1f, 10};
365: lim = {0.1f, 10};`<br/>


## Tests
