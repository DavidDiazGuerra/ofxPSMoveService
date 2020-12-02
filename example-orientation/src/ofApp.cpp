#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	ofSetFrameRate(60);

	psMoveServerClient.initialize(); // Initialize the communication with PSMoveService
	psMoveServerClient.register_connected_controllers(true); // Start receiving information about the connected controllers
	psMoveServerClient.start(); // Start updating the controllers status for each new frame and generating events
	ofAddListener(psMoveServerClient.buttonPressedEvent, this, &ofApp::psMoveButtonPressed);

	image_PSMSAngles.load("PSMSAngles.png");
	image_navigationAngles.load("NavigationAngles.png");
	image_orientationVector.load("OrientationVector.png");
}

//--------------------------------------------------------------
void ofApp::update(){
	PSMSAngles = psMoveServerClient.get_orientationAngles(controller_in_screen);
	navigationAngles = psMoveServerClient.get_navigationAngles(controller_in_screen);
	orientationVector = psMoveServerClient.get_orientationVector(controller_in_screen);
}

//--------------------------------------------------------------
void ofApp::draw(){
	ofBackground(54, 54, 54);

	if (psMoveServerClient.n_controllers > 0) {
		ofDrawBitmapString("Controller " + ofToString(controller_in_screen), 40, 40);
		ofDrawBitmapString("Use the arrows to change the controller.", 40, 60);

		ofPushMatrix();
		ofTranslate(60, 100);
		ofDrawBitmapString("PSMService Euler (Tait-Bryan) angles:", 0, 0);
		image_PSMSAngles.draw(10, 40, 2142 / 7.5, 2921 / 7.5);

		ofTranslate(60, 500);
		drawBarIndicator(PSMSAngles.x / PI, "yaw", ofColor::blue);
		ofTranslate(65, 0);
		drawBarIndicator(PSMSAngles.y / PI, "pitch", ofColor::green);
		ofTranslate(65, 0);
		drawBarIndicator(PSMSAngles.z / PI, "roll", ofColor::red);
		
		ofPopMatrix();

		ofPushMatrix();
		ofTranslate(480, 100);
		ofDrawBitmapString("Navigation Euler (Tait-Bryan) angles:", 0, 0);
		image_navigationAngles.draw(10, 40, 2126 / 7.5, 2921 / 7.5);

		ofTranslate(60, 500);
		drawBarIndicator(navigationAngles.x / PI, "yaw", ofColor::blue);
		ofTranslate(65, 0);
		drawBarIndicator(navigationAngles.y / PI, "pitch", ofColor::green);
		ofTranslate(65, 0);
		drawBarIndicator(navigationAngles.z / PI, "roll", ofColor::red);
		ofPopMatrix();

		ofPushMatrix();
		ofTranslate(880, 100);
		ofDrawBitmapString("Orientation vector and rotation angle:", 0, 0);
		image_orientationVector.draw(10, 20, 1983 / 8, 3350 / 8);

		ofTranslate(60, 500);
		drawBarIndicator(orientationVector.x, "x", ofColor::red);
		ofTranslate(65, 0);
		drawBarIndicator(orientationVector.y, "y", ofColor::green);
		ofTranslate(65, 0);
		drawBarIndicator(orientationVector.z, "z", ofColor::blue);
		ofPopMatrix();

	}
	else {
		ofDrawBitmapString("No controllers detected", 40, 40);
	}
}

void ofApp::drawBarIndicator(float value, string name, ofColor color)
{
	ofPushStyle();
	ofNoFill();
	ofDrawRectangle(0, 0, 25, 200);
	ofFill();
	ofSetColor(color);
	ofDrawRectangle(0, 100, 25, -value*100);
	ofSetColor(ofColor::white);
	ofDrawBitmapString(name, 12.5-name.size()*4, 220);
	ofPopStyle();
}

void ofApp::psMoveButtonPressed(PSMoveEventInfo& eventInfo) {
	if (eventInfo.button == PSMOVE_SELECT_BUTTON) {
		psMoveServerClient.reset_orientation(eventInfo.controller_idx);
	}
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {
	if (key == OF_KEY_RIGHT) {
		controller_in_screen++;
		if (controller_in_screen == psMoveServerClient.n_controllers) controller_in_screen = 0;
	}
	else if (key == OF_KEY_LEFT) {
		controller_in_screen--;
		if (controller_in_screen < 0) controller_in_screen = psMoveServerClient.n_controllers - 1;
	}
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
