#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup()
{
//	ofSetLogLevel( OF_LOG_VERBOSE );
	
	ofxObjLoader::load( "box0.obj", m0 );
	ofxObjLoader::load( "box1.obj", m1 );
}

//--------------------------------------------------------------
void ofApp::update()
{
	ofMesh mesh0 = m0, mesh1 = m1;
	
	ofMatrix4x4 transform;
	
	transform.translate( sin(ofGetElapsedTimef() ) * 10, 0, 0);
	transform.rotate( ofGetElapsedTimef() * 10, 0, .1, 1);
	
	auto& vertices = mesh1.getVertices();
	for(auto& v: vertices)
	{
		v = v * transform;
	}
	
	ofxCSG::meshUnion( mesh0, mesh1, mesh );

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
	mesh.draw();
	ofSetColor(255);
	mesh.drawWireframe();
	
	ofPopMatrix();
	
	camera.end();
	
	ofPopStyle();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}
