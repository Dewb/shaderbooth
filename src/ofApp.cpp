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

    previewSource = DSLR;

    _canonCamera.setup();
    if (!_canonCamera.isConnected()) {
        previewSource = Webcam;
    }

    if (previewSource == Webcam) {
        _grabber.setDeviceID(devices.size() - 1);
        _grabber.setDesiredFrameRate(60);
        _grabber.initGrabber(CAMERA_X, CAMERA_Y);
    }
    
    _printShader.load("vertex.glsl", "print_frag.glsl", "");
    _previewShader.load("vertex.glsl", "preview_frag.glsl", "");
    _previewUsesPrintShader = false;

    _textureImage.loadImage("texture1.jpg");
    
    _frameBuffer.allocate(PAPER_X, PAPER_Y);
    _pixels.allocate(PAPER_X, PAPER_Y, 3);

    _printer.setPrinterName(_printer.getDefaultPrinterName());

    _messageFont.loadFont("Digital dream Fat Skew Narrow.ttf", 150);
    ofSetFrameRate(15);

}

//--------------------------------------------------------------
void ofApp::update(){
    if (previewSource == Webcam) {
        _grabber.update();
    }

    _printer.updatePrinterInfo();

    if (_canonCamera.isConnected()) {
        _canonCamera.update();

        if(_canonCamera.isPhotoNew()) {
            processPhoto(_canonCamera.getPhotoTexture());
        }
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofTexture* pPreviewTexture;
    if (previewSource == Webcam) {
        pPreviewTexture = &_grabber.getTextureReference();
    } else if (previewSource == DSLR) {
        pPreviewTexture = &_canonCamera.getLiveTexture();
    }

    if (pPreviewTexture) {
        renderShader(_previewUsesPrintShader ? &_printShader : &_previewShader,
                     *pPreviewTexture,
                     ofGetWidth(),
                     ofGetHeight());
    }

    string message = "";
    int state = _printer.getPrinterState();
    if (state == 4) {
        message = "COPYING\nJOB\nTRAY 1";
    } else if (state == 5) {
        message = "ERROR\nERROR\nERROR";
    }

    string status = _printer.getPrinterInfo();
    if (status == "media-empty-warning") {
        message = "PC\nLOAD\nLETTER";
    }

    if (message != "") {
        ofSetColor(ofColor::black);
        _messageFont.drawString(message, 60, 210);
        ofSetColor(ofColor::white);
        _messageFont.drawString(message, 66, 216);
    }
}

void ofApp::renderShader(ofShader* shader, ofTexture& cameraTexture, int w, int h) {
    shader->begin();

    shader->setUniform2f("render_size", w, h);

    glActiveTexture(GL_TEXTURE0_ARB);
    cameraTexture.bind();
    shader->setUniformTexture("video", cameraTexture, 0);
    shader->setUniform2f("video_size", cameraTexture.getWidth(), cameraTexture.getHeight());

    glActiveTexture(GL_TEXTURE1_ARB);
    _textureImage.getTextureReference().bind();
    shader->setUniformTexture("texture", _textureImage.getTextureReference(), 1);
    shader->setUniform2f("texture_size", _textureImage.width, _textureImage.height);

    ofRect(0, 0, w, h);

    cameraTexture.unbind();
    _textureImage.getTextureReference().unbind();

    shader->end();
}

void ofApp::processPhoto(ofTexture& cameraTexture) {
    _frameBuffer.begin();
    renderShader(&_printShader, cameraTexture, PAPER_X, PAPER_Y);
    _frameBuffer.end();

    _frameBuffer.readToPixels(_pixels);
    _outputImage.setFromPixels(_pixels);
    _outputImage.saveImage("output.png");
    _printer.printImage("output.png");
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if (key == ' ') {
        if (_canonCamera.isConnected()) {
            _canonCamera.takePhoto();
        } else {
            processPhoto(_grabber.getTextureReference());
        }
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
