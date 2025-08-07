#pragma once
#include "ofMain.h"
#include "GpuParticleSystem.h"
#include "Attractor.h"
#include "Vec3Magnet.h"

namespace naus3a{
    class ParticleController{
    public:
        ParticleController();
        void setup(int w, int h);
        void update();
        void draw();
        void drawDebug();

        void beginFrame(){fboFrame.begin(); ofClear(0,0,0,0);}
        void endFrame(){fboFrame.end();}

        Attractor attractor;
        Vec3Magnet attractorMagnet;
        ofVec3f gravity;
        float dampening;
        float homing;
    private:
        void initShaders();
        float * makeFrameFloats(int nChans=4);
        void destroyFrameFloats(float * f);
        float * makeStartPosFloats();
        void loadDataFbo(ofFbo & fbo, float * data);

        void onPsUpdate(ofShader & shader);
        void onPsDraw(ofShader & shader);

        ofSpherePrimitive mshBall;
        GpuParticleSystem ps;
        ofFbo fboFrame;
        ofFbo fboStartPos;
        ofVec2f frameSz;
        float rAttractor;
        string pthShaders120;
        string pthShaders330;
        bool bShaders;
    };
}