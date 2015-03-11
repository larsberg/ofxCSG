#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup()
{
//	ofSetLogLevel( OF_LOG_VERBOSE );
	
//	ofxObjLoader::load("torus1.obj", m0); // box0
//	ofxObjLoader::load("sphere0.obj", m1); // box1
	
//	ofxObjLoader::load("coplanar1.obj", m0 );
//	ofxObjLoader::load("coplanar0.obj", m1 );
	
	ofxObjLoader::load( "box0.obj", m0 );
	ofxObjLoader::load( "box1.obj", m1 );
	
	paused = false;
}

//--------------------------------------------------------------
void ofApp::update()
{
	ofMesh mesh0 = m0, mesh1 = m1;
	
	ofMatrix4x4 transform;
	
	if(!paused )
	{
		t += .01666;
	}
	
	transform.rotate( t * 10, 0, .1, 1);
	transform.translate( sin( t * .2 ) * 10, 0, 0);
	
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
	ofScale( 20, 20, 20 );
	
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

	if(key == 'p')
	{
		paused = !paused;
	}
	else if( key == 'a')
	{
		t += .01666;
	}
	else if( key == 'A')
	{
		t -= .01666;
	}
}
