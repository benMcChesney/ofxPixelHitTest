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
   // ofPixels pixels;
    //map.readToPixels(pixels);
    (inputMap.getColor( input.x , input.y )).getHex() ; 
   // cout << "getting hex !" << endl; 
   // unsigned char rgb[3] ;
   // glReadPixels(input.x, input.y, 1 , 1 , GL_RGB, GL_UNSIGNED_BYTE, rgb ) ; 
    
   // cout << "r : " << rgb[0] << endl ; 
   // cout << "g : " << rgb[1] << endl ; 
   // cout << "b : " << rgb[2] << endl ;  
        
   // return backgroundHex ; 
    //int _hex = (pixels.getColor( input.x , input.y )).getHex() ;
    int _hex = (inputMap.getColor( input.x , input.y )).getHex() ;
    lastMapHex = _hex ;

    return _hex ;
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
}

void PixelHitTestHub::drawBegin()
{
    ofSetHexColor ( backgroundHex ) ;
    ofRect ( 0 , 0 , ofGetWidth() , ofGetHeight() ) ;
}

void PixelHitTestHub::drawEnd()
{
    inputMap.grabScreen(0, 0, ofGetWidth(), ofGetHeight() ) ; 
//    imageMap.loadScreenData() ; 
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
        //map.draw( 0 , 0 ) ;
        inputMap.draw( 0 , 0 ) ; 
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
    map.allocate( w , h , GL_RGBA ) ;

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

