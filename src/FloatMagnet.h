#pragma once
#include "ofMain.h"

class FloatMagnet{
public:
    FloatMagnet(){init();}
    FloatMagnet(float * _ptr){
        init();
        setAttracted(_ptr);
    }
    
    void setAttracted(float * _ptr){
        if(_ptr!=NULL){
            ptr=_ptr;
            bReady = true;
        }else{
            bReady = false;
        }
    }
    
    void update(){
        if(!bReady)return;
        *ptr = (strength*value)+((1.0-strength)*(*ptr));
    }
    
    float value;
    float strength;
protected:
    void init(){
        strength = 0.1;
        ptr = NULL;
        bReady = false;
    }
    float * ptr;
    bool bReady;
};