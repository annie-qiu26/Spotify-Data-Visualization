#include "ofApp.h"
//For debugging
#include <iostream>

//--------------------------------------------------------------
void ofApp::setup() {
	setupColors();

	ofSetWindowTitle("Spotify Visualization");
	ofBackground(black_);
	font_.load("montserrat/Montserrat-Black.ttf", 14);

	// Setting up data to used
	ifstream infile0("../data/liked_songs_features.json");
	ifstream infile1("../data/disliked_songs_features.json");
	Json::Reader reader;
	Json::Value audio;
	reader.parse(infile0, audio);
	TopTracks tracks_obj;

	// Getting the data set into vectors
	vector<vector<pair<string, double>>> liked_dataset
		= tracks_obj.GetDataset(audio["audio_features"], 100);

	reader.parse(infile1, audio);
	vector<vector<pair<string, double>>> disliked_dataset
		= tracks_obj.GetDataset(audio["audio_features"], 100);

	// Take any sample for the dataset to set the size
	feature_size_ = liked_dataset[0].size();

	// Combining the dataset
	vector<vector<pair<string, double>>> combined_dataset = liked_dataset;
	combined_dataset.insert(combined_dataset.end(), disliked_dataset.begin(),
		disliked_dataset.end());

	vector<double> means = tracks_obj.CalculateMeans(combined_dataset);
	vector<double> stds = tracks_obj.CalculateStds(combined_dataset);
	vector<vector<pair<string, double>>> standardized_dataset
		= tracks_obj.StandardizeFeatures(combined_dataset);

	setupGUI();

// Reference from ofxGrafica Histogram Example
	int start_limit = 0;
	int end_limit = 100;
	histogram_points_l_ = calculateHistograms(standardized_dataset, start_limit, end_limit,
		means, stds);
	start_limit = 100;
	end_limit = standardized_dataset.size();
	histogram_points_d_ = calculateHistograms(standardized_dataset, start_limit, end_limit,
		means, stds);
	histogram_titles_ = setupTitles(standardized_dataset);

	setupPlot();
}

void ofApp::setupColors() {
	green_.r = 132;
	green_.g = 189;
	green_.b = 0;

	black_.r = 0;
	black_.g = 0;
	black_.b = 0;

	grey_.r = 130;
	grey_.g = 130;
	grey_.b = 130;
}

void ofApp::setupPlot() {
	plot_.setPos(ofGetWidth() / 2 - 75, ofGetHeight() / 2 - 150);
	plot_.setDim(900, 500);
	plot_.setXLim(-4, 4);

	// Setting title properties
	plot_.setTitleText(histogram_titles_[current_index_]);
	plot_.getTitle().setFontSize(24);

	// Setting axis properties
	plot_.getYAxis().getAxisLabel().setText("N");
	plot_.getYAxis().getAxisLabel().setRotate(false);
	plot_.getYAxis().setFontColor({ ofColor(255, 255, 255, 100) });
	plot_.getYAxis().setFontSize(18);
	plot_.getXAxis().setFontColor({ ofColor(255, 255, 255, 100) });
	plot_.getXAxis().setFontSize(18);

	// Setting points and layers
	plot_.setPoints(histogram_points_l_[current_index_]);
	plot_.addLayer("Disliked Data", histogram_points_d_[current_index_]);

	// Setting the histogram up and colors up
	plot_.startHistograms(GRAFICA_VERTICAL_HISTOGRAM);
	plot_.getLayer("Disliked Data").getHistogram().setBgColors({ ofColor(255, 0, 0, 50) });
	plot_.getLayer("Disliked Data").getHistogram().setLineColors({ ofColor(0, 0) });
	plot_.getHistogram().setBgColors({ ofColor(0, 255, 0, 50) });
	plot_.getHistogram().setLineColors({ ofColor(0, 0) });
	plot_.getHistogram().setSeparations({ 0 });
	plot_.getHistogram().setFontColor({ ofColor(255, 255, 255, 100) });
	plot_.getHistogram().setFontSize(18);
	plot_.activateZooming(1.2, OF_MOUSE_BUTTON_LEFT, OF_MOUSE_BUTTON_LEFT);
	plot_.activateReset();
}

void ofApp::setupGUI() {
	input_ = new ofxDatGuiTextInput("SEARCH", "Type Something Here");
	input_->onTextInputEvent(this, &ofApp::onTextInputEvent);
	input_->setWidth(800, .2);
	input_->setPosition(ofGetWidth() / 2 - input_->getWidth() / 2, 650);

	start_button_ = new ofxDatGuiButton("Start");
	start_button_->onButtonEvent(this, &ofApp::onButtonEvent);
	start_button_->setWidth(150);
	start_button_->setStripeColor(green_);

	parameters_.load("parameters.png");
	globals_.load("globals.png");
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
	int start_limit, int end_limit, vector<double> means, vector<double> stds) {

	double bin_size = 0.3;
	// consider 99.7 % of the data, 3 std deviation away
	int range = 6;
	int num_bins = (int)(range / bin_size);

	// dataset[0].size == number of features
	vector<vector<ofxGPoint>> histogram_points(dataset[0].size());

	for (int feature = 0; feature < dataset[0].size(); feature++) {
		vector<double> count_in_bins(num_bins, 0);
		for (int sample = start_limit; sample < end_limit; sample++) {
			// do not consider outliers, temporarily
			int binIndex = (int)((dataset[sample][feature].second + 3) / bin_size);
			if (binIndex < num_bins && binIndex >= 0) {
				count_in_bins[binIndex]++;
			}
		}
		for (int i = 0; i < num_bins; i++) {
			histogram_points[feature].emplace_back((i + 0.5) * bin_size - 3 /* * stds[feature]
				+ means[feature] */, count_in_bins[i]);
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

void ofApp::onButtonEvent(ofxDatGuiButtonEvent e)
{
	cout << e.target->getLabel() << " was clicked!" << endl;
	start_ = false;
	instruction_ = true;

}

//--------------------------------------------------------------
void ofApp::update(){
	input_->update();
	start_button_->update();
}

//--------------------------------------------------------------
void ofApp::draw() {
	if (start_) {
		drawStartScreen();
	}
	else if (instruction_) {
		drawInstructionScreen();
	}
	else if (histogram_) {
		drawHistograms();
	}
	else if (prediction_) {
		// drawPredictions();
	}
}

void ofApp::drawStartScreen() {
	// Drawing Title
	ofSetColor(ofColor{ 255, 255, 255 });
	ofTrueTypeFont title_font;
	title_font.load("montserrat/Montserrat-Bold.ttf", 72);
	title_font.drawString("Spotify Data\nVisualization", ofGetWidth() / 2 - 318, 2 * ofGetHeight() / 5);

	// Drawing Button
	start_button_->draw();
	start_button_->setPosition(ofGetWidth() / 2 - start_button_->getWidth() / 3,
		3 * ofGetHeight() / 5 + 20);
}

void ofApp::drawInstructionScreen() {
	// Drawing Instructions
	ofSetColor(ofColor{ 255, 255, 255 });
	font_.drawString("1. Download the Postman extension on Google Chrome", 50, 
		ofGetHeight() / 15);
	string link = "https://www.getpostman.com/collections/90c7c17db56eb15c0e5b";
	font_.drawString("2. Click import, and then \"import from link\" and paste in this link:\n" + link,
		50, 2 * ofGetHeight() / 15);
	font_.drawString("3. Once you get the collection, change the Authorization type to OAuth2",
		50, 3 * ofGetHeight() / 15);
	font_.drawString("4. Get new access token, and fill in the following parameters shown on the right",
		50, 4 * ofGetHeight() / 15);
	parameters_.draw(3 * ofGetWidth() / 5 + 50, 75, 350, 450);
	font_.drawString("5. Client ID is: 8e8f59148afa40b08367c08ad922bf1f\nClient Secret is: 205d49e13d1a4086b200c350dd244f6e",
		50, 5 * ofGetHeight() / 15);
	font_.drawString("6. Request token, sign in to Spotify and accept conditions, and then click use token",
		50, 6 * ofGetHeight() / 15);
	font_.drawString("7. Press send for \"Find Playlist IDs\" and it should show a JSON body",
		50, 7 * ofGetHeight() / 15);
	font_.drawString("8. Find the playlist IDs of your Liked playlist and Disliked playlist\nNote the ids are above the name of the playlists",
		50, 8 * ofGetHeight() / 15);
	font_.drawString("9. Go to the \"Get Liked Playlist Tracks' IDs\" GET request, and click on\n\"Manage Environment\" in settings",
		50, 9 * ofGetHeight() / 15);
	font_.drawString("10. Click on \"Globals\" and fill out the parameters shown on the right",
		50, 10 * ofGetHeight() / 15);
	globals_.draw(3 * ofGetWidth() / 5 - 75, 550, 600, 400);
	font_.drawString("11. Press send and you should receive another JSON body", 50,
		11 * ofGetHeight() / 15);
	font_.drawString("12. Copy the JSON body in \"liked_songs_features\" file in the data folder",
		50, 12 * ofGetHeight() / 15);
	font_.drawString("13. Repeat for steps 9 - 12 with \"Get Disliked Playlist Tracks' IDs\"",
		50, 13 * ofGetHeight() / 15);
	font_.drawString("14. Press \'s\' to display the data", 50,
		14 * ofGetHeight() / 15);

}

void ofApp::drawHistograms() {
	// Code derived from: ofxDatGui example
	input_->draw();
	string str = input_->getText();
	ofRectangle bounds = font_.getStringBoundingBox(str, ofGetWidth() / 2, 4 * ofGetHeight() / 5);
	ofSetColor(ofColor::black);
	font_.drawString(str, bounds.x - bounds.width / 2, bounds.y - bounds.height / 2);

	// Code derived from: ofxGrafica example
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
	if (histogram_ && key == OF_KEY_RIGHT) {
		current_index_++;
		if (current_index_ == feature_size_) {
			current_index_ = 0;
		}
		histogramUpdate();
	}
	else if (histogram_ && key == OF_KEY_LEFT) {
		current_index_--;
		if (current_index_ < 0) {
			current_index_ = feature_size_ - 1;
		}
		histogramUpdate();
	}
	//Temp fix
	else if (key == 's') {
		histogram_ = true;
		instruction_ = false;
	}

}

void ofApp::histogramUpdate() {
	plot_.setTitleText(histogram_titles_[current_index_]);
	plot_.setPoints(histogram_points_l_[current_index_]);
	plot_.getLayer("Disliked Data").setPoints(histogram_points_d_[current_index_]);
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
