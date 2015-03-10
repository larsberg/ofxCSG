#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup()
{
	ofSetLogLevel( OF_LOG_VERBOSE );
	
	ofxObjLoader::load("torus1.obj", m0); // box0
	ofxObjLoader::load("sphere0.obj", m1); // box1
	
//	ofxObjLoader::load("coplanar1.obj", m0 );
//	ofxObjLoader::load("coplanar0.obj", m1 );
	
//	ofxObjLoader::load( "box0.obj", m0 );
//	ofxObjLoader::load( "box1.obj", m1 );
	
	ofxCSG::meshUnion( m0, m1, m0 );
}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw()
{
	ofBackground(0,0,0);
	ofPushStyle();
	ofDisableDepthTest();
	
	ofSetLineWidth( 1 );
	
	camera.begin();
	
	ofPushMatrix();
	ofScale(10, 10, 10);
	
	ofSetColor(255, 45);
	m0.draw();
	ofSetColor(255);
	m0.drawWireframe();
	
	ofSetColor(255, 0, 0);
	for(auto& p: polylines)
	{
		p.draw();
	}
	
	ofPopMatrix();
	
	camera.end();
	
	ofPopStyle();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}
