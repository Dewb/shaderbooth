#pragma once

#include "ofMain.h"
#include "ofxAutoReloadedShader.h"
#include "ofxCUPS.h"
#include "ofxEdsdk.h"

typedef enum {
    Webcam = 0,
    DSLR = 1
} LivePreviewSource;

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);

    void renderShader(ofShader* shader, ofTexture& cameraTexture, int width, int height);
    void processPhoto(ofTexture& cameraTexture);
    
    ofVideoGrabber _grabber;
    ofxEdsdk::Camera _canonCamera;

    ofxAutoReloadedShader _previewShader;
    ofxAutoReloadedShader _printShader;
    bool _previewUsesPrintShader;
    ofImage _textureImage;

    ofPixels _pixels;
    ofFbo _frameBuffer;

    ofImage _outputImage;
    ofxCUPS _printer;
    ofTrueTypeFont _messageFont;

    LivePreviewSource previewSource;
};
