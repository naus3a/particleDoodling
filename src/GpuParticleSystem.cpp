#include "GpuParticleSystem.h"

namespace naus3a{
    const string GpuParticleSystem::UNIFORM_PREFIX = "particles";
    const string GpuParticleSystem::UPDATE_SHADER_NAME = "update";
    const string GpuParticleSystem::DRAW_SHADER_NAME = "draw";
    const unsigned GpuParticleSystem::FLOATS_PER_TEXEL = 4;

    GpuParticleSystem::GpuParticleSystem(){
        currentReadFbo = 0;
        textureLocation = 0;
        width = 0;
        height = 0;
    }

    void GpuParticleSystem::init(unsigned w, unsigned h, ofPrimitiveMode primitive, unsigned numDataTextures){
        width = w;
        height = h;
        numFloats = width*height*FLOATS_PER_TEXEL;

        ofFbo::Settings s = getFboSettings(numDataTextures);
        for(unsigned i=0;i<2;i++){
            fbos[i].allocate(s);
        }

        mesh.clear();
        for(int y=0;y<height;++y){
            for(int x=0;x<width;++x){
                mesh.addVertex(ofVec3f(200.f * x / (float)width - 100.f, 200.f * y / (float)height - 100.f, -500.f));
                mesh.addTexCoord(ofVec2f(x,y));
            }
        }
        mesh.setMode(primitive);

        quadMesh.addVertex(ofVec3f(-1.f, -1.f, 0.f));
        quadMesh.addVertex(ofVec3f(1.f, -1.f, 0.f));
        quadMesh.addVertex(ofVec3f(1.f, 1.f, 0.f));
        quadMesh.addVertex(ofVec3f(-1.f, 1.f, 0.f));
        
        quadMesh.addTexCoord(ofVec2f(0.f, 0.f));
        quadMesh.addTexCoord(ofVec2f(width, 0.f));
        quadMesh.addTexCoord(ofVec2f(width, height));
        quadMesh.addTexCoord(ofVec2f(0.f, height));
        
        quadMesh.addIndex(0);
        quadMesh.addIndex(1);
        quadMesh.addIndex(2);
        quadMesh.addIndex(0);
        quadMesh.addIndex(2);
        quadMesh.addIndex(3);

        quadMesh.setMode(OF_PRIMITIVE_TRIANGLES);
    }

    void GpuParticleSystem::loadShaders(const string& updateShaderName, const string& drawShaderName){
        updateShader.load(updateShaderName);
        drawShader.load(drawShaderName);
    }

    void GpuParticleSystem::loadDataTexture(unsigned idx, float* data, unsigned x, unsigned y, unsigned w, unsigned h){
        if(idx<fbos[currentReadFbo].getNumTextures()){
            if(!w)w=width;
            if(!h)h=height;
            fbos[currentReadFbo].getTexture(idx).bind();
            glTexSubImage2D(GL_TEXTURE_RECTANGLE_ARB, 0, x,y, w,h, GL_RGBA, GL_FLOAT, data);
            fbos[currentReadFbo].getTexture(idx).unbind();
        }else{
            ofLogError()<<"Trying to load data into non-existent buffer";
        }
    }

    void GpuParticleSystem::zeroDataTexture(unsigned idx, unsigned x, unsigned y, unsigned w, unsigned h){
        if(!w)w=width;
        if(!h)h=height;
        unsigned numFloats = w*h*FLOATS_PER_TEXEL;
        float* zeros = new float[numFloats];
        memset(zeros,0,sizeof(float)*numFloats);
        loadDataTexture(idx, zeros,x,y,w,h);
        delete[] zeros;
    }

    void GpuParticleSystem::update(){
        int idx = 1-currentReadFbo;
        fbos[idx].begin(OF_FBOMODE_NODEFAULTS);
        ofPushStyle();
        glViewport(0,0,width,height);
        ofDisableBlendMode();
        ofSetColor(255,255,255);
        fbos[idx].activateAllDrawBuffers();

        updateShader.begin();
        ofNotifyEvent(updateEvent, updateShader, this);
        setUniforms(updateShader);
        quadMesh.draw();
        updateShader.end();
        
        ofPopStyle();
        fbos[idx].end();

        currentReadFbo = 1-currentReadFbo;
    }

    void GpuParticleSystem::draw(){
        drawShader.begin();
        ofNotifyEvent(drawEvent, drawShader, this);
        setUniforms(drawShader);
        mesh.draw();
        drawShader.end();
    }

    void GpuParticleSystem::setUniforms(ofShader& shader){
        unsigned textureLocationOffset = 0;
        for(unsigned i=0;i<fbos[currentReadFbo].getNumTextures();++i){
            string uName = UNIFORM_PREFIX+ofToString(i);
            shader.setUniformTexture(uName, fbos[currentReadFbo].getTexture(i), textureLocationOffset+textureLocation);
            textureLocationOffset++;
        }
        for(auto& r : readOnlyDatatTextures){
            shader.setUniformTexture(r.first, r.second, textureLocationOffset+textureLocation);
            textureLocationOffset++;
        }
    }

    ofFbo::Settings GpuParticleSystem::getFboSettings(unsigned numColBuffers) const{
        if(!width || !height){
            ofLogFatalError()<<"GpuParticleSystem::getFboSettings() called before init";
            return ofFbo::Settings();
        }
        ofFbo::Settings s;
        s.internalformat = GL_RGBA32F_ARB;
        s.textureTarget = GL_TEXTURE_RECTANGLE_ARB;
        s.minFilter = GL_NEAREST;
        s.maxFilter = GL_NEAREST;
        s.wrapModeHorizontal = GL_CLAMP;
        s.wrapModeVertical = GL_CLAMP;
        s.width = width;
        s.height = height;
        s.numColorbuffers = numColBuffers;
        return s;
    }
}