/************************************************************
************************************************************/
#include "ofApp.h"


/************************************************************
************************************************************/

/******************************
******************************/
ofApp::ofApp(int _id)
: Cam_id(_id)
, State(STATE_STOP)
, k_PLAY(false)
, k_STOP(false)
, VideoCam(NULL)
{
}

/******************************
******************************/
ofApp::~ofApp()
{
	if(VideoCam) delete VideoCam;
}

/******************************
******************************/
void ofApp::exit()
{
	printMessage("Good-bye");
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
	font.loadFont("FTY DELIRIUM NCV.ttf", 150);
	
	/********************
	********************/
	VideoCam = new ofVideoGrabber;
	
	ofSetLogLevel(OF_LOG_VERBOSE);
    VideoCam->setVerbose(true);
	
	VideoCam->listDevices();// 上 2行がないと、表示されない.
	
	if(Cam_id == -1){
		std::exit(1);
	}

	/********************
	********************/
	fbo_TextureSyphonServer.setName("LiveVideoServer");
	
	fbo.allocate(VIDEO_WIDTH, VIDEO_HEIGHT);
	
	/********************
	********************/
	Osc_VJ.setup("127.0.0.1", 12355, 12356);
}

//--------------------------------------------------------------
void ofApp::update(){
	/********************
	********************/
	if( (State == STATE_STOP) && (k_PLAY) ){
		k_PLAY = false;
		Process_STOP_to_PLAY();
		
		
	}else if( (State == STATE_PLAY) && (k_STOP) ){
		k_STOP = false;
		Process_PLAY_to_STOP();
		
	}
	
	/********************
	********************/
	while(Osc_VJ.OscReceive.hasWaitingMessages()){
		ofxOscMessage m_receive;
		Osc_VJ.OscReceive.getNextMessage(&m_receive);
		
		if(m_receive.getAddress() == "/Quit"){
			std::exit(1);
		}
	}
	
	/********************
	********************/
	if(State == STATE_PLAY){
		VideoCam->update();
	
		if(VideoCam->isFrameNew()){
			// nothing.
		}
	}
}

/******************************
******************************/
void ofApp::Process_STOP_to_PLAY()
{
	/********************
	********************/
	State = STATE_PLAY;
	
	/********************
	********************/
	VideoCam = new ofVideoGrabber;
	
	ofSetLogLevel(OF_LOG_VERBOSE);
    VideoCam->setVerbose(true);
	
	VideoCam->setDeviceID(Cam_id);
	VideoCam->initGrabber(VIDEO_WIDTH, VIDEO_HEIGHT);

	/********************
	********************/
	printMessage("PLAY start");
}

/******************************
******************************/
void ofApp::Process_PLAY_to_STOP()
{
	State = STATE_STOP;
	
	delete VideoCam;
	VideoCam = NULL;
	
	printMessage("Stop");
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
	if(State == STATE_PLAY){
		ofBackground(0);
		ofSetColor(255, 255, 255, 255);
		
		VideoCam->draw(0, 0, VIDEO_WIDTH, VIDEO_WIDTH);
		
	}else{
		ofBackground(0);
		
		char DispMessage[BUF_SIZE];
		sprintf(DispMessage, "STOP:LiveVideo");
		
		float offset_x = font.stringWidth(DispMessage) / 2;
		font.drawString(DispMessage, VIDEO_WIDTH/2 - offset_x, VIDEO_HEIGHT/2);
	}
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
	switch(key){
		case 'p':
			if(State == STATE_STOP) k_PLAY = true;
			break;
			
		case 's':
			if(State == STATE_PLAY) k_STOP= true;
			break;
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
