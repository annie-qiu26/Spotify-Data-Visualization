#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	ofColor black(0, 0, 0);
	ofSetWindowTitle("Spotify Visualization");
	ofBackground(black);

	// Setting up data to used
	ifstream infile3("../data/disliked_songs_features.json");
	Json::Reader reader;
	Json::Value audio;
	reader.parse(infile3, audio);
	TopTracks tracks_obj;;
	vector<vector<pair<string, double>>> dataset = tracks_obj.GetDataset(audio["audio_features"], 100);
	vector<vector<pair<string, double>>> standardized_dataset = tracks_obj.StandardizeFeatures(dataset);

	//From example
//	ofSetWindowShape(1920, 1080);
//	ofSetWindowPosition(ofGetScreenWidth() / 2 - ofGetWidth() / 2, 0);

//  Code from ofxDatGui Example, just for reference

	input = new ofxDatGuiTextInput("TEXT INPUT", "Type Something Here");
	input->onTextInputEvent(this, &ofApp::onTextInputEvent);
	input->setWidth(800, .2);
	input->setPosition(ofGetWidth() / 2 - input->getWidth() / 2, 240);
	font.load("ofxbraitsch/fonts/Verdana.ttf", 24);

// Reference from ofxGrafica Histogram Example
	vector<ofxGPoint> histogram_points = calculateHistograms(standardized_dataset);

	plot.setPos(0, 0);
	plot.setDim(300, 200);
	plot.setXLim(0, 2);
	plot.setTitleText("Test Histogram");
	plot.getYAxis().getAxisLabel().setText("N");
	plot.getYAxis().getAxisLabel().setRotate(false);
	plot.setPoints(histogram_points);
	plot.startHistograms(GRAFICA_VERTICAL_HISTOGRAM);
	plot.getHistogram().setBgColors({ ofColor(255, 0, 0, 100) });
	plot.getHistogram().setLineColors({ ofColor(0, 0) });
	plot.getHistogram().setSeparations({ 0 });
	plot.activateZooming(1.2, OF_MOUSE_BUTTON_LEFT, OF_MOUSE_BUTTON_LEFT);
	plot.activateReset();


}

vector<ofxGPoint> ofApp::calculateHistograms(vector<vector<pair<string, double>>> dataset) {
	double bin_size = 0.05;
	int range = 2;
	int num_bins = (int) (range / bin_size);
	vector<ofxGPoint> histogram_points;
	vector<double> count_in_bins(num_bins, 0);

	for (int j = 0; j < dataset.size(); j++) {
		// do not consider outliers, temporarily
		if (dataset[j][0].second < 1 && dataset[j][0].second >= -1) {
			count_in_bins[(int) ((dataset[j][0].second + 1) / bin_size) ]++;
		}
	}
	for (int i = 0; i < num_bins; i++) {
		histogram_points.emplace_back((i + 0.5) * bin_size, count_in_bins[i]);
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
	input->update();
}

//--------------------------------------------------------------
void ofApp::draw(){
	input->draw();
	string str = "Text Input: " + input->getText();
	ofRectangle bounds = font.getStringBoundingBox(str, ofGetWidth() / 2, ofGetHeight() / 2);
	ofSetColor(ofColor::black);
	font.drawString(str, bounds.x - bounds.width / 2, bounds.y - bounds.height / 2);

	// Reference from ofxGrafica
	plot.beginDraw();
	plot.drawBox();
	plot.drawTitle();
	plot.drawXAxis();
	plot.drawYAxis();
	plot.drawGridLines(GRAFICA_VERTICAL_DIRECTION);
	plot.drawHistograms();
	plot.endDraw();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

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
