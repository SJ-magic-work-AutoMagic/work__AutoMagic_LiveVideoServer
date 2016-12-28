/************************************************************
************************************************************/
#include "ofApp.h"


/************************************************************
************************************************************/

/******************************
******************************/
ofApp::ofApp(int _id)
: Cam_id(_id)
{
}

/******************************
******************************/
ofApp::~ofApp()
{
}

/******************************
******************************/
void ofApp::exit()
{
	printMessage("Good-bye");
	std::exit(1);
}

//--------------------------------------------------------------
void ofApp::setup(){

	/********************
	********************/
	ofSetWindowTitle( "Live Video Server" );
	
	ofSetWindowShape( MONITOR_WIDTH, MONITOR_HEIGHT );
	ofSetVerticalSync(true);
	ofSetFrameRate(30);
	ofSetEscapeQuitsApp(false);
	
	/********************
	********************/
	ofSetLogLevel(OF_LOG_VERBOSE);
    VideoCam.setVerbose(true);
	
	VideoCam.listDevices();// 上 2行がないと、表示されない.
	
	if(Cam_id == -1){
		ofExit();
	}else{
		VideoCam.setDeviceID(Cam_id);
	}

	/********************
	********************/
	fbo_TextureSyphonServer.setName("LiveVideoServer");
	
	fbo.allocate(VIDEO_WIDTH, VIDEO_HEIGHT);
	
	VideoCam.initGrabber(VIDEO_WIDTH, VIDEO_HEIGHT);
	
	/********************
	********************/
	Osc_VJ.setup("127.0.0.1", 12355, 12356);
}

//--------------------------------------------------------------
void ofApp::update(){

	/********************
	********************/
	while(Osc_VJ.OscReceive.hasWaitingMessages()){
		ofxOscMessage m_receive;
		Osc_VJ.OscReceive.getNextMessage(&m_receive);
		
		if(m_receive.getAddress() == "/Quit"){
			ofExit(1);
		}
	}
	
	/********************
	********************/
	VideoCam.update();
	
	if(VideoCam.isFrameNew()){
		// nothing.
	}
}

//--------------------------------------------------------------
void ofApp::draw(){

	/********************
	********************/
	// ofBackground(0);
	
	/********************
	********************/
	// Clear with alpha, so we can capture via syphon and composite elsewhere should we want.
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	/********************
	********************/
	fbo.begin();
	ofBackground(0);
	ofSetColor(255, 255, 255, 255);
	
	VideoCam.draw(0, 0, VIDEO_WIDTH, VIDEO_WIDTH);
	
	fbo.end();
	
	ofTexture tex = fbo.getTextureReference();
	fbo_TextureSyphonServer.publishTexture(&tex);
	
	/********************
	monitor
	********************/
	fbo.draw(0, 0, ofGetWidth(), ofGetHeight());
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

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
