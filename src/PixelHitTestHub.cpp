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

	if ( bUseCamera == true ) 
	{
		int curY = adjustedInput.y ; 
		adjustedInput.y = ( map.getHeight() -1 ) - curY ; 
	}

	//cout << "original input : " << input.x << "," << input.y << endl ; 
	//cout << "adjusted input : " << adjustedInput.x << " , " << adjustedInput.y << endl ;

    if ( adjustedInput.x > 0 && adjustedInput.x < maxBounds.width && adjustedInput.y > 0 && adjustedInput.y < maxBounds.height ) 
	{
		int index = (adjustedInput.x + adjustedInput.y * map.getWidth()) * 3; 
		//cout << "@" << input.x<< ","<<input.y << "| r : " << r << " g: " << g << " b: " << b << endl ;  
		lastMapHex = ofColor( mapPixels[index] , mapPixels[index + 1] , mapPixels[index + 2] ).getHex() ; 
		return lastMapHex ; 
	}
	else
	{
		return backgroundHex ; 
	}
 }

CorePixelHitTest * PixelHitTestHub::getItemAt ( ofVec2f input )
{
    int hex = getHexAt( input ) ;

    //If it's the background color it's not over content
    if ( hex == backgroundHex )
        return NULL ;

    //Otherwise try and get the color
    return getItemByMapColor( hex ) ;
}

CorePixelHitTest * PixelHitTestHub::getItemByMapColor ( int hexColor )
{
    //iterate through
    for ( int i = 0 ; i < items.size() ; i++ )
    {
        //Compare mapHexColor to isolated hexColor
        if ( items[i]->getMapHexColor() == hexColor )
        {
            //cout << "match found for : " << hexColor << endl ;
            return items[i] ;
        }
    }

    return NULL ;
}

bool PixelHitTestHub::beginFbo ( bool flipY )
{
    //TODO: faster way than mod ?
    //If it matches up to frameIncrement
    if ( ofGetFrameNum() % captureIncrement == 0 ) 
    {
        map.begin( flipY ) ;
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
    map.readToPixels(mapPixels) ; 
}


void PixelHitTestHub::drawItemsIntoFBO ( )
{
    map.begin() ;
        //Clear background
        ofSetHexColor ( backgroundHex ) ;
        ofRect ( 0 , 0 , ofGetWidth() , ofGetHeight() ) ;

        ofSetColor ( 255 , 255 , 255 ) ;
        for ( int i = 0 ; i < items.size() ; i++ )
        {
            items[i]->renderMap( ) ;
        }

    map.end() ;
}

//Draw minimap GUI
void PixelHitTestHub::drawMap( float scale , float padding )
{
    if ( debugDraw == false )
        return ;
    
    if ( padding != 0.0f ) 
        ofDisableAlphaBlending () ;
    else
    {
        ofEnableAlphaBlending() ; 
        ofSetColor ( 255 , 255 , 255, 127 ) ; 
    }

    //How far to draw from the edge
    if ( scale == 1.0f ) 
        padding = 0.0f ; 

    //ofSetColor ( 255 , 255 , 255 ) ;
    ofPushMatrix() ;
        if ( scale != 1.0 ) 
            ofTranslate ( padding , padding , 0 ) ;
        ofScale ( scale , scale , scale ) ;

		if ( bUseCamera == true ) 
		{

		glPushMatrix();  
		glScalef(1, -1, 1);  
		map.draw( 0, -map.getHeight() );  
        //map.draw( 0 , 0 ) ;
		glPopMatrix() ; 
		}
		else
		{
			map.draw( 0 , 0 ) ; 
		}
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
    //Convert decimcal to hexidecimal string ? or RGN channels for more clarity ?
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


void PixelHitTestHub::addItem ( CorePixelHitTest * c )
{
    items.push_back( c ) ;
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
    debugDraw = false ;
    availableColor = 0xFFFFFF ;
	bUseCamera = false ;

	maxBounds = ofRectangle ( 0 , 0 , map.getWidth() -1 , map.getHeight() -1 ) ; 
	cout << "maxBounds : " << maxBounds.x << "," << maxBounds.y << "," << maxBounds.width << "," << maxBounds.height << endl ; 
}

int PixelHitTestHub::getUniqueHex ( )
{
    availableColor-- ;
    return availableColor ;
}

int PixelHitTestHub::getColorfulUniqueHex ( )
{
    //In production this is faster but makes debugger harder
  //  return getUniqueHex() ;
    
    //Feel free to comment this out if needed but it makes the "mapping" more diverse visually and easier to debug
    int randomHex = ofColor( ofRandom ( 255 ) , ofRandom ( 255 ) , ofRandom ( 255 ) ).getHex() ;
    if ( items.size() < 1 ) 
        return randomHex ; 
    for ( int i = 0 ; i < items.size() ; i++ )
    {
        if ( items[i]->getMapHexColor() == randomHex )
        {
            //Color already in use, start over with a new hexValue
            i = 0 ;
            randomHex = ofColor( ofRandom ( 255 ) , ofRandom ( 255 ) , ofRandom ( 255 ) ).getHex() ;
        }
    }

    return randomHex ;
}

void PixelHitTestHub::setSampling( int _mapSampling ) 
{
	mapSampling = _mapSampling ; 
	mapScale = 1.0f / (float)mapSampling ; 
    map.allocate( fullDimensions.x/mapSampling , fullDimensions.y/mapSampling , GL_RGB ) ;
	maxBounds = ofRectangle ( 0 , 0 , map.getWidth() -1 , map.getHeight() -1 ) ; 
}

