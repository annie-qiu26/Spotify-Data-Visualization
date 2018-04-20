#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	ofColor black(0, 0, 0);
	ofSetWindowTitle("Spotify Visualization");
	ofBackground(black);
	
	//From example
	ofSetWindowShape(1920, 1080);
	ofSetWindowPosition(ofGetScreenWidth() / 2 - ofGetWidth() / 2, 0);

	input = new ofxDatGuiTextInput("TEXT INPUT", "Type Something Here");
	input->onTextInputEvent(this, &ofApp::onTextInputEvent);
	input->setWidth(800, .2);
	input->setPosition(ofGetWidth() / 2 - input->getWidth() / 2, 240);
	font.load("ofxbraitsch/fonts/Verdana.ttf", 24);
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
