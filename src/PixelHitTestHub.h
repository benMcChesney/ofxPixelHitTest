#ifndef PIXELHITTESTHUB_H
#define PIXELHITTESTHUB_H

#include "ofMain.h"
#include "CorePixelHitTest.h"

class PixelHitTestHub
{
    public:
        PixelHitTestHub( );
        virtual ~PixelHitTestHub();

        vector<CorePixelHitTest*> items ;

        CorePixelHitTest * getItemAt ( ofVec2f input ) ;
        CorePixelHitTest * getItemByMapColor ( int hexColor ) ;

        int getHexAt( ofVec2f input ) ;

        void addItem ( CorePixelHitTest * c ) ;
    
        //Need to Implement
        //void removeItemByHex( int hex ) { } 
    
        void setup( int w , int h , int _backgroundHex , int _captureIncrement = 6 ) ;
        int backgroundHex ;

        //Custom FBO Drawing
        bool beginFbo ( ) ;
        void endFbo ( ) ;
    
        //Simplified FBO Drawing
        void drawItemsIntoFBO ( ) ;

        //Draw debug
        void drawMap( float scale = .25f , float padding = 15.0f ) ;

        //returns a unique color for hitTest id
        int getUniqueHex ( ) ;

        //More human friendly colors
        int getColorfulUniqueHex ( ) ;

        int getLastMapHex()
        {
            return lastMapHex ;
        }

        //Drawing every frame is intensive so we skip frames because it shouldn't effect the interactions too much and makes it much faster
        int captureIncrement ; 
    
        //draw 'minimap'
        bool debugDraw ;
    
        //Store the contents of each map into this object
        ofPixels mapPixels ; 

    protected:
        int availableColor ;        //Starts at 0xFFFFFF and increment down...
        int lastMapHex ;            //for gui
        ofFbo map ;                 //Save screen into this FBO then use it's pixels for hitTest operations
    
    private:
};

#endif // PIXELHITTESTHUB_H
