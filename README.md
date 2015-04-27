shaderbooth
===========

Slapdash brain for photobooths with shader effect processing

Shaders can be live-edited while booth is running. Currently set up to mimic a photocopier.

Requires at least one attached camera. Supports Quicktime-compatible webcams, and Canon DSLRs that speak EDSDK. 

Three possible scenarios:
- Live preview and photos with DSLR
- Live preview and photos with webcam
- Live preview with webcam, photos taken with DSLR (change LivePreviewSource to Webcam in setup().)

Requirements:

- Mac OS X only (tested on 10.6 & 10.10)
- [openFrameworks 0.8.4](http://openframeworks.cc/download/)
- [ofxAutoReloadedShader](https://github.com/andreasmuller/ofxAutoReloadedShader)
- [ofxEDSDK](https://github.com/kylemcdonald/ofxEdsdk) (with "isConnected()" method added)
- [Gutenprint](http://gimp-print.sourceforge.net/)

