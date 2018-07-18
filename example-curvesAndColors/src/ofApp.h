#pragma once

#include "ofMain.h"
#include "ofxTimeline.h"

class ofApp : public ofBaseApp{

  public:
        void setup();
        void update();
        void draw();

        ofLight light;
	
        ofxTimeline timeline;
    
};
