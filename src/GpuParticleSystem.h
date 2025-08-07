#pragma once
#include "ofMain.h"

namespace naus3a{
    class GpuParticleSystem{
        public:
        static const string UNIFORM_PREFIX;
        static const string UPDATE_SHADER_NAME;
        static const string DRAW_SHADER_NAME;
        static const unsigned FLOATS_PER_TEXEL;

        enum DataTextureIndex{
            POSITION,
            VELOCITY
        };

        GpuParticleSystem();
        void init(unsigned w, unsigned h, ofPrimitiveMode primitive=OF_PRIMITIVE_POINTS, unsigned numDataTextures = 2);
        void loadShaders(const string& updateShaderName, const string& drawShadername);
        void loadDataTexture(unsigned idx, float* data, unsigned x=0, unsigned y=0, unsigned w=0, unsigned h=0);
        void zeroDataTexture(unsigned idx, unsigned x=0, unsigned y=0, unsigned w=0, unsigned h=0);

        void update();
        void draw();

        ofFbo::Settings getFboSettings(unsigned numColBuffers) const;

        ofEvent<ofShader> updateEvent;
        ofEvent<ofShader> drawEvent;

        private:
        void setUniforms(ofShader& shader);

        ofFbo fbos[2];
        map<string, ofTexture> readOnlyDatatTextures;
        ofVboMesh mesh;
        ofVboMesh quadMesh;
        ofShader updateShader;
        ofShader drawShader;
        unsigned currentReadFbo;
        unsigned textureLocation;
        unsigned width;
        unsigned height;
        unsigned numFloats;
    };
}