//
//  PixelHitTestEvent.h
//  emptyExample
//
//  Created by Ben McChesney on 2/26/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#ifndef emptyExample_PixelHitTestEvent_h
#define emptyExample_PixelHitTestEvent_h


class PixelHitTestEventArgs
{
    public:
        PixelHitTestEventArgs( ofVec2f _input , int _inputHex )
        {
            input = _input ;
            inputHex = _inputHex ;
        }
        
        ofVec2f input ;
        int inputHex ;
};

class PixelHitTestEvent
{
    public :
        PixelHitTestEvent ( ) { } ; 
    
    
        static PixelHitTestEvent* Instance()
        {
            static PixelHitTestEvent inst ; 
            return &inst ; 
        }
    
        ofEvent<int>    PIXEL_OVER ;   
        ofEvent<int>    PIXEL_OUT ;
        ofEvent<int>    PIXEL_CLICK ;
    
        ofEvent<PixelHitTestEventArgs>    PIXEL_DOWN ;
        ofEvent<PixelHitTestEventArgs>    PIXEL_DRAG ;
        ofEvent<PixelHitTestEventArgs>    PIXEL_UP ;
    
        ofEvent<int>    PIXEL_CLICK_AFTER ;
};


#endif
