#include "ofApp.h"

FloatRange * fr = new FloatRange(-360,360);

//--------------------------------------------------------------
void ofApp::setup(){
    ofBackground(0);
    ofSetFrameRate(60);
    ofSetVerticalSync(true);

    bGui = false;

    noiseY.range = fr;

    float w = 1280;
    float h = 720;
    ofImage img("img/orchids.jpg");
    pc.setup(w, h);
    pc.beginFrame();
    img.draw(0, h, w, -h);
    pc.endFrame();
    
    pc.attractor.setRadius(275);
    pc.homing = 172;
    pc.dampening = 0.97;
}

//--------------------------------------------------------------
void ofApp::update(){
    
    pc.attractorMagnet.x.value = ofMap(ofGetMouseX(), 0, ofGetWidth(), -640,640);//ofGetMouseX();
    pc.attractorMagnet.y.value = ofMap(ofGetMouseY(), 0, ofGetHeight(), 320,-320);//noiseY.update();
    pc.attractorMagnet.z.value = ofMap(ofGetMouseY(), 0, ofGetHeight(), -400,400);

    pc.update();
}

//--------------------------------------------------------------
void ofApp::draw(){
    cam.begin();
    ofEnableBlendMode(OF_BLENDMODE_ADD);
    pc.draw();
    if(bGui)pc.drawDebug();
    ofDisableBlendMode();
    cam.end();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    switch (key)
    {
    case 'g':
        toggleGui();
        break;
    case 'r':
        pc.attractor.setRadius(pc.attractor.getRadius()+1);
        break;
    case 'R':
        pc.attractor.setRadius(pc.attractor.getRadius()-1);
        break;
    case 'h':
        pc.homing = pc.homing+1;
        break;
    case 'H':
        pc.homing = pc.homing-1;
        break;
    case 'd':
        pc.dampening = pc.dampening+0.001;
        break;
    case 'D':
        pc.dampening = pc.dampening-0.001;
        break;
    case '1':
        pc.dampening = 0.2;
        break;
    case '2':
        pc.dampening = 0.9;
        break;
    default:
        break;
    }
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}

void ofApp::toggleGui(){
    bGui = !bGui;
}