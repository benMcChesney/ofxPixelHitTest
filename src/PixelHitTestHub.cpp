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
    int index = (input.x + input.y * map.getWidth()) * 3; 
    
    //cout << "@" << input.x<< ","<<input.y << "| r : " << r << " g: " << g << " b: " << b << endl ;  
    lastMapHex = ofColor( mapPixels[index] , mapPixels[index + 1] , mapPixels[index + 2] ).getHex() ; 
    return lastMapHex ; 
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

void PixelHitTestHub::beginFbo ( )
{
    map.begin() ;
    //Clear background
    ofSetHexColor ( backgroundHex ) ;
    ofRect ( 0 , 0 , ofGetWidth() , ofGetHeight() ) ;

}

void PixelHitTestHub::endFbo ( )
{
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
void PixelHitTestHub::drawMap( float scale )
{
    if ( debugDraw == false )
        return ;
    
    //ofDisableAlphaBlending () ;

    //How far to draw from the edge
    int padding  = 20 ;
    if ( scale == 1.0f ) 
        padding = 0.0f ; 

    //ofSetColor ( 255 , 255 , 255 ) ;
    ofPushMatrix() ;
        if ( scale != 1.0 ) 
            ofTranslate ( padding , padding , 0 ) ;
        ofScale ( scale , scale , scale ) ;
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

void PixelHitTestHub::setup ( int w , int h , int _backgroundHex )
{
    backgroundHex = _backgroundHex ;
    map.allocate( w , h , GL_RGB ) ;
   // mapPixels = new unsigned char[w * h * 3] ; 
    
    
    lastMapHex = backgroundHex ;
    debugDraw = false ;
    availableColor = 0xFFFFFF ;
}

int PixelHitTestHub::getUniqueHex ( )
{
    availableColor-- ;
    return availableColor ;
}

int PixelHitTestHub::getColorfulUniqueHex ( )
{
  //  return getUniqueHex() ;
    
    //Feel free to comment this out if needed but it makes the "mapping" more diverse visually
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

