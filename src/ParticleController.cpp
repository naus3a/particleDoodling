#include "ParticleController.h"

namespace naus3a{
    ParticleController::ParticleController(){
        bShaders = false;
        attractor.set(ofVec3f(0,0,200), 200);
        gravity.set(0,-0.5,0);
        dampening = 0.995;
        homing = 10;
        pthShaders120 = ofFilePath::join("shaders", "glsl120");
        pthShaders330 = ofFilePath::join("shaders", "glsl330");
        mshBall.set(50, 5);
        
        attractorMagnet.x.setAttracted(&attractor.position.x);
        attractorMagnet.y.setAttracted(&attractor.position.y);
        attractorMagnet.z.setAttracted(&attractor.position.z);
        
        initShaders();
    }

    void ParticleController::initShaders(){
        if(bShaders)return;
        if(ofIsGLProgrammableRenderer()){
            cout<<"Particles: using programmable renderer"<<endl;
            ps.loadShaders(ofFilePath::join(pthShaders330, "update"), ofFilePath::join(pthShaders330, "draw"));
        }else{
            cout<<"Particles: using old school non programmable renderer"<<endl;
            ps.loadShaders(ofFilePath::join(pthShaders120, "update"), ofFilePath::join(pthShaders120, "draw"));
            //TODO
        }
        bShaders = true;
    }

    float * ParticleController::makeFrameFloats(int nChans){
        int n = frameSz.x*frameSz.y*nChans;
        float * f = new float[n];
        return f;
    }

    void ParticleController::destroyFrameFloats(float * f){
        if(f==NULL)return;
        delete [] f;
    }

    float * ParticleController::makeStartPosFloats(){
        float startX = -frameSz.x/2;
        float startY = -frameSz.y/2;
        float * _f = makeFrameFloats();
        for(int y=0;y<frameSz.y;y++){
            for(int x=0;x<frameSz.x;x++){
                unsigned _idx = y*frameSz.x+x;
                _f[_idx*4] = startX+x; //x
                _f[_idx*4+1] = startY+y; //y
                _f[_idx*4+2] = 0; //z
                _f[_idx*4+3] = 0; //unused
            }
        }
        return _f;
    }

    void ParticleController::loadDataFbo(ofFbo & fbo, float * data){
        fbo.getTexture().bind();
        glTexSubImage2D(GL_TEXTURE_RECTANGLE_ARB, 0, 0, 0, fbo.getWidth(), fbo.getHeight(), GL_RGBA, GL_FLOAT, data);
        fbo.getTexture().unbind();
    }

    void ParticleController::setup(int w, int h){
        frameSz.set(w,h);
        ps.init(frameSz.x, frameSz.y);
        initShaders();

        fboFrame.allocate(frameSz.x, frameSz.y, GL_RGBA);
        beginFrame();
        endFrame();

        float * startPos = makeStartPosFloats();
        ps.loadDataTexture(GpuParticleSystem::POSITION, startPos);
        fboStartPos.allocate(ps.getFboSettings(2));
        loadDataFbo(fboStartPos, startPos);
        destroyFrameFloats(startPos);

        ps.zeroDataTexture(GpuParticleSystem::VELOCITY);
        ofAddListener(ps.updateEvent, this, &ParticleController::onPsUpdate);
        ofAddListener(ps.drawEvent, this, &ParticleController::onPsDraw);
    }

    void ParticleController::onPsUpdate(ofShader & shader){
        shader.setUniformTexture("texStartPos", fboStartPos.getTexture(), 2);
        shader.setUniform3fv("gravity", gravity.getPtr());
        shader.setUniform1f("attFrc", attractor.force);
        shader.setUniform1f("dampening", dampening);
        shader.setUniform1f("homing", homing);

        shader.setUniform3fv("attractor", attractor.position.getPtr());
        shader.setUniform1f("elapsed", ofGetLastFrameTime());
        shader.setUniform1f("radiusSquared", attractor.getRadiusSquared());
    }

    void ParticleController::onPsDraw(ofShader & shader){
        shader.setUniformTexture("texFrame", fboFrame.getTexture(), 2);
    }

    void ParticleController::update(){
        attractorMagnet.update();
        ps.update();
    }

    void ParticleController::draw(){
        ps.draw();
    }

    void ParticleController::drawDebug(){
        attractor.draw();

        ofPushStyle();
        ofSetColor(ofColor::red);
        ofPushMatrix();
        ofTranslate(attractorMagnet.x.value,attractorMagnet.y.value,attractorMagnet.z.value);
        mshBall.drawWireframe();
        ofPopMatrix();
        ofSetColor(ofColor::white);
        ofDrawBitmapString("dampening: "+ofToString(dampening)+"\nhoming: "+ofToString(homing)+"\nradius: "+ofToString(attractor.getRadius()), 20,20);
        ofPopStyle();
    }
}