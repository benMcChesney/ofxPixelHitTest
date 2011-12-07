#ifndef COREPIXELCOLLIDER_H
#define COREPIXELCOLLIDER_H


#include "ofMain.h"

class CorePixelHitTest
{
    public:
        CorePixelHitTest() { }
        virtual ~CorePixelHitTest() {}

        virtual void renderMap( )
        {
            ofSetHexColor ( mapHexColor ) ;
            ofRect ( hitRect ) ;
        }

        virtual void setupMapping( ofRectangle _hitRect , int _hexColor )
        {
            mapHexColor = _hexColor ;
            hitRect = _hitRect ;
        }

        virtual ofRectangle getHitRect ( ) { return hitRect ; }
        virtual int getMapHexColor ( ) { return mapHexColor ; }
    
    protected:
        int mapHexColor ;
        ofRectangle hitRect ;

    private:
};

#endif // COREPIXELCOLLIDER_H
