#pragma once

#include "ofMain.h"
#include "ofxDatGui.h"
#include "ofxGrafica.h"
#include "top_tracks.h"
#include "track_predictor.h"

class ofApp : public ofBaseApp{

public:
	void setup();
	void update();
	void draw();

	// Navigation
	void keyPressed(int key);

	// Setting up functions
	void setupColors();
	void setupHistograms();
	void setupGUI();
	void setupTitles(vector<vector<pair<string, double>>> dataset);
	void setupBounds(vector<double> means, vector<double> stds);
	void setupButtons();
	void setupSliders(vector<double> means, vector<double> stds);
	void setupResultMessages();

	// Function used for analyzing data
	vector<vector<pair<string, double>>> standardizeDataset();
	vector<vector<ofxGPoint>> calculateHistograms(vector<vector<pair<string, double>>> dataset,
		int start_limit, int end_limit, vector<double> means, vector<double> stds);

	// Update functions that change information on screen
	void histogramUpdate();
	void resultUpdate(vector<double> means, vector<double> stds);

	// GUI Functions
	void onButtonEvent(ofxDatGuiButtonEvent e);
	void onTextInputEvent(ofxDatGuiTextInputEvent e);
	void onSliderEvent(ofxDatGuiSliderEvent e) {}; 		// Empty function to prevent warnings

	// Drawing functions
	void drawStart();
	void drawInstruction();
	void drawHistograms();
	void drawPredictions();
private:
	// Spotify themed colors
	ofColor black_;
	ofColor green_;
	ofColor grey_;

	// Fonts
	ofTrueTypeFont title_font_;
	ofTrueTypeFont font_;

	// Text input
	ofxDatGuiTextInput* input_;

	// Buttons
	ofxDatGuiButton* start_button_;
	ofxDatGuiButton* instruction_button_;
	ofxDatGuiButton* histogram_button_;
	ofxDatGuiButton* prediction_button_;
	ofxDatGuiButton* get_prediction_button_;

	// Histogram features
	ofxGPlot plot_;
	vector<vector<ofxGPoint>> histogram_points_l_;
	vector<vector<ofxGPoint>> histogram_points_d_;
	vector<string> histogram_titles_;

	vector<double> means_;
	vector<double> stds_;

	vector<double> lower_bounds_;
	vector<double> upper_bounds_;

	int current_index_ = 0;
	int feature_size_;

	// Flags to determine which screen application
	// is on
	bool start_ = true;
	bool instruction_ = false;
	bool histogram_ = false;
	bool prediction_ = false;

	// Pictures used in instructions
	ofImage parameters_;
	ofImage globals_;

	// Sliders
	vector<ofxDatGuiSlider*> feature_sliders_;

	// For Predictions
	TrackPredictor predictor_;
	vector<string> result_messages_;
	string result_ = "";
};
