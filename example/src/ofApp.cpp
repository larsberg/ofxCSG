#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup()
{
//	ofxObjLoader::load("torus1.obj", m0); // box0
//	ofxObjLoader::load("sphere0.obj", m1); // box1
	
	ofxObjLoader::load("coplanar0.obj", m0);
	ofxObjLoader::load("coplanar1.obj", m1);
	
//	ofxObjLoader::load( "box0.obj", m0 );
//	ofxObjLoader::load( "box1.obj", m1 );
	
	
	ofxCSG::meshUnion( m0, m1, m0 );
	
	
	ofxObjLoader::load("plane0.obj", p0);
	ofxObjLoader::load("plane1.obj", p1);
	
	auto polygons0 = ofxCSG::meshToPolygons( p0 );
	auto polygons1 = ofxCSG::meshToPolygons( p1 );
	
	auto p1 = ofxCSG::polygonsToPolylines( polygons0 );
	auto p2 = ofxCSG::polygonsToPolylines( polygons1 );
	
	ofTessellator tess;
	tess.tessellateToPolylines( p1, OF_POLY_WINDING_POSITIVE, p1 );
	
	p1.insert( p1.end(), p2.begin(), p2.end() );
	
	//OF_POLY_WINDING_ABS_GEQ_TWO	==	trim to inner shape
	//OF_POLY_WINDING_ODD			==	cut out inner shape
	//OF_POLY_WINDING_POSITIVE		==	merge outlines
	//OF_POLY_WINDING_NONZERO		==	merge outlines
	//OF_POLY_WINDING_NEGATIVE		==	nothing...
	
	ofPolyWindingMode pMode = OF_POLY_WINDING_POSITIVE;
	
	tess.tessellateToPolylines( p1, pMode, polylines );
	tess.tessellateToMesh( p1, pMode, pMesh );
	
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
	
//	p0.drawWireframe();
//	p1.drawWireframe();
	
	ofSetColor(255, 100);
	pMesh.drawWireframe();
	
	ofSetColor(255, 0, 0);
	for(auto& p: polylines)
	{
		p.draw();
	}
	
	
//	for(auto& p: polygons0)
//	{
//		p.draw();
//	}
//	
//	for(auto& p: polygons1)
//	{
//		p.draw();
//	}
//	
//	ofSetColor(255, 100);
//	ofSetLineWidth( 2 );
//	m0.drawWireframe();
//	m1.drawWireframe();

	ofPopMatrix();
	
	camera.end();
	
	ofPopStyle();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}
