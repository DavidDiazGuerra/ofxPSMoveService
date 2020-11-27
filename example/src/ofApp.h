#pragma once

#include "ofMain.h"
#include "ofxPSMoveService.h"

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
		void exit();

		void psMoveButtonPressed(PSMoveEventInfo & eventInfo);
		void psMoveButtonReleased(PSMoveEventInfo & eventInfo);

		ofxPSMoveServiceClient psMoveServerClient;

		int controller_in_screen = 0;

		char *button_names[N_BUTTONS] = { 
			"Triangle", 
			"Circle", 
			"Cross", 
			"Square", 
			"SELECT", 
			"START", 
			"PS", 
			"Move", 
			"Trigger" 
		};
		ofColor button_colors[N_BUTTONS] = { 
			ofColor::lawnGreen, 
			ofColor::indianRed, 
			ofColor::deepSkyBlue, 
			ofColor::deepPink, 
			ofColor::white,
			ofColor::white, 
			ofColor::white, 
			ofColor::white, 
			ofColor::white
		};
};
