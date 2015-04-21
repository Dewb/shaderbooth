#include "ofApp.h"

#define CAMERA_X 1280
#define CAMERA_Y 720
#define DPI 300
#define PAPER_X 10.5 * DPI
#define PAPER_Y 8 * DPI
//#define PAPER_X CAMERA_X
//#define PAPER_Y CAMERA_Y

//--------------------------------------------------------------
void ofApp::setup(){

    vector<ofVideoDevice> devices = _grabber.listDevices();

    for(int i = 0; i < devices.size(); i++){
        cout << devices[i].id << ": " << devices[i].deviceName;
        //cout << devices[i].formats[0].width << "x" << devices[i].formats[0].height;
        if( devices[i].bAvailable ){
            cout << endl;
        }else{
            cout << " - unavailable " << endl;
        }
    }

    _grabber.setDeviceID(devices.size() - 1);
    _grabber.setDesiredFrameRate(60);
    _grabber.initGrabber(CAMERA_X, CAMERA_Y);

    _printShader.load("vertex.glsl", "print_frag.glsl", "");
    _previewShader.load("vertex.glsl", "preview_frag.glsl", "");
    _previewUsesPrintShader = false;

    _textureImage.loadImage("texture1.jpg");
    
    _frameBuffer.allocate(PAPER_X, PAPER_Y);
    _pixels.allocate(PAPER_X, PAPER_Y, 3);

    _printer.setPrinterName(_printer.getDefaultPrinterName());

}

//--------------------------------------------------------------
void ofApp::update(){
    _grabber.update();
    _printer.updatePrinterInfo();
}

//--------------------------------------------------------------
void ofApp::draw(){
    if (0) {
        // draw without shader
        float wf = ofGetWidth() / (_grabber.width * 1.0);
        float hf = ofGetHeight() / (_grabber.height * 1.0);
        float factor = max(wf, hf);
        float w = _grabber.width * factor;
        float h = _grabber.height * factor;
        _grabber.draw((ofGetWidth() - w) / 2, (ofGetHeight() - h) / 2, w, h);
    } else {
        renderShader(_previewUsesPrintShader ? &_printShader : &_previewShader, ofGetWidth(), ofGetHeight());
    }
}

void ofApp::renderShader(ofShader* shader, int w, int h) {
    shader->begin();

    shader->setUniform2f("render_size", w, h);

    glActiveTexture(GL_TEXTURE0_ARB);
    _grabber.getTextureReference().bind();
    shader->setUniformTexture("video", _grabber.getTextureReference(), 0);
    shader->setUniform2f("video_size", _grabber.width, _grabber.height);

    glActiveTexture(GL_TEXTURE1_ARB);
    _textureImage.getTextureReference().bind();
    shader->setUniformTexture("texture", _textureImage.getTextureReference(), 1);
    shader->setUniform2f("texture_size", _textureImage.width, _textureImage.height);

    ofRect(0, 0, w, h);

    _grabber.getTextureReference().unbind();
    _textureImage.getTextureReference().unbind();

    shader->end();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if (key == ' ') {
        _frameBuffer.begin();
        renderShader(&_printShader, PAPER_X, PAPER_Y);
        _frameBuffer.end();

        _frameBuffer.readToPixels(_pixels);
        _outputImage.setFromPixels(_pixels);
        _outputImage.saveImage("foo.png");
        _printer.printImage("foo.png");
    } else if (key == 'p') {
        _previewUsesPrintShader = !_previewUsesPrintShader;
    }
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

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
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
