#pragma once

#include "ofMain.h"
#include "ofxTimeline.h"
#include "ofxDatGui.h"

class ofApp : public ofBaseApp{

  public:
        void setup();
        void update();
        void draw();
	
        ofxTimeline timeline;
    
};
