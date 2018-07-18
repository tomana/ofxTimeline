#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	
	ofSetFrameRate(60);
	ofSetVerticalSync(true);
    ofEnableSmoothing();
	
	glEnable(GL_DEPTH_TEST);
	ofEnableLighting();
	
	light.setPosition(ofGetWidth()*.5, ofGetHeight()*.25, 0);
	light.enable();
    
	timeline.setup();
    timeline.setFrameRate(30);
	timeline.setDurationInFrames(90);
	timeline.setLoopType(OF_LOOP_NORMAL);
    
	//each call to "add keyframes" add's another track to the timeline
	timeline.addCurves("Rotate X", ofRange(0, 360));
	timeline.addCurves("Rotate Y", ofRange(0, 360));
    
	//Flags are little markers that you can attach text to
    //They are only useful when listening to bangFired() events
    //so that you know when one has passed
	timeline.addColors("Colors");

    //setting framebased to true results in the timeline never skipping frames
    //and also speeding up with the 
    //try setting this to true and see the difference
    timeline.setFrameBased(false);
	
}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){
	
	ofBackground(.15*255);
	ofPushMatrix();
	
    //ofPushStyle();
    //set the color to whatever the last color we encountered was
	ofSetColor(timeline.getColor("Colors"));
	
    //translate to the center of the screen
	ofTranslate(ofGetWidth()*.5, ofGetHeight()*.66, 40);
    
    //Read the values out of the timeline and use them to change the viewport rotation
    ofRotateDeg(timeline.getValue("Rotate X"), 1, 0, 0);
    ofRotateDeg(timeline.getValue("Rotate Y"), 0, 1, 0);
	
    ofDrawBox(0,0,0, 200);
	
	ofPopMatrix();

    //ofPopStyle();
	
	timeline.draw();
}
	
