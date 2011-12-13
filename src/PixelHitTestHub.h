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
        void removeItemByHex( int hex ) { } 
    
        ofImage inputMap ; 
    
        void setup( int w , int h , int _backgroundHex ) ;
        int backgroundHex ;

        //Custom FBO Drawing
        void beginFbo ( ) ;
        void endFbo ( ) ;
    
        void drawBegin(); 
        void drawEnd() ; 
    
        //Simplified FBO Drawing
        void drawItemsIntoFBO ( ) ;

        //Draw debug
        void drawMap( float scale = .25f ) ;

        //returns a unique color for hitTest id
        int getUniqueHex ( ) ;

        //More human friendly colors
        int getColorfulUniqueHex ( ) ;

        int getLastMapHex()
        {
            return lastMapHex ;
        }

        //draw 'minimap'
        bool debugDraw ;

    protected:
        int availableColor ;        //Starts at 0xFFFFFF and increment down...
        int lastMapHex ;            //for gui
        ofFbo map ;                 //Save screen into this FBO then use it's pixels for hitTest operations
    private:
};

#endif // PIXELHITTESTHUB_H
