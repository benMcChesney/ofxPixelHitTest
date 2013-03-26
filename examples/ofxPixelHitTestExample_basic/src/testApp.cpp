#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup()
{
    ofBackground ( 0 , 0 , 0 ) ; 
    ofSetVerticalSync ( true ) ;   
    ofEnableAlphaBlending() ; 
    
    ofRectangle rectBounds = ofRectangle( -75 , -75 , 150 , 150 ) ; 

    //This is the main performance point, raising the last two paramters will significanlty boost performance
    //but they will be slightly less accurate per frame increment
    pixelHitTest.setup( ofGetWidth() , ofGetHeight() , 0x000000 , 6 , 4 ) ; 
    
    for ( int i = 0 ; i < 6 ; i++ ) 
    {
        CustomRect* rect = new CustomRect() ; 
        ofVec2f randomPosition = ofVec2f ( ofRandom ( 100 , ofGetWidth() - 100 ) , 
                                           ofRandom ( 100 , ofGetHeight() - 100 ) ) ;
        
        ofColor randomColor = ofColor::fromHex( ofRandom(0xFFFFFF) ) ; 
        rect->setup( rectBounds, randomPosition, randomColor , pixelHitTest.getColorfulUniqueHex() ) ;  
        rects.push_back( rect ) ; 
    }
}

//--------------------------------------------------------------
void testApp::update(){
    int hex = pixelHitTest.getHexAt( ofVec2f( mouseX , mouseY ) ) ; 
    
    ofSetWindowTitle( ofToString( ofGetFrameRate() ) ) ; 
}

//--------------------------------------------------------------
void testApp::draw()
{
    //If the frame increment matches we update the input map
    if ( pixelHitTest.beginFbo() == true ) 
    {
        for ( int a = 0 ; a < rects.size() ; a++ ) 
        {
            rects[a]->drawMap() ;        //draw the input map layer
        }
        pixelHitTest.endFbo() ;         //stop drawing the map
    }
    
    ofBackground(0, 0, 0) ; 
    //Draw the visual rects
    for ( int i = 0 ; i < rects.size() ; i++ ) 
    {
        rects[i]->draw( ) ; 
    }
       
   pixelHitTest.drawOverlay() ;     //Draws a "minimap" to help with debugging
   pixelHitTest.drawMap() ;        //Draws an overlay
    
    ofSetColor ( 255 , 255 , 255 ) ; 
    ofDrawBitmapString( "'M' Draw Map ? - " + ofToString ( pixelHitTest.bDebugDraw ) , 15 , 700 ) ;
    ofDrawBitmapString( "'O' Draw Overlay ? - " + ofToString ( pixelHitTest.bDebugOverlay ) , 15 , 725 ) ;
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
    switch( key ) 
    {
        case 'M':
        case 'm':
            pixelHitTest.bDebugDraw = !pixelHitTest.bDebugDraw ; 
            break ; 
            
        case 'o':
        case 'O':
            pixelHitTest.bDebugOverlay = !pixelHitTest.bDebugOverlay ; 
            break ; 
    }
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
    
    int hex = pixelHitTest.getHexAt(ofVec2f( mouseX , mouseY ) ) ; 
    ofNotifyEvent ( PixelHitTestEvent::Instance()->PIXEL_CLICK , hex , this ) ; 
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){ 

}