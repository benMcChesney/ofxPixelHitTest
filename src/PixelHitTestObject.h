#ifndef PIXELHITTESTOBJECT_H
#define PIXELHITTESTOBJECT_H


class PixelHitTestObject
{
    public:
        PixelHitTestObject() {}
        virtual ~PixelHitTestObject() {}


        //Override these in the inherited classes to deal with input / rendering the map out how you see fit !
        virtual void renderMap ( ) { }
        virtual void inputFromHex( int hex ) { }

        virtual int getPixelHex( )
        {
            return pixelHex ;
        }

        virtual void setPixelHex( int _pixelHex )
        {
            pixelHex = _pixelHex ;
        }

        virtual bool isHex( int inputHex ) { return ( pixelHex == inputHex ) ; }
        protected:
    
        int pixelHex ;


    private:
};

#endif // PIXELHITTESTOBJECT_H
