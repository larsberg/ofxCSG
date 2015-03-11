#pragma once

#include "ofMain.h"

#include "ofxObjLoader.h"

#include "ofxCSG.h"

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
	
	ofEasyCam camera;
	
	ofMesh m0, m1, mesh, otherMesh;
	
	
	ofxCSG::Polygon cp0, cp1;
	vector<ofVec3f> intersectionPoints;
};
