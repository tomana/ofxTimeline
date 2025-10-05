/**
 * ofxTimeline
 * openFrameworks graphical timeline addon
 *
 * Copyright (c) 2011-2012 James George
 * Development Supported by YCAM InterLab http://interlab.ycam.jp/en/
 * http://jamesgeorge.org + http://flightphase.com
 * http://github.com/obviousjim + http://github.com/flightphase
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use,
 * copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following
 * conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 *
 */

#include "ofxTLNotes.h"
#include "ofxTimeline.h"

#define NUM_NOTES 96
#define TOTAL_NOTES 128

string _notesNotation[TOTAL_NOTES] = {"C1","C#1","D1","D#1","E1","F1","F#1","G1","G#1","A1","A#1","B1",
                                    "C2","C#2","D2","D#2","E2","F2","F#2","G2","G#2","A2","A#2","B2",
                                    "C3","C#3","D3","D#3","E3","F3","F#3","G3","G#3","A3","A#3","B3",
                                    "C4","C#4","D4","D#4","E4","F4","F#4","G4","G#4","A4","A#4","B4",
                                    "C5","C#5","D5","D#5","E5","F5","F#5","G5","G#5","A5","A#5","B5",
                                    "C6","C#6","D6","D#6","E6","F6","F#6","G6","G#6","A6","A#6","B6",
                                    "C7","C#7","D7","D#7","E7","F7","F#7","G7","G#7","A7","A#7","B7",
                                    "C8","C#8","D8","D#8","E8","F8","F#8","G8","G#8","A8","A#8","B8",
                                    "C9","C#9","D9","D#9","E9","F9","F#9","G9","G#9","A9","A#9","B9",
                                    "C10","C#10","D10","D#10","E10","F10","F#10","G10","G#10","A10","A#10","B10",
                                    "C11","C#11","D11","D#11","E11","F11","F#11","G11"};

ofxTLNote::ofxTLNote(){
    isOn = false;
    wasOn = false;
    triggeredOn = false;
    triggeredOff = false;
    triggerWasRead = false;
}

ofxTLNotes::ofxTLNotes(){
	placingSwitch = NULL;
    oneArgMode = false;
    valueRange = ofRange(0,NUM_NOTES-1);

    _activeNotes = new vector<float>();
    _activeNotes->assign(NUM_NOTES,0.0f);
}

ofxTLNotes::~ofxTLNotes(){
}

void ofxTLNotes::update(){
    
    long thisUpdateSample = timeline->getCurrentTimeMillis();
    for (int i = 0; i < keyframes.size(); ++i) {
        ofxTLNote* key = (ofxTLNote*)keyframes[i];
        
        // track trigger on/off
        key->wasOn = key->isOn;
        key->isOn = key->timeRange.contains(thisUpdateSample);
        if(key->isOn && !key->wasOn){
            key->triggeredOn = true;
        }
        else if (!key->isOn && key->wasOn){
            key->triggeredOff = true;
        }
        
        // reset 'read' flag if we used it
        if (key->triggerWasRead) {
            key->triggeredOn = key->triggeredOff = false;
            key->triggerWasRead = false;
        }
        
        // grow active notes
        if(key->growing){
            key->timeRange.max = currentTrackTime();
        }
        
    }
}

void ofxTLNotes::draw(){
    
    ofPushStyle();
	ofFill();
    
    // Draw Row BGs
	ofFill();
    float rowHeight = bounds.height / (valueRange.span()+1);
	
    for (int i = 0; i <= valueRange.span(); i++) {
		// alternate row colors
		if(i%2 == 1) {
			ofSetColor(255, 255, 255, 50);
		} else {
			ofSetColor(255, 255, 255, 25);
		}
		
        // set row color for active notes
        int whichRow = ofMap(i, 0, valueRange.span(), valueRange.max, valueRange.min);
        if(pitchIsOn(whichRow)){
            ofSetColor(245);
            timeline->getFont().drawString(_notesNotation[i],bounds.x, bounds.y + i * rowHeight);
            ofSetColor(0, 0, 0, 100);
        }
        ofDrawRectangle(bounds.x, bounds.y + i * rowHeight, bounds.width, rowHeight);

        /*if(i == 1 || i == 3 || i == 6 || i == 8 || i == 10 || i == 1+12 || i == 3+12 || i == 6+12 || i == 8+12 || i == 10+12 || i == 1+24 || i == 3+24 || i == 6+24 || i == 8+24 || i == 10+24 || i == 1+36 || i == 3+36 || i == 6+36 || i == 8+36 || i == 10+36 || i == 1+48 || i == 3+48 || i == 6+48 || i == 8+48 || i == 10+48 || i == 1+60 || i == 3+60 || i == 6+60 || i == 8+60 || i == 10+60 || i == 1+72 || i == 3+72 || i == 6+72 || i == 8+72 || i == 10+72 || i == 1+84 || i == 3+84 || i == 6+84 || i == 8+84 || i == 10+84 || i == 1+96 || i == 3+96 || i == 6+96 || i == 8+96 || i == 10+96 || i == 1+108 || i == 3+108 || i == 6+108 || i == 8+108 || i == 10+108 || i == 1+120 || i == 3+120 || i == 6+120){
            ofSetColor(0);
        }else{
            ofSetColor(255);
        }
        ofDrawRectangle(bounds.x, bounds.y + i * rowHeight, 40, rowHeight);*/
	}
    
    for(int i = 0; i < keyframes.size(); i++){
        // Calculate Note Bounds
        ofxTLNote* switchKey = (ofxTLNote*)keyframes[i];
        float startScreenX = MAX(millisToScreenX(switchKey->timeRange.min), 0);
        float endScreenX = MIN(millisToScreenX(switchKey->timeRange.max), bounds.getMaxX());
		if(startScreenX == endScreenX){
			continue;
		}
        int whichRow = ofMap(switchKey->pitch, valueRange.max, valueRange.min, 0, valueRange.span());
		switchKey->display = ofRectangle(startScreenX, bounds.y + whichRow * rowHeight, endScreenX-startScreenX, rowHeight);
            
        // Drawing The Handles
        ofSetLineWidth(2);
        bool keyIsSelected = isKeyframeSelected(switchKey);
        if(keyIsSelected || switchKey->startSelected){
	        ofSetColor(timeline->getColors().textColor);
        }
        else{
	        ofSetColor(timeline->getColors().keyColor);
        }
        // Do Left Line
        ofDrawLine(switchKey->display.x, switchKey->display.y,switchKey->display.x, switchKey->display.y + switchKey->display.height);
        
        if(keyIsSelected || switchKey->endSelected){
	        ofSetColor(timeline->getColors().textColor);
        }
        else{
	        ofSetColor(timeline->getColors().keyColor);
        }
        // Do Right Line
        ofDrawLine(switchKey->display.x+switchKey->display.width,  switchKey->display.y,switchKey->display.x+switchKey->display.width, switchKey->display.y + switchKey->display.height);
        
        //draw region
        if(keyIsSelected){
        	ofSetColor(timeline->getColors().textColor, 100);
        }
        else{
        	ofSetColor(timeline->getColors().keyColor, 100);
        }
        //set overlay colors, this will override the colors above
        if(hoverKeyframe == switchKey){
            if(startHover){
                ofPushStyle();
                if(switchKey->startSelected){
                    ofSetColor(timeline->getColors().highlightColor);
                }
                else{
                    ofSetColor(timeline->getColors().keyColor);
                }
                ofDrawRectangle(switchKey->display.x-2, bounds.y, 4, bounds.height);
                ofPopStyle();
            }
            else if(endHover){
				ofPushStyle();
                if(switchKey->endSelected){
                    ofSetColor(timeline->getColors().highlightColor);
                }
                else{
                    ofSetColor(timeline->getColors().keyColor);
                }
                ofDrawRectangle(switchKey->display.x+switchKey->display.width-2, bounds.y, 4.0, bounds.height);
                ofPopStyle();
            }
            else {
                if(keyIsSelected){
	                ofSetColor(timeline->getColors().highlightColor);
                }else {
	                ofSetColor(timeline->getColors().keyColor);
                }
            }
        }
        ofDrawRectangle(switchKey->display);
    }
    ofPopStyle();
}

bool ofxTLNotes::isOnAtMillis(long millis){
    for(int i = 0; i < keyframes.size(); i++){
        ofxTLNote* switchKey = (ofxTLNote*)keyframes[i];
        if(switchKey->timeRange.min > millis){
            break;
        }
        if(switchKey->timeRange.contains(millis)){
            return true;
        }
    }
    return false;
}

bool ofxTLNotes::isOn(){
	return isOnAtMillis(currentTrackTime());
}

bool ofxTLNotes::pitchIsOnAtMillis(int pitch, long millis){
    for(int i = 0; i < keyframes.size(); i++){
        ofxTLNote* switchKey = (ofxTLNote*)keyframes[i];
        if(switchKey->timeRange.min > millis){
            break;
        }
        if(switchKey->timeRange.contains(millis) && switchKey->pitch == pitch){
            return true;
        }
    }
    return false;
}

vector<float>* ofxTLNotes::getActiveNotes(){
    for (int i = 0; i <= valueRange.span(); i++) {
        int whichRow = ofMap(i, 0, valueRange.span(), valueRange.max, valueRange.min);
        if(pitchIsOn(whichRow)){
            _activeNotes->at(whichRow) = 1.0f;
        }else{
            _activeNotes->at(whichRow) = 0.0f;
        }
    }
    reverse(_activeNotes->begin(),_activeNotes->end());
    return _activeNotes;
}

bool ofxTLNotes::pitchIsOn(int pitch){
    return pitchIsOnAtMillis(pitch, currentTrackTime());
}

bool ofxTLNotes::isOnAtPercent(float percent){
    unsigned long millis = percent*timeline->getDurationInMilliseconds();
    return isOnAtMillis(millis);
}

bool ofxTLNotes::mousePressed(ofMouseEventArgs& args, long millis){
    
    // if you're placing a switch and you just pressed the left button
    // then set its end time to the pressed location
    // if you're placing a switch and you pressed any other button, toss it and clean up
	if(placingSwitch != NULL){
		if(isActive() && args.button == 0){
			placingSwitch->timeRange.max = millis;
			updateTimeRanges();
		}
		else {
			deleteKeyframe(placingSwitch);
		}
		placingSwitch = NULL;
		return false;
	}
	
    keysAreDraggable = !ofGetKeyPressed(OF_KEY_SHIFT);

    /*if(!keysAreDraggable){
        int pitch = pitchForScreenY(args.y);
        int velocity = 50;
        float startTime = screenXtoNormalizedX(args.x, zoomBounds);
        addKeyframeAtMillis(pitch, velocity,startTime);
    }*/
	
    //check to see if we are close to any edges, if so select them
    bool startSelected = false;
    bool endSelected = false;
    int selectedKeyframeIndex;
    if(isActive() && args.button == 0){
        for(int i = 0; i < keyframes.size(); i++){
            
            ofxTLNote* switchKey = (ofxTLNote*)keyframes[i];
            //unselect everything else if we just clicked this edge without shift held down
            startSelected = (abs(switchKey->display.x - args.x) < 10.0 && switchKey->display.inside(args.x,args.y));
            if (startSelected && !switchKey->startSelected && !ofGetKeyPressed(OF_KEY_SHIFT)) {
                timeline->unselectAll();
            }
            //Deselect the key if we clicked it already selected with shift held down
            if(ofGetKeyPressed(OF_KEY_SHIFT) && ((startSelected && switchKey->startSelected) || isKeyframeSelected(switchKey))){
                switchKey->startSelected = false;
            }
            else {
                switchKey->startSelected |= startSelected;
            }
            float endEdge = switchKey->display.x+switchKey->display.width;
            endSelected = (abs(endEdge - args.x) < 10.0 && switchKey->display.inside(args.x,args.y));
            //don't let them both be selected in one click!
            if(!startSelected && endSelected && !switchKey->endSelected && !ofGetKeyPressed(OF_KEY_SHIFT)){
                timeline->unselectAll();
            }
            //Deselect the key if we clicked it already selected with shift held down
            if(ofGetKeyPressed(OF_KEY_SHIFT) && ((endSelected && switchKey->endSelected) || isKeyframeSelected(switchKey))){
                switchKey->endSelected = false;
            }
            else{
                switchKey->endSelected |= endSelected && !startSelected;
            }
            
            if(startSelected || endSelected){
				selectedKeyframeIndex = i;
                break;
            }
        }
    }
    
    //update dragging and snapping if we clicked an edge
    updateEdgeDragOffsets(millis);
    if(endSelected || startSelected){
        ofxTLNote* selectedSwitch = (ofxTLNote*)keyframes[selectedKeyframeIndex];
        timeline->setDragTimeOffset(selectedSwitch->edgeDragOffset);
    }
	
    if(!endSelected && !startSelected){
    	//normal selection from above
	    ofxTLKeyframes::mousePressed(args, millis);
        if(isActive()){
	        timeline->cancelSnapping(); //don't snap when dragging the whole switch
        }
    }
    
    //move through the keyframes, if both the start and the end have been selected
    //count it as completely selected and let the super class take care of it
    //otherwise if just one of the edges are selected make sure it's unselected
    for(int i = 0; i < keyframes.size(); i++){
        ofxTLNote* switchKey = (ofxTLNote*)keyframes[i];
        if (switchKey->startSelected && switchKey->endSelected) {
            switchKey->startSelected = switchKey->endSelected = false;
            selectKeyframe(switchKey);
        }
        //make sure that if just one of the edges is clicked that the keyframe is *not* selected
		//also make sure it wasn't *just* selected in the last click by checking that it's not 'the' selected key
        else if( (switchKey->startSelected || switchKey->endSelected) && isKeyframeSelected(switchKey)){
			if(selectedKeyframe == switchKey){
				switchKey->startSelected = switchKey->endSelected = false;
			}
			else{
	            deselectKeyframe(switchKey);
			}
        }
    }
	return false;
}

void ofxTLNotes::unselectAll(){
    ofxTLKeyframes::unselectAll();
    for(int i = 0; i < keyframes.size(); i++){
        ofxTLNote* switchKey = (ofxTLNote*)keyframes[i];
        switchKey->startSelected = switchKey->endSelected = false;
    }
}

void ofxTLNotes::updateEdgeDragOffsets(long clickMillis){
    for(int i = 0; i < keyframes.size(); i++){
        ofxTLNote* switchKey = (ofxTLNote*)keyframes[i];
    	if(switchKey->startSelected){
            switchKey->edgeDragOffset = clickMillis - switchKey->timeRange.min;
        }
        if( switchKey->endSelected){
            switchKey->edgeDragOffset = clickMillis - switchKey->timeRange.max;
        }
    }
}

//TODO: make range select respect rows separate from time
void ofxTLNotes::mouseDragged(ofMouseEventArgs& args, long millis){
    
    //do the normal dragging behavior for selected keyframes
    ofxTLKeyframes::mouseDragged(args, millis);
	
	if(keysAreDraggable){
		//look for any keys with just beginning and ends selected
		//becaues of the logical in the mousePressed, there will never
		//be a selected keyframe with an end selected
		for(int i = 0; i < keyframes.size(); i++){
			ofxTLNote* switchKey = (ofxTLNote*)keyframes[i];
			if(switchKey->startSelected){
				switchKey->timeRange.min = millis - switchKey->edgeDragOffset;
				switchKey->time = switchKey->timeRange.min;
			}
			else if(switchKey->endSelected){
				switchKey->timeRange.max = millis - switchKey->edgeDragOffset;
			}
		}
		
		updateTimeRanges();
	}
}

void ofxTLNotes::mouseMoved(ofMouseEventArgs& args, long millis){
    endHover = startHover = false;
    if(hover && placingSwitch != NULL){
		placingSwitch->timeRange.max = millis;
		return;
	}
	
    for(int i = 0; i < keyframes.size(); i++){
        ofxTLNote* switchKey = (ofxTLNote*)keyframes[i];
        if(abs(switchKey->display.x - args.x) < 10.0 && switchKey->display.inside(args.x,args.y)){
            hoverKeyframe = switchKey;
            startHover = true;
            return; //return cancels call to parent
        }
        float endEdge = switchKey->display.x+switchKey->display.width;
        if(abs(endEdge - args.x) < 10.0 && switchKey->display.inside(args.x,args.y)){
            hoverKeyframe = switchKey;
            endHover = true;
            return; //cancels call to parent
        }
    }
    ofxTLKeyframes::mouseMoved(args, millis);
}

void ofxTLNotes::nudgeBy(ofVec2f nudgePercent){
	//super class nudge for selected keys
	ofxTLKeyframes::nudgeBy(nudgePercent);
	
	for(int i = 0; i < keyframes.size(); i++){
        ofxTLNote* switchKey = (ofxTLNote*)keyframes[i];
        if(switchKey->startSelected){
            switchKey->timeRange.min += nudgePercent.x*timeline->getDurationInMilliseconds();
            switchKey->time = switchKey->timeRange.min;
        }
        else if(switchKey->endSelected){
            switchKey->timeRange.max += nudgePercent.x*timeline->getDurationInMilliseconds();
		}
	}
	
	updateTimeRanges();
}

//needed to sync the time ranges from pasted keys
void ofxTLNotes::pasteSent(string pasteboard){
	ofxTLKeyframes::pasteSent(pasteboard);
	updateTimeRanges();
}

//This is called after dragging or nudging, and let's us make sure
void ofxTLNotes::updateTimeRanges(){
	
    //the superclass will move the ->time value with the drag
    //so we look at the selected keyframes values and see if their changed
    //if so update both the min and the max time so the control moves as a block
	for(int i = 0; i < selectedKeyframes.size(); i++){
        ofxTLNote* switchKey = (ofxTLNote*)selectedKeyframes[i];
        long dif = switchKey->time - switchKey->timeRange.min;
        switchKey->timeRange.min = switchKey->time;
        switchKey->timeRange.max += dif;
    }
    
	//we also want ot make sure that any nudges or drags that happened to only edge values
	//reversed the min/max relationship value and swap them really quick
    for(int i = 0; i < keyframes.size(); i++){
        //check to see if the user reversed the value and swap them really quick
        ofxTLNote* switchKey = (ofxTLNote*)keyframes[i];
		if(switchKey->timeRange.min > switchKey->timeRange.max){
            float tempPos = switchKey->timeRange.max;
            switchKey->timeRange.max = switchKey->timeRange.min;
            switchKey->timeRange.min = switchKey->time = tempPos;
            bool tempSelect = switchKey->startSelected;
            switchKey->startSelected = switchKey->endSelected;
            switchKey->endSelected = tempSelect;
        }
    }
	
    //TODO: no overlaps!!
}

void ofxTLNotes::mouseReleased(ofMouseEventArgs& args, long millis){
	ofxTLKeyframes::mouseReleased(args, millis);
}

void ofxTLNotes::regionSelected(ofLongRange timeRange, ofRange valueRange){
    for(int i = 0; i < keyframes.size(); i++){
        ofxTLNote* switchKey = (ofxTLNote*)keyframes[i];
    	if(switchKey->timeRange.intersects(timeRange)){
            selectKeyframe(switchKey);
        }
    }
}

void ofxTLNotes::getSnappingPoints(std::set<unsigned long>& points){
	for(int i = 0; i < keyframes.size(); i++){
        ofxTLNote* switchKey = (ofxTLNote*)keyframes[i];
		if (isKeyframeIsInBounds(switchKey) && !isKeyframeSelected(switchKey) &&
            !switchKey->startSelected && !switchKey->endSelected) {
			points.insert(switchKey->timeRange.min);
            points.insert(switchKey->timeRange.max);
		}
	}
}

int ofxTLNotes::getSelectedItemCount(){
	int numEdgesSelected = 0;
	for(int i = 0; i < keyframes.size(); i++){
        ofxTLNote* switchKey = (ofxTLNote*)keyframes[i];
		if(switchKey->startSelected ||switchKey->endSelected){
			numEdgesSelected++;
		}
	}
	return ofxTLKeyframes::getSelectedItemCount() + numEdgesSelected;
}

ofxTLKeyframe* ofxTLNotes::newKeyframe(){
    ofxTLNote* switchKey = new ofxTLNote();
    //in the case of a click, start at the mouse positiion
    //if this is being restored from XML, the next call to restore will override this with what is in the XML
    switchKey->timeRange.min = switchKey->timeRange.max = screenXToMillis(ofGetMouseX());
    switchKey->startSelected = false;
    switchKey->endSelected   = true; //true so you can drag the range to start with
	
	//for just placing a switch we'll be able to decide the end position
	placingSwitch = switchKey;
    
    // add pitch based on mouseY if that's the input device
    if(createNewOnMouseup){
        switchKey->pitch = pitchForScreenY(ofGetMouseY());
    }
	
    return switchKey;
}

void ofxTLNotes::restoreKeyframe(ofxTLKeyframe* key, ofxXmlSettings& xmlStore){
    //pull the saved time into min, and our custom max value
    ofxTLNote* switchKey = (ofxTLNote*)key;
    // TODO: reset function for note?
    switchKey->wasOn = false;
    switchKey->isOn = false;
    switchKey->triggeredOn = false;
    switchKey->triggeredOff = false;
    switchKey->growing = false;
    
    switchKey->timeRange.min = switchKey->time;
    //
    string timecode = xmlStore.getValue("max", "00:00:00:000");
    if(timecode.find(":") == string::npos){
        switchKey->timeRange.max = ofToFloat(timecode) * timeline->getDurationInMilliseconds(); //Legacy max of 0-1
    }
    else{
		switchKey->timeRange.max = timeline->getTimecode().millisForTimecode(timecode);
    }
    //this is so freshly restored keys won't have ends selected but click keys will
    switchKey->startSelected = switchKey->endSelected = false;
    
    // restore pitch
    int pitch = xmlStore.getValue("pitch", 0);
    switchKey->pitch = pitch;
    
    float velocity = xmlStore.getValue("velocity", 0.0f);
    switchKey->velocity = velocity;
	
	//a bit of a hack, but if
	placingSwitch = NULL;
    trimToPitches();
}

void ofxTLNotes::storeKeyframe(ofxTLKeyframe* key, ofxXmlSettings& xmlStore){
    //push the time range into X/Y
    ofxTLNote* switchKey = (ofxTLNote* )key;
    switchKey->time = switchKey->timeRange.min;
	xmlStore.addValue("max", timeline->getTimecode().timecodeForMillis(switchKey->timeRange.max));
    xmlStore.addValue("pitch", switchKey->pitch);
    xmlStore.addValue("velocity", switchKey->velocity);
}

ofxTLKeyframe* ofxTLNotes::keyframeAtScreenpoint(ofVec2f p){
	for(int i = 0; i < keyframes.size(); i++){
		ofxTLNote* switchKey = (ofxTLNote*)keyframes[i];
    	if(switchKey->display.inside(p)){
            return switchKey;
        }
    }
    return NULL;
}

string ofxTLNotes::getTrackType(){
    return "Notes";
}

#pragma mark NOTE METHODS
int ofxTLNotes::pitchForScreenY(int y) {
	float normalizedY = (y - bounds.y) / bounds.height;
	int pitch = ofClamp(ofMap(normalizedY, 1, 0, valueRange.min, valueRange.max + 1), valueRange.min, valueRange.max); // clamp to range
	return pitch;
}


void ofxTLNotes::addKeyframeAtMillis(int pitch, float velocity, unsigned long millis, bool isGrowing){
	ofxTLNote* key = (ofxTLNote*)newKeyframe();
	key->time = key->previousTime = millis;
    key->timeRange.min = millis;
    key->timeRange.max = millis + 100;
    key->pitch = pitch;
    key->velocity = velocity;
    cout << "added keyframe with velocity " << key->velocity << endl;
	key->value = ofMap(pitch, valueRange.min, valueRange.max, 0, 1, true);
    key->growing = isGrowing;
	keyframes.push_back(key);
	//smart sort, only sort if not added to end
	if(keyframes.size() > 2 && keyframes[keyframes.size()-2]->time > keyframes[keyframes.size()-1]->time){
		updateKeyframeSort();
	}
    trimToPitches();
	lastKeyframeIndex = 1;
	timeline->flagTrackModified(this);
	shouldRecomputePreviews = true;
}

void ofxTLNotes::finishNote(int pitch){
    for (int i = 0; i < keyframes.size(); ++i) {
        ofxTLNote* key = (ofxTLNote*)keyframes[i];
        int diff = key->pitch - pitch;
        if(key->growing && key->pitch == pitch){
            key->growing = false;                           // stop growing
            key->endSelected = key->startSelected = false;  // deselect
            placingSwitch = NULL;
        }
    }
}

void ofxTLNotes::playbackStarted(ofxTLPlaybackEventArgs &args){

}

void ofxTLNotes::playbackLooped(ofxTLPlaybackEventArgs &args){
    for(int i = 0; i < keyframes.size(); i++){
		ofxTLNote* switchKey = (ofxTLNote*)keyframes[i];
    	if(switchKey->growing){
            switchKey->growing = false;
            switchKey->endSelected = switchKey->startSelected = false;
            switchKey->timeRange.max = getTimeline()->getOutTimeInMillis();
        }
        
    }
}

void ofxTLNotes::trimToPitches(){
    if(keyframes.size() == 0) return;
    // set range to first note pitch
    ofxTLNote* switchKey = (ofxTLNote*)keyframes[0];
    ofRange newRange = ofRange(switchKey->pitch, switchKey->pitch);
    // iterate through all keys, expand to fit pitches
    for(int i = 1; i < keyframes.size(); i++){
		switchKey = (ofxTLNote*)keyframes[i];
        newRange.min = MIN(newRange.min, switchKey->pitch);
        newRange.max = MAX(newRange.max, switchKey->pitch);
    }
    setValueRange(newRange);
    
    // fix values of each keyframes
    for (int i = 0; i < keyframes.size(); ++i) {
        switchKey = (ofxTLNote*)keyframes[i];
        switchKey->value = ofMap(switchKey->pitch, valueRange.min, valueRange.max, 0, 1);
    }
}

vector<ofxTLNote*> ofxTLNotes::getDirtyNotes(){
    vector<ofxTLNote*>notes;
    for (int i = 0; i < keyframes.size(); ++i) {
        ofxTLNote* sourceKey = (ofxTLNote*)keyframes[i];
        if(sourceKey->triggeredOff || sourceKey->triggeredOn){
            if(sourceKey->triggerWasRead == false){
                notes.push_back(sourceKey);
                sourceKey->triggerWasRead = true;
            }
        }
    }
    return notes;
}
