#pragma once

#include "ofMain.h"
#include "ofxDatGui.h"
#include "ofxGrafica.h"
#include "top_tracks.h"

class ofApp : public ofBaseApp{

public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);

		void setupPlot();
		vector<vector<ofxGPoint>> calculateHistograms(vector<vector<pair<string, double>>> dataset,
			int start_limit, int end_limit);
		void histogramUpdate();
		vector<string> setupTitles(vector<vector<pair<string, double>>> dataset);
private:
	// From examples
	ofxDatGuiTextInput * input_;
	void onTextInputEvent(ofxDatGuiTextInputEvent e);
	ofTrueTypeFont font_;

	// Wait do I need these?
	ofColor point_color_;
	bool draw_lines_;
	int circle_resolution_;

	ofxGPlot plot_;
	vector<vector<ofxGPoint>> histogram_points_l;
	vector<vector<ofxGPoint>> histogram_points_d;
	vector<string> histogram_titles_;

	int current_index_ = 0;
	int feature_size_;

};
