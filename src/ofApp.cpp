#include "ofApp.h"
//For debugging
#include <iostream>

//--------------------------------------------------------------
void ofApp::setup(){
	ofColor black(0, 0, 0);
	ofSetWindowTitle("Spotify Visualization");
	ofBackground(black);

	// Setting up data to used
	ifstream infile0("../data/liked_songs_features.json");
	ifstream infile1("../data/disliked_songs_features.json");
	Json::Reader reader;
	Json::Value audio;
	reader.parse(infile0, audio);
	TopTracks tracks_obj;
	vector<vector<pair<string, double>>> liked_dataset 
		= tracks_obj.GetDataset(audio["audio_features"], 100);
	reader.parse(infile1, audio);
	vector<vector<pair<string, double>>> disliked_dataset 
		= tracks_obj.GetDataset(audio["audio_features"], 100);
	
	vector<vector<pair<string, double>>> combined_dataset = liked_dataset;
	combined_dataset.insert(combined_dataset.end(), disliked_dataset.begin(), 
		disliked_dataset.end());

	vector<vector<pair<string, double>>> standardized_dataset 
		= tracks_obj.StandardizeFeatures(combined_dataset);

	//From example
//	ofSetWindowShape(1920, 1080);
//	ofSetWindowPosition(ofGetScreenWidth() / 2 - ofGetWidth() / 2, 0);

//  Code from ofxDatGui Example, just for reference

	input_ = new ofxDatGuiTextInput("TEXT INPUT", "Type Something Here");
	input_->onTextInputEvent(this, &ofApp::onTextInputEvent);
	input_->setWidth(800, .2);
	input_->setPosition(ofGetWidth() / 2 - input_->getWidth() / 2, 650);
	font_.load("ofxbraitsch/fonts/Verdana.ttf", 24);

// Reference from ofxGrafica Histogram Example
	int start_limit = 0;
	int end_limit = 100;
	histogram_points_l = calculateHistograms(liked_dataset, start_limit, end_limit);
	start_limit = 100;
	end_limit = standardized_dataset.size();
	histogram_points_d = calculateHistograms(disliked_dataset, start_limit, end_limit);
	histogram_titles_ = setupTitles(standardized_dataset);
}

void ofApp::setupPlot() {
	plot_.setPos(0, 15);
	plot_.setDim(900, 500);
	//plot_.setXLim(-0.5, 0.5);
	plot_.setTitleText(histogram_titles_[current_index_]);
	plot_.getTitle().setFontSize(24);
	plot_.getYAxis().getAxisLabel().setText("N");
	plot_.getYAxis().getAxisLabel().setRotate(false);
	plot_.getYAxis().setFontColor({ ofColor(255, 255, 255, 100) });
	plot_.getYAxis().setFontSize(18);
	plot_.getXAxis().setFontColor({ ofColor(255, 255, 255, 100) });
	plot_.getXAxis().setFontSize(18);
	plot_.setPoints(histogram_points_l[current_index_]);
	plot_.addLayer("Disliked Data", histogram_points_d[current_index_]);
	plot_.startHistograms(GRAFICA_VERTICAL_HISTOGRAM);
	plot_.getLayer("Disliked Data").getHistogram().setBgColors({ ofColor(255, 0, 0, 50) });
	plot_.getLayer("Disliked Data").getHistogram().setLineColors({ ofColor(0, 0) });
	plot_.getHistogram().setBgColors({ ofColor(0, 255, 0, 100) });
	plot_.getHistogram().setLineColors({ ofColor(0, 0) });
	plot_.getHistogram().setSeparations({ 0 });
	plot_.getHistogram().setFontColor({ ofColor(255, 255, 255, 100) });
	plot_.getHistogram().setFontSize(18);
	plot_.activateZooming(1.2, OF_MOUSE_BUTTON_LEFT, OF_MOUSE_BUTTON_LEFT);
	plot_.activateReset();
}

vector<string> ofApp::setupTitles(vector<vector<pair<string, double>>> dataset) {
	vector<string> titles;
	// just take the first sample for titles
	for (int i = 0; i < dataset[0].size(); i++) {
		titles.push_back(dataset[0][i].first);
	}
	return titles;

}

vector<vector<ofxGPoint>> ofApp::calculateHistograms(vector<vector<pair<string, double>>> dataset,
	int start_limit, int end_limit) {
	feature_size_ = dataset[0].size();
	double bin_size = 0.03;
	int range = 2;
	int num_bins = (int) (range / bin_size);
	// dataset[0].size == number of features
	vector<vector<ofxGPoint>> histogram_points(dataset[0].size());

	for (int feature = 0; feature < dataset[0].size(); feature++) {
		vector<double> count_in_bins(num_bins, 0);
		for (int sample = start_limit; sample < end_limit; sample++) {
			// do not consider outliers, temporarily
			//if (dataset[sample][feature].second < 1 && dataset[sample][feature].second >= -1) {
				count_in_bins[(int)((dataset[sample][feature].second + 1) / bin_size)]++;
			//}
		}
		for (int i = 0; i < num_bins; i++) {
			histogram_points[feature].emplace_back((i + 0.5) * bin_size - 1, count_in_bins[i]);
		}
	}
	return histogram_points;
}

void ofApp::onTextInputEvent(ofxDatGuiTextInputEvent e)
{
	// text input events carry the text of the input field //
	cout << "From Event Object: " << e.text << endl;
	// although you can also retrieve it from the event target //
	cout << "From Event Target: " << e.target->getText() << endl;
}

//--------------------------------------------------------------
void ofApp::update(){
	input_->update();
}

//--------------------------------------------------------------
void ofApp::draw(){
	input_->draw();
	string str = "Text Input: " + input_->getText();
	ofRectangle bounds = font_.getStringBoundingBox(str, ofGetWidth() / 2, 4 * ofGetHeight() / 5);
	ofSetColor(ofColor::black);
	font_.drawString(str, bounds.x - bounds.width / 2, bounds.y - bounds.height / 2);

	// Reference from ofxGrafica
	plot_.beginDraw();
	plot_.drawBox();
	plot_.drawTitle();
	plot_.drawXAxis();
	plot_.drawYAxis();
	plot_.drawGridLines(GRAFICA_VERTICAL_DIRECTION);
	plot_.drawHistograms();

	plot_.endDraw();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	if (key == OF_KEY_RIGHT) {
		current_index_++;
		if (current_index_ == feature_size_) {
			current_index_ = 0;
		}
		histogramUpdate();
	}
	else if (key == OF_KEY_LEFT) {
		current_index_--;
		if (current_index_ < 0) {
			current_index_ = feature_size_ - 1;
		}
		histogramUpdate();
	}

}

void ofApp::histogramUpdate() {
	plot_.setTitleText(histogram_titles_[current_index_]);
	plot_.setPoints(histogram_points_l[current_index_]);
	plot_.getLayer("Disliked Data").setPoints(histogram_points_d[current_index_]);
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){

}
