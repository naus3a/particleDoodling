#pragma once
#include "ofMain.h"

class FloatRange{
public:
    FloatRange(){}
    FloatRange(float _min, float _max){set(_min, _max);}
    void set(float _min, float _max){min=_min;max=_max;}
    
    void adjustRange(float _newRange){
        float _ctr = getCenter();
        float hR = _newRange/2;
        set(_ctr-hR, _ctr+hR);
    }
    
    void adjustCenter(float _newCtr){
        float hR = getRange()/2;
        set(_newCtr-hR, _newCtr+hR);
    }
    
    float getRange(){return (max-min);}
    float getPctInRange(float _pct){return ofMap(_pct,0,1,min,max);}
    float getCenter(){return (min+(getRange()/2));}
    
    float min;
    float max;
};