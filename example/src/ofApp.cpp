#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup()
{
//	ofxObjLoader::load("sphere0.obj", m0); // box0
//	ofxObjLoader::load("torus1.obj", m1); // box1
	
	ofxObjLoader::load("coplanar1.obj", m0);
	ofxObjLoader::load("coplanar0.obj", m1);
	
//	ofxObjLoader::load( "box0.obj", m0 );
//	ofxObjLoader::load( "box1.obj", m1 );
	
	// get our polygons
	polygons0 = ofxCSG::meshToPolygons( m0 );
	polygons1 = ofxCSG::meshToPolygons( m1 );
	
	
	
	//split
	for(auto& p0: polygons0)
	{
		for(auto& p1: polygons1)
		{
			p0.split( p1 );
		}
	}
	
	//classy the triangles
	ofVec3f rayDir(0,1,0);
	for(auto& p0: polygons0)
	{
		p0.classify( polygons1 );
	}
	
	//set the meshes from our poygons
	ofxCSG::setMeshFromPolygons( m0, polygons0 );
	ofxCSG::setMeshFromPolygons( m1, polygons1 );
}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw()
{
	ofPushStyle();
	ofEnableDepthTest();
	
	camera.begin();
	
	ofPushMatrix();
	ofScale(10, 10, 10);
	
	for(auto& p: polygons0)
	{
		p.draw();
	}
	
	ofSetColor(255, 100);
	ofSetLineWidth( 2 );
	m0.drawWireframe();
	m1.drawWireframe();

	ofPopMatrix();
	
	camera.end();
	
	ofPopStyle();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}
