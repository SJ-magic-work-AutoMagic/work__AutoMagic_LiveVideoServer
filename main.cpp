#include "ofMain.h"
#include "ofApp.h"

//========================================================================
int main( int argc, char** argv ){
	ofSetupOpenGL(1024,768,OF_WINDOW);			// <-------- setup the GL context

	// this kicks off the running of my app
	// can be OF_WINDOW or OF_FULLSCREEN
	// pass in width and height too:
	
	int cam_id = -1;
	
	if(argc < 2){
		printf("*.exe [cam id]\n");
	}else{
		cam_id = atoi(argv[1]);
	}
	
	ofRunApp(new ofApp(cam_id));
}
