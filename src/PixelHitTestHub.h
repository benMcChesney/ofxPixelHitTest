#ifndef PIXELHITTESTHUB_H
#define PIXELHITTESTHUB_H

#include "ofMain.h"
#include "PixelHitTestEvent.h"

class PixelHitTestHub
{
    public:
        PixelHitTestHub( );
        virtual ~PixelHitTestHub();
        int getHexAt( ofVec2f input ) ;
        void setup( int w , int h , int _backgroundHex , int _captureIncrement = 6 , int _mapSampling = 2 ) ;
    
        int backgroundHex ; //color of the background

        //Custom FBO Drawing
        bool beginFbo ( ) ;
        void endFbo ( ) ;

        //Draw debug
        void drawMap( float scale = .25f , float padding = 10.0f ) ;
        void drawOverlay ( ) ; 
    
        //returns a unique color for hitTest id
        int getUniqueHex ( ) ;

        //More human friendly colors
        int getColorfulUniqueHex ( ) ;

        int getLastMapHex() ;
        void addColor ( int _hex ) ;

        //Drawing every frame is intensive so we skip frames because it shouldn't effect the interactions too much and makes it much faster
        int captureIncrement ; 
    
        //draw 'minimap'
        bool bDebugDraw ;
        bool bDebugOverlay ; 
    
        ofPoint fullDimensions ;
    
        //Store the contents of each map into this object
        ofPixels mapPixels ; 
        ofFbo map ;                 //Save screen into this FBO then use it's pixels for hitTest operations

		 //To increase performance we sample the screen and do not use the full size
		 void setSampling( int sampleRate ) ; 

		 //"Cache" the max indicies to prevent crashing for nonexisting pixels
		 ofRectangle maxBounds ; 
    
    protected:

        int availableColor ;        //Starts at 0xFFFFFF and increment down...
        int lastMapHex ;            //for gui
		vector<int> takenColors  ; 
		float mapScale ;            //amount the screen is scaled
        float iMapScale ;           //returns fbo to screen size
		int mapSampling ; 
    
};

#endif // PIXELHITTESTHUB_H
