#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    
    ofSetFrameRate(30);
    ofEnableAlphaBlending();
    ofSetBackgroundAuto(false);
    ofBackground(0);
    ofNoFill();
    
    TrottingPoint samplePoint = { "samplePoint", 0, 20};
    points.push_back(samplePoint);
    
    frameCheckpoint = 0;
    colorFlag = 0;
    
    ofSetVerticalSync(true);
    ofSetLogLevel(OF_LOG_VERBOSE);
    
    //start leap
    leap.open();
    
    l1.setPosition(20, 30, 5);
    l2.setPosition(-20, -20, 5);
    
    camera.setPosition(ofVec3f(0, 0, -500.f));
    
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_NORMALIZE);
    
}

//--------------------------------------------------------------
void ofApp::update(){
    
    //leap start
    
    fingersFound.clear();
    
    simpleHands = leap.getSimpleHands();
    
    if( leap.isFrameNew() && simpleHands.size() ){
        
        leap.setMappingX(-230, 230, -ofGetWidth()/2, ofGetWidth()/2);
        leap.setMappingY(90, 490, -ofGetHeight()/2, ofGetHeight()/2);
        leap.setMappingZ(-150, 150, -200, 200);
        
        for(int i = 0; i < simpleHands.size(); i++){
            
            for(int j = 0; j < simpleHands[i].fingers.size(); j++){
                int id = simpleHands[i].fingers[j].id;
                
                ofPolyline & polyline = fingerTrails[id];
                ofPoint pt = simpleHands[i].fingers[j].pos;
                
                //if the distance between the last point and the current point is too big - lets clear the line
                //this stops us connecting to an old drawing
                if( polyline.size() && (pt-polyline[polyline.size()-1] ).length() > 50 ){
                    polyline.clear();
                }
                
                //add our point to our trail
                polyline.addVertex(pt);
                
                //store fingers seen this frame for drawing
                fingersFound.push_back(id);
            }
        }
    }
    
    if (simpleHands.size() > 0) {
        float xValue = simpleHands[0].handNormal.x;
        float yValue = simpleHands[0].handNormal.y;
        if (xValue < 0.f) {
            for (int i = 0; i < points.size(); i++) {
                if (points[i].longitude > -90) {
                    points[i].longitude--;
                }
            }
        } else {
            for (int i = 0; i < points.size(); i++) {
                if (points[i].longitude < 90) {
                    points[i].longitude++;
                }
            }
        }
        if (yValue < -0.5) {
            radius = 1;
        } else if (yValue < -0.4) {
            radius = 2;
        } else if (yValue < -0.3) {
            radius = 3;
        } else if (yValue < -0.2) {
            radius = 4;
        } else {
            radius = 5;
        }
    }
    
    if (simpleHands.size() > 0) {
        fingerCount = 0;
        for (int i = 0; i < simpleHands[0].fingers.size(); i++) {
            if(ofVec3f(0, 0, 0).distance(simpleHands[0].handPos - simpleHands[0].fingers[i].pos) > 100) {
                fingerCount++;
            }
        }
    } else {
        fingerCount = 0;
        radius = 1;
    }
    
    leap.markFrameAsOld();
}

//--------------------------------------------------------------
void ofApp::draw(){
    
    ofPushMatrix();
    
    //translate so that the center of the screen is 0,0
    ofTranslate(ofGetWidth()/2, ofGetHeight()/2);
    ofSetColor(255, 255, 255, 20);
    
    //draw a translucent wireframe sphere (ofNoFill() is on)
    //ofPushMatrix();
    //add an extra spin at the rate of 1 degree per frame
    //ofRotate(ofGetFrameNum(), 0, 1, 0);
    ofDrawSphere(0, 0, 0, 300);
    //ofPopMatrix();
    
    ofSetColor(255);
    for(unsigned int i = 0; i < points.size(); i++){
        
        /*cities[i].latitude += 1;
        if (cities[i].longitude >= 10) {
            cities[i].longitude = -10;
        }*/
        
        //three rotations
        //two to represent the latitude and lontitude of the city
        //a third so that it spins along with the spinning sphere
        ofQuaternion latRot, longRot, spinQuat;
        //latRot.makeRotate(10, 1, 0, 0);
        longRot.makeRotate(points[i].longitude, 0, 1, 0);
        spinQuat.makeRotate(ofGetFrameNum(), -1, 0, 0);
        
        //our starting point is 0,0, on the surface of our sphere, this is where the meridian and equator meet
        ofVec3f center = ofVec3f(0,0,300);
        //multiplying a quat with another quat combines their rotations into one quat
        //multiplying a quat to a vector applies the quat's rotation to that vector
        //so to to generate our point on the sphere, multiply all of our quaternions together then multiple the centery by the combined rotation
        ofVec3f worldPoint = latRot * longRot * spinQuat * center;
        /*if (ofGetFrameNum() - frameCheckpoint > 100) {
            frameCheckpoint = ofGetFrameNum();
            colorFlag = (colorFlag + 1) % 4;
        }*/
        if (ofGetFrameNum() % 360 > 75 && ofGetFrameNum() % 360 < 285) {
            alphaValue = 255;//change to 127 to make drawings in background transluscent
        } else {
            alphaValue = 255;
        }
        ofSetColor(alphaValue);

        switch (fingerCount) {
            case 0:
                ofSetColor(alphaValue);
                break;
            case 1:
                ofSetColor(0, 255, 0, alphaValue);
                break;
            case 2:
                ofSetColor(0, 255, 0, alphaValue);
                break;
            case 3:
                ofSetColor(0, 127, 127, alphaValue);
                break;
            case 4:
                ofSetColor(0, 127, 127, alphaValue);
                break;
            case 5:
                ofSetColor(127, 0, 127, alphaValue);
                break;
            default:
                ofSetColor(alphaValue);
                break;
        }
        ofFill();
        ofDrawCircle(worldPoint, radius);
        ofNoFill();
        
    }
    
    if (simpleHands.size() > 0) {
        ofSetColor(255);
        //ofDrawBitmapString("Hand Normal:" + ofToString(simpleHands[0].handNormal.x) + " " + ofToString(simpleHands[0].handNormal.y) + " " + ofToString(simpleHands[0].handNormal.z), 20, 20);
    }
    
    ofPopMatrix();

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    
    if (key == ' ') {
        ofBackground(0);
        for (int i = 0; i < points.size(); i++) {
            points[i].longitude = 20;
        }
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
    leap.close();
}