#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){

	ofSetVerticalSync(true);
	
	// this uses depth information for occlusion
	// rather than always drawing things on top of each other
	glEnable(GL_DEPTH_TEST);
	
	// this sets the camera's distance from the object
	cam.setDistance(500);
	ofBackground(0);

	// open an outgoing connection to HOST:PORT
	sender.setup(HOST, PORT);
    //ofBackgroundHex(0xff72a1);
    ofSetFrameRate(30);
    ofSetWindowPosition(500, 500);    
    // this is our buffer to stroe the text data
    ofBuffer buffer = ofBufferFromFile("/Users/ari/Projects/artistic/Import3dfromVRML/data/engefalos00.txt");
    
    if(buffer.size()) { 
		
        // we now keep grabbing the next line
        // until we reach the end of the file
        while(buffer.isLastLine() == false) {
            
            // move on to the next line
            string line = buffer.getNextLine();
			
            
            // copy the line to draw later
            // make sure its not a empty line
            if(line.empty() == false) {
                seussLines.push_back(line);
            }
            findPoint(line);
            // print out the line
            //cout << line << endl;
            
        }
        
    }	
}

ofVec3f testApp::findPoint(string str) {
	ofVec3f tempVec;
	string tempString;
	int tempPosition = str.find(" ");
	tempString = str.substr(0,str.size());
	tempVec.x = ofToFloat(tempString.substr(0,tempPosition));	
	tempPosition = tempString.find(" ");
	tempVec.y = ofToFloat(tempString.substr(tempPosition+1,tempPosition));
	tempString = tempString.substr(tempPosition+1,tempString.size());
	tempPosition = tempString.find(" ");
	tempVec.z = ofToFloat(tempString.substr(tempPosition+1,tempPosition));
	tempString = tempString.substr(tempPosition+1,tempString.size());
	//tempString = tempString.substr(tempPosition+1,tempString.size());
	cout << tempVec << endl;
	points[counter] = tempVec;
	counter++;
	
	
	
	
	return tempVec;
}

//--------------------------------------------------------------
void testApp::update(){

}

//--------------------------------------------------------------
void testApp::draw(){
	cam.begin();
	//ofSetPolyMode(OF_POLY_WINDING_NONZERO);
	//ofBeginShape();

	for (int i =0; i < 30000; i++){
		ofCircle(300*points[i].x, 300*points[i].y, 300*points[i].z, 2);
		//ofVertex(300*points[i].x, 300*points[i].y, 300*points[i].z);
	}
	//ofEndShape();
	cam.end();
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
	if(key == 'a' || key == 'A'){
		ofxOscMessage m;
		m.setAddress("/test");
		m.addIntArg(1);
		m.addFloatArg(3.5f);
		m.addStringArg("hello");
		m.addFloatArg(ofGetElapsedTimef());
		sender.sendMessage(m);
	}
	switch(key) {
		case 'M':
		case 'm':
			if(cam.getMouseInputEnabled()) cam.disableMouseInput();
			else cam.enableMouseInput();
			break;
			
		case 'F':
		case 'f':
			ofToggleFullscreen();
			break;
	}
	
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y){
	ofxOscMessage m;
	m.setAddress("/mouse/position");
	m.addIntArg(x);
	m.addIntArg(y);
	sender.sendMessage(m);
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
	ofxOscMessage m;
	m.setAddress("/mouse/button");
	m.addStringArg("down");
	sender.sendMessage(m);
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){
	ofxOscMessage m;
	m.setAddress("/mouse/button");
	m.addStringArg("up");
	sender.sendMessage(m);

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){

}

