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
    
        void setup( int w , int h , int _backgroundHex , int _captureIncrement = 6 , int _mapSampling = 2 ) ;
        int backgroundHex ;

        //Custom FBO Drawing
        bool beginFbo ( bool flipY = false ) ;
        void endFbo ( ) ;
    
        //Simplified FBO Drawing
        void drawItemsIntoFBO (  ) ;

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

		void addColor ( int _hex ) 
		{
			takenColors.push_back ( _hex ) ;
		}

		ofPoint fullDimensions ; 

        //Drawing every frame is intensive so we skip frames because it shouldn't effect the interactions too much and makes it much faster
        int captureIncrement ; 
    
        //draw 'minimap'
        bool debugDraw ;
    
        //Store the contents of each map into this object
        ofPixels mapPixels ; 
		bool cameraRendered ;	//if using ofCamera it will flip the Y
		 ofFbo map ;                 //Save screen into this FBO then use it's pixels for hitTest operations

		 bool bUseCamera ; 

		 //To increase performance we sample the screen and do not use the full size
		 void setSampling( int sampleRate ) ; 

		 //"Cache" the max indicies to prevent crashing
		 ofRectangle maxBounds ; 
    protected:

        int availableColor ;        //Starts at 0xFFFFFF and increment down...
        int lastMapHex ;            //for gui
		vector<int> takenColors  ; 
		float mapScale ; 
		int mapSampling ; 
       
    
    private:
};

#endif // PIXELHITTESTHUB_H
