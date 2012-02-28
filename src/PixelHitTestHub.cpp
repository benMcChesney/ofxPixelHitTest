#include "PixelHitTestHub.h"

PixelHitTestHub::PixelHitTestHub()
{

}

PixelHitTestHub::~PixelHitTestHub()
{
    //dtor

}

int PixelHitTestHub::getHexAt( ofVec2f input )
{
	//Adjust our input by the sampling Rate
	ofVec2f adjustedInput = ofVec2f ( (int)input.x / mapSampling , (int)input.y / mapSampling ) ; 
	//cout << "adjusted input : " << adjustedInput.x << " , " << adjustedInput.y << endl ;

    //Calculate if we are within bounds
    if ( adjustedInput.x > 0 && adjustedInput.x < maxBounds.width && adjustedInput.y > 0 && adjustedInput.y < maxBounds.height ) 
	{
        //(x + ( y * width )) * numberColorChannels 
		int index = (adjustedInput.x + adjustedInput.y * map.getWidth()) * 3 ; 
        //store the old hex for comparison
        int oldHex = lastMapHex ; 
        //we use the R + G + B channels to get the color
		lastMapHex = ofColor( mapPixels[index] , mapPixels[index + 1] , mapPixels[index + 2] ).getHex() ; 
        
        //Pixel changed, need to call pixel_out on the last value
        if ( oldHex != lastMapHex ) 
            ofNotifyEvent( PixelHitTestEvent::Instance()->PIXEL_OUT , oldHex , this ) ; 
        //New item ! call pixel_over
        else
            ofNotifyEvent( PixelHitTestEvent::Instance()->PIXEL_OVER, lastMapHex, this ) ; 
        
        return lastMapHex ; 
	}
	else
	{
        lastMapHex = backgroundHex ; 
		return backgroundHex ; 
	}
 }

bool PixelHitTestHub::beginFbo (  )
{
    //TODO: faster way than mod ?
    //If it matches up to frameIncrement
    if ( ofGetFrameNum() % captureIncrement == 0 ) 
    {
        //Record the FBO !
        map.begin( ) ;
		ofPushMatrix() ; 
		ofScale( mapScale , mapScale , 1 ) ; 
        //Clear background
        ofSetHexColor ( backgroundHex ) ;
        ofRect ( 0 , 0 , ofGetWidth() , ofGetHeight() ) ;
        return true ; 
    }
    else
    {
	
        return false ; 
    }
  
}

void PixelHitTestHub::endFbo ( )
{
	ofPopMatrix() ; 
    map.end() ;
    //Store the pixels as a seperate array. You cannot read / write at the same time asaynchronously
    //this can espeically cause issue with servers or hardware like touchscreens / depth cameras
    map.readToPixels(mapPixels) ; 
}

//Draw minimap GUI
void PixelHitTestHub::drawMap( float scale , float padding )
{
    if ( bDebugDraw == false )
        return ;
    
    float iMapScale = 1.0f / mapScale ; 
    
    ofSetColor ( 255 , 255 , 255 ) ; 
    ofPushMatrix() ; 
        ofTranslate( padding, padding , 0 ) ; 
        ofScale ( iMapScale * scale, iMapScale *scale , iMapScale *scale ) ;
        map.draw( 0 , 0 ) ; 
    ofPopMatrix() ;

    ofFill() ;
    //Draw Last Color Swatches
    
    int w = ofGetWidth() * scale ;
    int h = ofGetHeight() * scale ;

    ofSetHexColor ( 0 ) ;
    ofRect ( padding , padding + h , w , 25 ) ;


    ofFill() ;

    //Map Hex Swatch
    ofSetHexColor ( lastMapHex ) ;
    ofRect ( padding + 1 , padding + h , 25 , 25 ) ;

    //Titles
    ofSetColor ( 175 , 175 , 175 ) ;
    //Convert decimcal to hexidecimal string ? or RGB channels for more clarity ?
    //ofDrawBitmapString( "Map Color: \n " + ofToString( lastMapHex ) , 60 , padding + 10 ) ;
    ofDrawBitmapString( "Map Color " , 60 , padding + h + 15 ) ;

     //Draw hot pink ! Border
    ofSetColor ( 255 , 105 , 180 ) ;
    ofNoFill() ;

    //Box Borders
    ofRect ( padding , padding , w , h )  ;
    ofRect ( padding , padding + h , w , 25 ) ;
    ofRect ( padding , padding + h , 25 , 25 ) ;
    ofRect ( padding + 125 , padding + h , 25 , 25 ) ;

    ofFill() ;
}

void PixelHitTestHub::drawOverlay( )
{
    if ( bDebugOverlay == false )
        return ;
    
    float iMapScale = 1.0f / mapScale ; 
    
    ofSetColor ( 255 , 255 , 255 , 125 ) ; 
    ofPushMatrix() ; 
        ofScale ( iMapScale , iMapScale , iMapScale ) ;
        map.draw( 0 , 0 ) ; 
    ofPopMatrix() ;

}

void PixelHitTestHub::setup ( int w , int h , int _backgroundHex , int _captureIncrement , int _mapSampling )
{
	mapSampling = _mapSampling ; 
	mapScale = 1.0f / (float)mapSampling ; 
	fullDimensions = ofPoint ( w , h ) ; 

    backgroundHex = _backgroundHex ;
    map.allocate( fullDimensions.x/mapSampling , fullDimensions.y/mapSampling , GL_RGB ) ;
    captureIncrement = _captureIncrement ; 
    
    lastMapHex = backgroundHex ;
    bDebugDraw = false ;
    bDebugOverlay = false ;
    availableColor = 0xFFFFFF ;

	maxBounds = ofRectangle ( 0 , 0 , map.getWidth() -1 , map.getHeight() -1 ) ; 
	cout << "Pixel bounds : " << maxBounds.x << "," << maxBounds.y << "," << maxBounds.width << "," << maxBounds.height << endl ; 
}

int PixelHitTestHub::getUniqueHex ( )
{
    availableColor-- ;
    return availableColor ;
}

int PixelHitTestHub::getLastMapHex()
{
    return lastMapHex ;
}

void PixelHitTestHub::addColor ( int _hex ) 
{
    takenColors.push_back ( _hex ) ;
}

int PixelHitTestHub::getColorfulUniqueHex ( )
{
    /* faster, more accurate way but harder to debug 
    addColor( availableColor ) ; 
    availableColor-- ; 
    
    return availableColor ; 
    */
    
    //Feel free to comment this out if needed but it makes the "mapping" more diverse visually and easier to debug
    int randomHex = ofColor( ofRandom ( 255 ) , ofRandom ( 255 ) , ofRandom ( 255 ) ).getHex() ;
    if ( takenColors.size() == 0 ) 
        return randomHex ; 
    for ( int i = 0 ; i < takenColors.size() ; i++ )
    {
        if ( takenColors[i] == randomHex )
        {
            //Color already in use, start over with a new hexValue
            i = 0 ;
            randomHex = ofColor( ofRandom ( 255 ) , ofRandom ( 255 ) , ofRandom ( 255 ) ).getHex() ;
        }
    }
    
    addColor( randomHex ) ; 

    return randomHex ;

}

void PixelHitTestHub::setSampling( int _mapSampling ) 
{
	mapSampling = _mapSampling ; 
	mapScale = 1.0f / (float)mapSampling ; 
    map.allocate( fullDimensions.x/mapSampling , fullDimensions.y/mapSampling , GL_RGB ) ;
	maxBounds = ofRectangle ( 0 , 0 , map.getWidth() -1 , map.getHeight() -1 ) ; 
}

