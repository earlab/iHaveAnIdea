#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
	ofSetBackgroundAuto(false);
	part2 = false;
	rateSendNode = 20;
	counter = 0;
	part1 = false;
	part3 = false;	
	attractToCenterBool = false;
	//ofHideCursor();
	ofEnableAlphaBlending(); 
	CGDisplayHideCursor(kCGDirectMainDisplay); 
	//glutSetCursor(
	//glutSetCursor(GLUT_CURSOR_FULL_CROSSHAIR);
	// open an outgoing connection to HOST:PORT
	sender.setup(HOST, PORT);
    ofBackgroundHex(0x000000);
    ofSetFrameRate(30);
    ofSetWindowPosition(500, 500);
	cam.setDistance(1000);
	cam.setFarClip(10000);
	center = ofVec3f(0,0,0);
    // this is our buffer to stroe the text data
    ofBuffer buffer = ofBufferFromFile("/Users/ari/Projects/artistic/Import3dfromVRML/data/engefalos00.txt");
	//ofBuffer buffer = ofBufferFromFile("engefalos00.txt");
    if(buffer.size()) { 
		
        // we now keep grabbing the next line
        // until we reach the end of the file
        while(buffer.isLastLine() == false) {
            
            // move on to the next line
            string line = buffer.getNextLine();

			// extract the vector and push it on the back of the list
			ofVec3f tmpLine = findPoint(line);
            points.push_back(tmpLine);
			speeds.push_back(ofVec3f(ofRandomf()*2,ofRandomf()*2,ofRandomf()*2));
			colors.push_back(ofFloatColor(1,1,1));
			accelerations.push_back(ofVec3f(0,0,0));
			initialPoints.push_back(tmpLine);
//			initialPointsPos[counter] = tmpLine;
//			counter++;
        }
    }

	
}

ofVec3f testApp::findPoint(string str) {
	ofVec3f tempVec;
	string tempString;
	int tempPosition = str.find(" ");
	tempString = str.substr(0,str.size());
	tempVec.x = ofToFloat(tempString.substr(0,tempPosition))*300;	
	tempPosition = tempString.find(" ");
	tempVec.y = ofToFloat(tempString.substr(tempPosition+1,tempPosition))*300;
	tempString = tempString.substr(tempPosition+1,tempString.size());
	tempPosition = tempString.find(" ");
	tempVec.z = ofToFloat(tempString.substr(tempPosition+1,tempPosition))*300;
	tempString = tempString.substr(tempPosition+1,tempString.size());
	//tempString = tempString.substr(tempPosition+1,tempString.size());
	//cout << tempVec << endl;
	initialPointsPos[counter] = tempVec;
	counter++;

	return tempVec;
	glPointSize(15);
}

//--------------------------------------------------------------
void testApp::update(){
	if (ofGetFrameNum()%rateSendNode == true) {
		randomNode = int(ofRandom(0, 40000));
		ofxOscMessage m;
		m.setAddress("node");
		m.addFloatArg(initialPointsPos[randomNode].x);		
		m.addFloatArg(initialPointsPos[randomNode].y);		
		m.addFloatArg(initialPointsPos[randomNode].z);
		m.addFloatArg(initialPointsPos[randomNode].distance( before ));
		if (attractToCenterBool == true) {
			m.addIntArg(10);
		}	else {
			m.addIntArg(1000);
		}
		sender.sendMessage(m);
		
	}

	
	if (attractToCenterBool == true) {
		attractToCenter();
	} else {
		pushThemAway();
		pullToCenter();
		applyPerlin();		
	}
	
	ofxOscMessage m;
	m.setAddress("camPos");
	m.addIntArg(ofGetMouseX());
	m.addIntArg(ofGetMouseY());
	m.addIntArg(cam.getDistance());	
	sender.sendMessage(m);
}

//--------------------------------------------------------------
void testApp::draw(){
	cam.begin();

	
	if (part1) {
		vbo.setColorData(&colors[0],colors.size(),GL_DYNAMIC_DRAW);
		vbo.setVertexData(&points[0], points.size(), GL_DYNAMIC_DRAW);
		vbo.draw(GL_POINTS, 0, (int)points.size());		
	}
	if (part2) {
		ofSetColor(255,255,0,250);
		ofFill();
		ofCircle(initialPointsPos[randomNode].x, initialPointsPos[randomNode].y, initialPointsPos[randomNode].z, 5);
		
		ofLine(before.x, before.y, before.z, initialPointsPos[randomNode].x, initialPointsPos[randomNode].y, initialPointsPos[randomNode].z);
		before = initialPointsPos[randomNode];
		//cout << initialPointsPos[1] << endl;
	}
	
	cam.end();
	if (part3) {
		ofSetColor(0,0,0,30);
	}	else {
		ofSetColor(0,0,0,0);
	}
	ofFill();
	ofRect(-50, -50, ofGetWidth() + 100, ofGetHeight() + 100);
	
}
void testApp::pullToCenter() {
	for (int i = 0 ; i < points.size(); i++){
		ofVec3f dirToCenter	= points[i] - center;
		float distToCenter	= dirToCenter.length();
		float maxDistance	= 800.0f;
	
		if( distToCenter > maxDistance ){
			dirToCenter.normalize();
			float pullStrength = 0.0001f;
			speeds[i] -= dirToCenter * ( ( distToCenter - maxDistance ) * pullStrength );
		}
	}
}
void testApp::attractToCenter() {
	for (int i = 0 ; i < points.size(); i++){
		ofVec3f dirToCenter	= points[i] - initialPoints[i];
		float distToCenter	= dirToCenter.length();
		float maxDistance	= 0.0f;
		dirToCenter.normalize();
		float pullStrength = 0.001f;
		speeds[i] -= dirToCenter * ( ( distToCenter - maxDistance ) * pullStrength );
		points[i] += speeds[i];
		colors[i] = ofFloatColor(1-speeds[i].x,1-speeds[i].y ,1-speeds[i].z + 0.2);
		accelerations[i] = 0;		
		speeds[i] = 0.99*speeds[i];
	}
}
void testApp::pushThemAway() {
	for (int i = 0 ; i < points.size(); i++){
		speeds[i] += accelerations[i];
		points[i] += speeds[i];
		colors[i] = ofFloatColor(speeds[i].x,speeds[i].y,speeds[i].z);
		accelerations[i] = 0;
	}		
}

void testApp::applyPerlin() {
	for (int i = 0 ; i < points.size(); i++)
	{
		float nX = points[i].x * 0.005f;
		float nY = points[i].y * 0.005f;
		float nZ = points[i].z * 0.005f;
		float nW = ofGetFrameNum() * 0.0025f;
		float noise = ofNoise( nX, nY, nZ );
		float angle = noise * 15.0f;
		ofVec3f noiseVector( cos( angle ), sin( angle ), cos(angle) );
		speeds[i] += noiseVector  * 0.01f;
	}
}
void testApp::mouseMoved(int x, int y){
}
void testApp::mouseDragged(int x, int y, int button){}
void testApp::mousePressed(int x, int y, int button){
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){
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

void testApp::keyPressed(int key){
	/*
	 ofxOscMessage m;
	 m.setAddress("/test");
	 m.addIntArg(1);
	 m.addFloatArg(3.5f);
	 m.addStringArg("hello");
	 m.addFloatArg(ofGetElapsedTimef());
	 sender.sendMessage(m);
	 */
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
		case 'i':
			for (int i = 0 ; i < points.size(); i++){
				points[i] = initialPoints[i];
			}
			break;
		case 'g':
			attractToCenterBool = !attractToCenterBool;
			break;			
		case '1':
			part1 = !part1;
			break;			
		case '2':
			part2 = !part2;
			break;			
		case '3':
			part3 = !part3;
			break;			
			
		case '+':
			rateSendNode = rateSendNode + 5;
			cout << rateSendNode << endl;
			break;			

		case '-':
			if (rateSendNode> 6) 	{	
				rateSendNode = rateSendNode - 5;
				cout << rateSendNode << endl;			
			}

			break;			
			
	}
	
}
void testApp::keyReleased(int key){
	
}
