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
	
	ofMesh m0, m1;
	
	vector<ofxCSG::Polygon> polygons0, polygons1;
	
	ofMesh p0, p1;
	
	vector<ofPolyline> polylines;
	ofMesh pMesh;
};
