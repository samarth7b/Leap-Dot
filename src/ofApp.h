#pragma once

#include "ofMain.h"
#include "ofxLeapMotion.h"
#include "ofxStrip.h"

typedef struct {
    string name;
    float latitude;
    float longitude;
} TrottingPoint;

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
    
    ofEasyCam camera;
    
    ofLight light;
    
    //leap motion
    ofxLeapMotion leap;
    vector <ofxLeapMotionSimpleHand> simpleHands;
    
    vector <int> fingersFound;
    ofEasyCam cam;
    ofLight l1;
    ofLight l2;
    ofMaterial m1;
    
    map <int, ofPolyline> fingerTrails;

    vector<TrottingPoint> points;
    vector<ofVec3f> pointTrail;
    int frameCheckpoint, colorFlag, radius, fingerCount, alphaValue;
};
