#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	ofSetFrameRate(60);

	psMoveServerClient.initialize(); // Initialize the communication with PSMoveService
	psMoveServerClient.register_connected_controllers(true); // Start receiving information about the connected controllers
	psMoveServerClient.start(); // Start updating the controllers status for each new frame and generating events

	ofAddListener(psMoveServerClient.buttonPressedEvent, this, &ofApp::psMoveButtonPressed);
	ofAddListener(psMoveServerClient.buttonReleasedEvent, this, &ofApp::psMoveButtonReleased);
}

//--------------------------------------------------------------
void ofApp::update(){
	for (int controller = 0; controller < psMoveServerClient.n_controllers; controller++) {
		psMoveServerClient.set_rumble_fraction(controller, 
			(float)psMoveServerClient.get_trigger_value(controller) / 255);
	}
}

//--------------------------------------------------------------
void ofApp::draw(){
	ofBackground(54, 54, 54);

	if (psMoveServerClient.n_controllers > 0) {
		ofDrawBitmapString("Controller " + ofToString(controller_in_screen), 20, 20);
		ofDrawBitmapString("Use the arrows to change the controller.", 20, 40);

		ofDrawBitmapString("Pressed buttons:", 20, 80);
		int y_pos = 100;
		for (int button = 0; button < N_BUTTONS; button++) {
			if (psMoveServerClient.get_button_state(controller_in_screen, (PSMButton)button) == PSMButtonState_PRESSED) {
				ofDrawBitmapStringHighlight(button_names[button], 40, y_pos, ofColor::white, button_colors[button]);
				y_pos += 20;
			}
			else if (psMoveServerClient.get_button_state(controller_in_screen, (PSMButton)button) == PSMButtonState_DOWN) {
				ofDrawBitmapStringHighlight(button_names[button], 40, y_pos, ofGetBackgroundColor(), button_colors[button]);
				y_pos += 20;
			}
		}

		ofDrawBitmapString("Position:", 512, 80);
		if (psMoveServerClient.is_position_valid(controller_in_screen)) {
			ofDrawBitmapString(psMoveServerClient.get_position(controller_in_screen), 532, 100);
		}
		else ofDrawBitmapString("NO VALID", 532, 100);

		ofDrawBitmapString("Orientation:", 512, 140);
		if (psMoveServerClient.is_orientation_valid(controller_in_screen)) {
			ofDrawBitmapString(psMoveServerClient.get_orientation(controller_in_screen), 532, 160);
		}
		else ofDrawBitmapString("NO VALID", 532, 160);

		ofDrawBitmapString("Raw accelerometer:", 512, 200);
		ofDrawBitmapString(psMoveServerClient.get_raw_accelerometer(controller_in_screen), 532, 220);
		ofDrawBitmapString("Raw gyroscope:", 512, 240);
		ofDrawBitmapString(psMoveServerClient.get_raw_gyroscope(controller_in_screen), 532, 260);
		ofDrawBitmapString("Raw magnetometer:", 512, 280);
		ofDrawBitmapString(psMoveServerClient.get_raw_magnetometer(controller_in_screen), 532, 300);

		ofDrawBitmapString("Calibrated accelerometer:", 512, 340);
		ofDrawBitmapString(psMoveServerClient.get_calibrated_accelerometer(controller_in_screen), 532, 360);
		ofDrawBitmapString("Calibrated gyroscope:", 512, 380);
		ofDrawBitmapString(psMoveServerClient.get_calibrated_gyroscope(controller_in_screen), 532, 400);
		ofDrawBitmapString("Calibrated magnetometer:", 512, 420);
		ofDrawBitmapString(psMoveServerClient.get_calibrated_magnetometer(controller_in_screen), 532, 440);

		ofDrawBitmapString("Linear velocity:", 512, 480);
		ofDrawBitmapString(psMoveServerClient.get_linear_velocity(controller_in_screen), 532, 500);
		ofDrawBitmapString("Linear acceleration:", 512, 520);
		ofDrawBitmapString(psMoveServerClient.get_linear_acceleration(controller_in_screen), 532, 540);
		ofDrawBitmapString("Angular velocity:", 512, 560);
		ofDrawBitmapString(psMoveServerClient.get_angular_velocity(controller_in_screen), 532, 580);
		ofDrawBitmapString("Angular acceleration:", 512, 600);
		ofDrawBitmapString(psMoveServerClient.get_angular_acceleration(controller_in_screen), 532, 620);

	}
	else {
		ofDrawBitmapString("No controllers detected", 20, 20);
	}
}

//--------------------------------------------------------------
void ofApp::psMoveButtonPressed(PSMoveEventInfo & eventInfo) {
	if (eventInfo.button == PSMOVE_MOVE_BUTTON) {
		psMoveServerClient.set_LED_color(eventInfo.controller_idx, ofColor::white);
	} 
	else if (eventInfo.button == PSMOVE_SELECT_BUTTON) {
		psMoveServerClient.reset_orientation(eventInfo.controller_idx);
	}
}

//--------------------------------------------------------------
void ofApp::psMoveButtonReleased(PSMoveEventInfo & eventInfo) {
	if (eventInfo.button == PSMOVE_MOVE_BUTTON) {
		psMoveServerClient.reset_LED_color(eventInfo.controller_idx);
	}
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	if (key == OF_KEY_RIGHT) {
		controller_in_screen++;
		if (controller_in_screen == psMoveServerClient.n_controllers) controller_in_screen = 0;
	} else if (key == OF_KEY_LEFT) {
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

void ofApp::exit(){
	psMoveServerClient.close();
}
