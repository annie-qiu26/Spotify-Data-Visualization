#include "ofApp.h"
//For debugging
#include <iostream>

//--------------------------------------------------------------
void ofApp::setup() {
	setupColors();

	ofSetWindowTitle("Spotify Visualization");
	ofBackground(black_);
	font_.load("montserrat/Montserrat-Black.ttf", 14);
	title_font_.load("montserrat/Montserrat-Bold.ttf", 72);

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


	setupTitles(standardized_dataset);
	setupHistograms();
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

void ofApp::setupHistograms() {
	// Code derived from: ofxGrafica example
	plot_.setPos(ofGetWidth() / 2 - 250, ofGetHeight() / 2 - 250);
	plot_.setDim(1200, 700);
	plot_.setFontName("montserrat/Montserrat-Black.ttf");

	// Setting title properties
	plot_.getTitle().setFontName("montserrat/Montserrat-Black.ttf");
	plot_.setTitleText(histogram_titles_[current_index_]);
	plot_.getTitle().setFontSize(24);
	plot_.getTitle().setFontColor(ofColor(255, 255, 255));

	// Setting axis properties
	plot_.getYAxis().getAxisLabel().setText("N");
	plot_.getYAxis().getAxisLabel().setRotate(false);
	plot_.getYAxis().setFontColor({ ofColor(255, 255, 255) });
	plot_.getYAxis().setFontSize(18);
	plot_.getXAxis().setFontColor({ ofColor(255, 255, 255) });
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
	input_->setPosition(ofGetWidth() / 2 + 50, 850);

	start_button_ = new ofxDatGuiButton("Start");
	start_button_->onButtonEvent(this, &ofApp::onButtonEvent);
	start_button_->setWidth(150);
	start_button_->setStripeColor(green_);

	prediction_button_ = new ofxDatGuiButton("Predictions");
	prediction_button_->onButtonEvent(this, &ofApp::onButtonEvent);
	prediction_button_->setWidth(150);
	prediction_button_->setStripeColor(green_);

	parameters_.load("parameters.png");
	globals_.load("globals.png");
}

void ofApp::setupTitles(vector<vector<pair<string, double>>> dataset) {
	// just take the first sample for titles
	for (int i = 0; i < dataset[0].size(); i++) {
		histogram_titles_.push_back(dataset[0][i].first);
	}
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
			histogram_points[feature].emplace_back(((i + 0.5) * bin_size - 3) * stds[feature]
				+ means[feature], count_in_bins[i]);
		}
	}
	return histogram_points;
}

void ofApp::onTextInputEvent(ofxDatGuiTextInputEvent e)
{
	string target = e.text;
	transform(target.begin(), target.end(), target.begin(), ::tolower);
	for (int i = 0; i < histogram_titles_.size(); i++) {
		// Code derived from:
		// http://thispointer.com/implementing-a-case-insensitive-stringfind-in-c/
		string title = histogram_titles_[i];
		transform(title.begin(), title.end(), title.begin(), ::tolower);
		if (title.find(target) != string::npos) {
			current_index_ = i;
			histogramUpdate();
		}
	}
}

void ofApp::onButtonEvent(ofxDatGuiButtonEvent e)
{
	if (start_) {
		start_ = false;
		instruction_ = true;
	}
	else {
		histogram_ = false;
		prediction_ = true;
	}
}

//--------------------------------------------------------------
void ofApp::update(){
	input_->update();
	start_button_->update();
	prediction_button_->update();
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
		drawPredictions();
	}
}

void ofApp::drawStartScreen() {
	// Drawing Title
	ofSetColor(ofColor{ 255, 255, 255 });
	title_font_.drawString("Spotify Data\nVisualization", ofGetWidth() / 2 - 318, 2 * ofGetHeight() / 5);

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
	string link = "https://www.getpostman.com/collections/47dc5dade17a715c89b2";
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

	// Drawing Button
	prediction_button_->draw();
	prediction_button_->setPosition(ofGetWidth() - 200,
		ofGetHeight() - 75);
}

void ofApp::drawPredictions() {
	ofSetColor(ofColor{ 255, 255, 255 });
	title_font_.drawString("Track Prediction", ofGetWidth() / 2 - 365, ofGetHeight() / 8);
	font_.drawString("1. In the same Postman collection before, in the search track request\nput the title of the song in the q parameter",
		50, 2 * ofGetHeight() / 9);
	font_.drawString("2. Make sure you press \"Use Token\" for authentication", 50,
		3 * ofGetHeight() / 9);
	font_.drawString("3. Once you press send, you should receive a JSON body", 50, 4 * ofGetHeight() / 9);
	font_.drawString("4. Find the ID of the track you're looking for. It should be above \"is_local\", \nwhich is right above the song's name",
		50, 5 * ofGetHeight() / 9);
	font_.drawString("5. In \"Globals\", paste in the track ID in the track_id parameter", 50,
		6 * ofGetHeight() / 9);
	font_.drawString("6. In \"Get Audio Track Features\", make sure to press \"Use Token\" for authentication",
		50, 7 * ofGetHeight() / 9);
	font_.drawString("7. Press send, and you should receive a JSON body of the track features",
		50, 8 * ofGetHeight() / 9);
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
	else if (key == 's' && instruction_) {
		histogram_ = true;
		instruction_ = false;
	}

}

void ofApp::histogramUpdate() {
	plot_.setTitleText(histogram_titles_[current_index_]);
	plot_.setPoints(histogram_points_l_[current_index_]);
	plot_.getLayer("Disliked Data").setPoints(histogram_points_d_[current_index_]);
	plot_.setXLim(lower_bounds_[current_index_], upper_bounds_[current_index_]);
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
