//
//  CustomRect.h
//  emptyExample
//
//  Created by Ben McChesney on 2/26/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#include "ofMain.h"
#include "PixelHitTestEvent.h"

#ifndef emptyExample_CustomRect_h
#define emptyExample_CustomRect_h


class CustomRect
{
    public :
        CustomRect ( ) 
        {
            rotation = ofRandom ( 0 , 360 ) ; 
        } 
    
    
        void setup ( ofRectangle _bounds , ofVec2f _position , ofColor _color , int _inputHexMap ) 
        {
            bounds = _bounds ; 
            position = _position ; 
            color = _color ; 
            inputHexMap = _inputHexMap ; 
            scale = 1.0f ; 
            
            ofAddListener( PixelHitTestEvent::Instance()->PIXEL_OUT, this , &CustomRect::pixelOutHandler ) ; 
            ofAddListener( PixelHitTestEvent::Instance()->PIXEL_OVER, this , &CustomRect::pixelOverHandler ) ; 
            ofAddListener( PixelHitTestEvent::Instance()->PIXEL_CLICK , this , &CustomRect::pixelClickHandler ) ; 
        } 
    
    
        void pixelClickHandler( int &inputHex ) 
        {
            if ( inputHex == inputHexMap ) 
            {
                color = ofColor ( ofRandom ( 255 ) , ofRandom ( 255 ) , ofRandom ( 255 ) ) ; 
                position = ofVec2f( ofRandom( 100 , ofGetWidth() - 100 ) , ofRandom ( 100 , ofGetHeight() - 100 ) ) ; 
                rotation = ofRandom ( 0 , 360 ) ; 
            }
        }
    
        void pixelOutHandler ( int &inputHex ) 
        { 
            //cout << "OUT  does : " << inputHex << " == " << inputHexMap << endl ; 
            if ( inputHex == inputHexMap ) 
            {
                scale = 1.0f ; 
            }
        }
    
        void pixelOverHandler ( int &inputHex ) 
        {
           // cout << "OVER does : " << inputHex << " == " << inputHexMap << endl ; 
            if ( inputHex == inputHexMap ) 
            {
                cout << "match found! : " << inputHex << endl ;
                scale = 1.5f ; 
            }
        }
    
        void draw ( ) 
        {
            //This will branch the current global transformation matrix
            ofPushMatrix( ) ;   //anything done in between push / pop could be thought of as "local"
                ofTranslate ( position.x , position.y , 0 ) ;           //Move to the center of our square
                ofRotate( rotation ) ;                                  //rotation from the center
                ofScale ( scale , scale , 1 ) ; 
            
                ofSetColor ( color ) ; 
                ofRect ( bounds ) ;                                //Use bounds to draw rectangle
            
                ofSetColor ( 75 , 75 , 75 )  ; 
                //Lets add another cross section
                ofRotate( 90 ) ;                        //add an additional quarter-turn to rotation
                ofRect ( -5 , -10 , 10 , 20 ) ;         //x , y , width , height
            
            ofPopMatrix( ) ;    //End the local transformation
        }
    
        void drawMap ( ) 
        {
            ofPushMatrix( ) ;   //anything done in between push / pop could be thought of as "local"
                ofTranslate ( position.x , position.y , 0 ) ;           //Move to the center of our square
                ofRotate( rotation ) ;                                  //rotation from the center
                ofScale ( scale , scale , 1 ) ; 
            
                ofSetHexColor ( inputHexMap ) ;
                ofRect ( bounds ) ;   
            ofPopMatrix ( ) ; 
        }
    
        ofRectangle bounds ;
        ofColor color ; 
        ofVec2f position ; 
        float rotation ; 
        float scale ; 
    
        int inputHexMap ;
        

};

#endif
