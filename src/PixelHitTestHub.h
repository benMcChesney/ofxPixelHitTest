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
    
 //       Need to implement
        //void removeItemByHex( int hex ) { } 
    
        void setup( int w , int h , int _backgroundHex ) ;
        int backgroundHex ;

        //Custom FBO Drawing
        void beginFbo ( ) ;
        void endFbo ( ) ;

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

        //Fixes rare read/write issue
        ofFbo readMap ;             //used for hit Tracking, written into after draw is complete
        ofFbo writeMap ;                //render buffer FBO
    private:
};

#endif // PIXELHITTESTHUB_H
