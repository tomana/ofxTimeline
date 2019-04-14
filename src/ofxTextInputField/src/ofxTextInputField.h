//
//  ofxTextInputField.h
//
//  Created by Elliot Woods on 09/12/2011.
//  Copyright 2011 Kimchi and Chips.
//
//  modified by James George 12/2/2011
//  modified by Momo the Monster 7/10/2012
//  swappable fonts added by James George 9/11/2012
//
//  MIT license
//  http://www.opensource.org/licenses/mit-license.php
//

#pragma once

#include "ofMain.h"

#include "ofxFontStash.h"

#define TEXTFIELD_IS_ACTIVE "textfieldIsActive"
#define TEXTFIELD_IS_INACTIVE "textfieldIsInactive"

class ofxTextInputField {
  public:
    ofxTextInputField();
    virtual ~ofxTextInputField();
    //swap in a font!
    void setFont(ofxFontStash& font);
    
    void setup();
    
    void enable();
    void disable();
    bool getIsEnabled();
    
    bool getIsEditing();
    void beginEditing();
    void endEditing();
    
    //can be set manually or otherwise is controlled by enable/disable
    bool drawCursor;
    
    ofRectangle bounds;
    
    void draw();
    void clear();
    
    string text;
    int cursorPosition;
    
    ofEvent<string> textChanged;
    void keyPressed(ofKeyEventArgs &a);
    
    bool autoClear;
    
  protected:
    
    ofxFontStash* fontRef;
    
    bool    isEnabled;
    bool    isEditing;
    bool    mouseDownInRect;
    void    mousePressed(ofMouseEventArgs& args);
    void    mouseReleased(ofMouseEventArgs& args);
    int     cursorx, cursory;
};
