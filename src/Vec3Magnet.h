#pragma once
#include "FloatMagnet.h"
//#include "ofxXmlSettings.h"

class Vec3Magnet{
public:
    Vec3Magnet(){}
    Vec3Magnet(FloatMagnet _x, FloatMagnet _y, FloatMagnet _z){x=_x;y=_y;z=_z;}
    
    void update(){
        x.update();
        y.update();
        z.update();
    }
    
    /*void save(ofxXmlSettings & _xml){
        _xml.addTag("magnet");
        _xml.pushTag("magnet");
        _xml.setValue("x",x.strength);
        _xml.setValue("y",y.strength);
        _xml.setValue("z",z.strength);
        _xml.popTag();
    }*/
    
    FloatMagnet x;
    FloatMagnet y;
    FloatMagnet z;
};