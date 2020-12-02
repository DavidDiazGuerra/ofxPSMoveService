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

		void psMoveButtonPressed(PSMoveEventInfo& eventInfo);

		void drawBarIndicator(float value, string name, ofColor color);

		ofImage image_PSMSAngles;
		ofImage image_navigationAngles;
		ofImage image_orientationVector;
		
		ofxPSMoveServiceClient psMoveServerClient;
		int controller_in_screen = 0;

		ofVec3f PSMSAngles;
		ofVec3f navigationAngles;
		ofVec3f orientationVector;
};
