#include "ofApp.h"

// Function that setups everything in the application
//--------------------------------------------------------------
void ofApp::setup() {
	setupColors();

	ofSetWindowTitle("Spotify Visualization");
	ofBackground(black_);

	vector<vector<pair<string, double>>> standardized_dataset =
		standardizeDataset();

	// First half of dataset are liked songs, second
	// half are disliked songs.
	int start_limit = 0;
	int end_limit = 100;
	histogram_points_l_ = calculateHistograms(standardized_dataset, start_limit, end_limit,
		means_, stds_);
	start_limit = 100;
	end_limit = standardized_dataset.size();
	histogram_points_d_ = calculateHistograms(standardized_dataset, start_limit, end_limit,
		means_, stds_);

	setupGUI();
	setupTitles(standardized_dataset);
	setupSliders(means_, stds_);
	setupBounds(means_, stds_);
	setupHistograms();
	setupResultMessages();

	// Gets weights and bias of prediction for SVM classifier
	vector<vector<double>> modified_dataset =
		predictor_.RemoveTitles(standardized_dataset);
	predictor_.SVMTrain(modified_dataset);
}

// Helper function used to combine liked and disliked
// songs' datasets and standardizes them
//--------------------------------------------------------------
vector<vector<pair<string, double>>> ofApp::standardizeDataset() {
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

	means_ = tracks_obj.CalculateMeans(combined_dataset);
	stds_ = tracks_obj.CalculateStds(combined_dataset);
	vector<vector<pair<string, double>>> standardized_dataset
		= tracks_obj.StandardizeFeatures(combined_dataset);

	return standardized_dataset;
}

// Sets up the main colors that mimick Spotify's theme
//--------------------------------------------------------------
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

// Function used to set features of the histograms such as
// font, title, and colors
//--------------------------------------------------------------
void ofApp::setupHistograms() {
	// Code derived from: ofxGrafica example
	plot_.setPos(ofGetWidth() / 2 - 190, ofGetHeight() / 2 - 325);
	plot_.setDim(1200, 700);
	plot_.setFontName("montserrat/Montserrat-Black.ttf");

	// Setting title properties
	plot_.getTitle().setFontName("montserrat/Montserrat-Black.ttf");
	plot_.setTitleText(histogram_titles_[current_index_]);
	plot_.getTitle().setFontSize(24);
	plot_.getTitle().setFontColor({ ofColor(255, 255, 255) });

	// Setting axis properties
	plot_.getYAxis().getAxisLabel().setText("# of Songs");
	plot_.getYAxis().getAxisLabel().setOffset(65);
	plot_.getYAxis().getAxisLabel().setFontName("montserrat/Montserrat-Black.ttf");
	plot_.getYAxis().getAxisLabel().setFontColor({ ofColor(255, 255, 255) });
	plot_.getYAxis().getAxisLabel().setFontSize(18);
	plot_.getYAxis().setFontColor({ ofColor(255, 255, 255) });
	plot_.getYAxis().setFontSize(18);
	plot_.getYAxis().setFontName("montserrat/Montserrat-Black.ttf");
	plot_.getXAxis().setFontColor({ ofColor(255, 255, 255) });
	plot_.getXAxis().setFontSize(18);
	plot_.getXAxis().setFontName("montserrat/Montserrat-Black.ttf");

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

// Sets up the graphical interface that involve buttons, test
// boxes, fonts, etc.
//--------------------------------------------------------------
void ofApp::setupGUI() {
	// Sets up text input for search feature
	input_ = new ofxDatGuiTextInput("Search", "Type Something Here");
	input_->onTextInputEvent(this, &ofApp::onTextInputEvent);
	input_->setWidth(800, .2);
	input_->setPosition(ofGetWidth() / 2 + 75, 875);
	ofxDatGuiTextInputField* field = input_->getTextInput();
	field->setTextInactiveColor(green_);
	input_->setStripeColor(green_);

	setupButtons();

	// Loads in pictures
	parameters_.load("parameters.png");
	globals_.load("globals.png");

	// Loads in fonts
	font_.load("montserrat/Montserrat-Black.ttf", 14);
	title_font_.load("montserrat/Montserrat-Bold.ttf", 72);
}

// Sets the vector of strings that holds the titles for each histogram
//--------------------------------------------------------------
void ofApp::setupTitles(vector<vector<pair<string, double>>> dataset) {
	// just take the first sample for titles
	for (int i = 0; i < dataset[0].size(); i++) {
		histogram_titles_.push_back(dataset[0][i].first);
	}
}

// Sets the vectors of doubles that holds the bounds for the
// X-Axes of the histograms
//--------------------------------------------------------------
void ofApp::setupBounds(vector<double> means, vector<double> stds) {
	for (int i = 0; i < means.size(); i++) {
		lower_bounds_.push_back(means[i] - 4 * stds[i]);
		upper_bounds_.push_back(means[i] + 4 * stds[i]);
	}
}

// Function used to set button properties such as size,
// position, and color
//--------------------------------------------------------------
void ofApp::setupButtons() {
	start_button_ = new ofxDatGuiButton("Back to Start");
	start_button_->onButtonEvent(this, &ofApp::onButtonEvent);
	start_button_->setWidth(125);
	start_button_->setStripeColor(green_);

	instruction_button_ = new ofxDatGuiButton("Start");
	instruction_button_->onButtonEvent(this, &ofApp::onButtonEvent);
	instruction_button_->setWidth(150);
	instruction_button_->setStripeColor(green_);

	histogram_button_ = new ofxDatGuiButton("To Histograms");
	histogram_button_->onButtonEvent(this, &ofApp::onButtonEvent);
	histogram_button_->setWidth(100);
	histogram_button_->setStripeColor(green_);

	prediction_button_ = new ofxDatGuiButton("Go to Prediction Screen");
	prediction_button_->onButtonEvent(this, &ofApp::onButtonEvent);
	prediction_button_->setWidth(150);
	prediction_button_->setStripeColor(green_);

	get_prediction_button_ = new ofxDatGuiButton("Get Prediction");
	get_prediction_button_->onButtonEvent(this, &ofApp::onButtonEvent);
	get_prediction_button_->setWidth(100);
	get_prediction_button_->setStripeColor(green_);
}

// Function used to set slider properties such as size,
// position, and color
//--------------------------------------------------------------
void ofApp::setupSliders(vector<double> means, vector<double> stds) {
	for (int i = 0; i < means.size(); i++) {
		ofxDatGuiSlider* slider = new ofxDatGuiSlider(histogram_titles_[i],
			means[i] - 5 * stds[i], means[i] + 5 * stds[i], means[i]);
		slider->setPrecision(4, false);
		slider->setPosition(ofGetWidth() + 185, i * ofGetHeight() / 12 + 200);
		slider->setStripeColor(green_);
		slider->setSliderFillColor(green_);
		slider->setTextInputColor(green_);
		slider->setWidth(500, 150);
		slider->onSliderEvent(this, &ofApp::onSliderEvent);
		feature_sliders_.push_back(slider);
	}
}

// Function used to set up default like/dislike messages
// in classifying songs for users
//--------------------------------------------------------------
void ofApp::setupResultMessages() {
	result_messages_.push_back("That's an awesome song!");
	result_messages_.push_back("I'll listen to that if I were you");
	result_messages_.push_back("You have great music taste");
	result_messages_.push_back("Please save your ears");
	result_messages_.push_back("I wouldn't listen to that if I were you");
	result_messages_.push_back("I would mute that song");
}

// Function used to calculate frequencies in bins based on
// the given dataset, sets the points for the histograms
//--------------------------------------------------------------
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
			// Code derived from: ofxGrafica example
			histogram_points[feature].emplace_back(((i + 0.5) * bin_size - 3)
				* stds[feature] + means[feature], count_in_bins[i]);
		}
	}
	return histogram_points;
}

// GUI function that responds when a user inputs text, user can
// search for the histogram that they want to display
//--------------------------------------------------------------
void ofApp::onTextInputEvent(ofxDatGuiTextInputEvent e)
{
	string target = e.text;
	transform(target.begin(), target.end(), target.begin(), ::tolower);
	for (int i = 0; i < histogram_titles_.size(); i++) {
		// Code derived from:
		// http://thispointer.com/implementing-a-case-insensitive-stringfind-in-c/
		string title = histogram_titles_[i];
		transform(title.begin(), title.end(), title.begin(), ::tolower);
		if (title.find(target) == 0 && title.length() == target.length()) {
			current_index_ = i;
			histogramUpdate();
		}
	}
}

// GUI function that responds when a user clicks a button
//--------------------------------------------------------------
void ofApp::onButtonEvent(ofxDatGuiButtonEvent e)
{
	string target = e.target->getLabel();
	if (target == "Back to Start") {
		start_ = true;
		instruction_ = false;
		histogram_ = false;
		prediction_ = false;
	}
	else if (target == "Start") {
		start_ = false;
		instruction_ = true;
		histogram_ = false;
		prediction_ = false;
	}
	else if (target == "To Histograms") {
		start_ = false;
		instruction_ = false;
		histogram_ = true;
		prediction_ = false;
	}
	else if (target == "Go to Prediction Screen") {
		start_ = false;
		instruction_ = false;
		histogram_ = false;
		prediction_ = true;
	}
	else if (target == "Get Prediction") {
		resultUpdate(means_, stds_);
	}
}

// openFrameworks default update function
//--------------------------------------------------------------
void ofApp::update(){
	input_->update();
	start_button_->update();
	instruction_button_->update();
	histogram_button_->update();
	prediction_button_->update();
	get_prediction_button_->update();
	for (ofxDatGuiSlider* feature : feature_sliders_) {
		feature->update();
	}
}

// Chooses which screen to draw on the screen; there are four:
// start, instruction, histogram, prediction
//--------------------------------------------------------------
void ofApp::draw() {
	if (start_) {
		drawStart();
	}
	else if (instruction_) {
		drawInstruction();
	}
	else if (histogram_) {
		drawHistograms();
	}
	else if (prediction_) {
		drawPredictions();
	}
}

// Function that displays the start screen of the application
// to the user
//--------------------------------------------------------------
void ofApp::drawStart() {
	// Drawing Title
	ofSetColor(ofColor{ 255, 255, 255 });
	title_font_.drawString("Spotify Data\nVisualization", ofGetWidth() / 2 - 318, 2 * ofGetHeight() / 5);

	// Drawing Button
	instruction_button_->draw();
	instruction_button_->setPosition(ofGetWidth() / 2 - instruction_button_->getWidth() / 3,
		3 * ofGetHeight() / 5 + 20);
}

// Function that displays the instruction screen of the application
// to the user
//--------------------------------------------------------------
void ofApp::drawInstruction() {
	// Drawing Instructions
	ofSetColor(ofColor{ 255, 255, 255 });
	font_.drawString("1. Download the Postman extension on Google Chrome", 75,
		ofGetHeight() / 15);
	string link = "https://www.getpostman.com/collections/47dc5dade17a715c89b2";
	font_.drawString("2. Click import, and then \"import from link\" and paste in this link:\n" + link,
		75, 2 * ofGetHeight() / 15);
	font_.drawString("3. Once you get the collection, change the Authorization type to OAuth2",
		75, 3 * ofGetHeight() / 15);
	font_.drawString("4. Get new access token, and fill in the following parameters shown on the right",
		75, 4 * ofGetHeight() / 15);
	parameters_.draw(3 * ofGetWidth() / 5 + 100, 75, 350, 450);
	font_.drawString("5. Client ID is: 8e8f59148afa40b08367c08ad922bf1f\nClient Secret is: 205d49e13d1a4086b200c350dd244f6e",
		75, 5 * ofGetHeight() / 15);
	font_.drawString("6. Request token, sign in to Spotify and accept conditions, and then click use token",
		75, 6 * ofGetHeight() / 15);
	font_.drawString("7. Press send for \"Find Playlist IDs\" and it should show a JSON body",
		75, 7 * ofGetHeight() / 15);
	font_.drawString("8. Find the playlist IDs of your Liked playlist and Disliked playlist\nNote the ids are above the name of the playlists",
		75, 8 * ofGetHeight() / 15);
	font_.drawString("9. Go to the \"Get Liked Playlist Tracks' IDs\" GET request, and click on\n\"Manage Environment\" in settings",
		75, 9 * ofGetHeight() / 15);
	font_.drawString("10. Click on \"Globals\" and fill out the parameters shown on the right",
		75, 10 * ofGetHeight() / 15);
	globals_.draw(3 * ofGetWidth() / 5 - 25, 550, 600, 400);
	font_.drawString("11. Press send and you should receive another JSON body", 75,
		11 * ofGetHeight() / 15);
	font_.drawString("12. Copy the JSON body in \"liked_songs_features\" file in the data folder",
		75, 12 * ofGetHeight() / 15);
	font_.drawString("13. Repeat for steps 9 - 12 with \"Get Disliked Playlist Tracks' IDs\"",
		75, 13 * ofGetHeight() / 15);
	font_.drawString("14. Press button on right to display the data", 75,
		14 * ofGetHeight() / 15);

	// Drawing button
	histogram_button_->draw();
	histogram_button_->setPosition(585,
		14 * ofGetHeight() / 15 - 15);

}

// Function that displays the histogram screen of the application
// to the user
//--------------------------------------------------------------
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

	// Drawing Buttons
	prediction_button_->draw();
	prediction_button_->setPosition(ofGetWidth() - 185,
		ofGetHeight() - 60);
	start_button_->draw();
	start_button_->setPosition(40, ofGetHeight() - 60);
}

// Function that displays the prediction screen of the application
// to the user
//--------------------------------------------------------------
void ofApp::drawPredictions() {
	ofSetColor(ofColor{ 255, 255, 255 });
	title_font_.drawString("Track Prediction", ofGetWidth() / 2 - 375, ofGetHeight() / 10);

	// Drawing Instructions
	font_.drawString("1. In the same Postman collection before, find the search track request\nput the title of the song in the q parameter",
		50, 2 * ofGetHeight() / 10);
	font_.drawString("2. Make sure you press \"Use Token\" for authentication", 50,
		3 * ofGetHeight() / 10);
	font_.drawString("3. Once you press send, you should receive a JSON body", 50, 4 * ofGetHeight() / 10);
	font_.drawString("4. Find the ID of the track you're looking for. It should be above \"is_local\", \nwhich is right above the song's name",
		50, 5 * ofGetHeight() / 10);
	font_.drawString("5. In \"Globals\", paste in the track ID in the track_id parameter", 50,
		6 * ofGetHeight() / 10);
	font_.drawString("6. In \"Get Audio Track Features\", make sure to press \"Use Token\" for authentication",
		50, 7 * ofGetHeight() / 10);
	font_.drawString("7. Press send, and you should receive a JSON body of the track features",
		50, 8 * ofGetHeight() / 10);
	font_.drawString("8. Input feature values in sliders to the right, and press button for results",
		50, 9 * ofGetHeight() / 10);

	// Drawing Buttons
	start_button_->draw();
	start_button_->setPosition(50, 50);
	histogram_button_->draw();
	histogram_button_->setPosition(ofGetWidth() - 150, 50);
	get_prediction_button_->draw();
	get_prediction_button_->setPosition(3 * ofGetWidth() / 4, 10 * ofGetHeight() / 12 + 30);

	// Drawing Sliders
	for (ofxDatGuiSlider* feature : feature_sliders_) {
		feature->draw();
	}

	// Drawing Results
	font_.drawString(result_, 3 * ofGetWidth() / 4 - 100, 11 * ofGetHeight() / 12 + 30);
}

// Function that changes the histogram displayed, uses
// the left and right arrow keys to navigate
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

}

// Function that updates the histogram displayed after
// user navigation or search
//--------------------------------------------------------------
void ofApp::histogramUpdate() {
	plot_.setTitleText(histogram_titles_[current_index_]);
	plot_.setPoints(histogram_points_l_[current_index_]);
	plot_.getLayer("Disliked Data").setPoints(histogram_points_d_[current_index_]);
	plot_.setXLim(lower_bounds_[current_index_], upper_bounds_[current_index_]);
}

// Function that updates the result message after user
// requests features of a track to be classified into like 
// or dislike
//--------------------------------------------------------------
void ofApp::resultUpdate(vector<double> means, vector<double> stds) {
	vector<double> sample;
	for (int i = 0; i < feature_sliders_.size(); i++) {
		// Push back standardized value
		sample.push_back((feature_sliders_[i]->getValue() - means[i]) / stds[i]);
		cout << sample[i] << endl;
	}
	int classification = predictor_.Classify(sample);
	// First half of messages are positive results
	int index = rand() % (result_messages_.size() / 2);
	if (classification == -1) {
		index += result_messages_.size() / 2;
	}
	result_ = result_messages_[index];
}

//--------------------------------------------------------------
