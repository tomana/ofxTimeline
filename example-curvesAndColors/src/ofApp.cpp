#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	
    ofSetFrameRate(30);
    
    timeline.setWorkingFolder("temp/");
	timeline.setup();
    timeline.setFrameRate(30);
    timeline.setDurationInSeconds(60);
	timeline.setLoopType(OF_LOOP_NORMAL);


	//each call to "add keyframes" add's another track to the timeline
    timeline.addCurves("Rotate X");
    timeline.addCurves("Rotate Y");
    
	//Flags are little markers that you can attach text to
    //They are only useful when listening to bangFired() events
    //so that you know when one has passed
    timeline.addColors("Colors");

    timeline.addBangs("Bangs");

    timeline.addLFO("LFO");

    //setting framebased to true results in the timeline never skipping frames
    //and also speeding up with the 
    //try setting this to true and see the difference
    timeline.setFrameBased(false);

    //
    timeline.loadTracksFromFolder("temp/");
	
}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){
	
    ofBackground(20);
    /*ofPushMatrix();
	
    //ofPushStyle();
    //set the color to whatever the last color we encountered was
	ofSetColor(timeline.getColor("Colors"));
	
    //translate to the center of the screen
	ofTranslate(ofGetWidth()*.5, ofGetHeight()*.66, 40);
    
    //Read the values out of the timeline and use them to change the viewport rotation
    ofRotateDeg(timeline.getValue("Rotate X"), 1, 0, 0);
    ofRotateDeg(timeline.getValue("Rotate Y"), 0, 1, 0);
	
    ofDrawBox(0,0,0, 200);
	
    ofPopMatrix();*/

    //ofPopStyle();
	
    timeline.draw();

}
