#pragma once
#include "ofMain.h"

class Attractor{
public:
    Attractor(){init();}
    Attractor(ofVec3f _pos, float _r){init();set(_pos, _r);}
    
    void draw(){
        ofPushStyle();
        ofSetColor(ofColor::yellow);
        ofPushMatrix();
        ofTranslate(position);
        ofScale(radius);
        msh.drawWireframe();
        ofPopMatrix();
        ofPopStyle();
    }
    
    void set(ofVec3f _pos, float _r){
        position = _pos;
        setRadius(_r);
    }
    
    void setRadius(float _r){
        radius = _r;
        radiusSq = radius*radius;
    }
    
    float getRadius(){return radius;}
    float getRadiusSquared(){return radiusSq;}
    
    /*void save(ofxXmlSettings & _xml){
        _xml.addTag("attractor");
        _xml.pushTag("attractor");
        _xml.setValue("radius", radius);
        _xml.setValue("force", force);
        _xml.popTag();
    }
    
    void load(ofxXmlSettings & _xml){
        if(_xml.tagExists("attractor")){
            _xml.pushTag("attractor");
            radius = _xml.getValue("radius", radius);
            force = _xml.getValue("force", force);
            _xml.popTag();
        }else{
            ofLogError("Attractor::load","no attractor tag");
        }
    }*/
    
    ofVec3f position;
    float force;
protected:
    void init(){msh.set(1,5);force=500;}
    
    ofSpherePrimitive msh;
    float radius;
    float radiusSq;
};