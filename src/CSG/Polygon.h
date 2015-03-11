//
//  Polygon.h
//
//  Created by lars berg on 3/3/15.
//

#pragma once

#include <Triangle.h>
#include <BoundBox.h>

namespace ofxCSG
{
	class Polygon
	{
	public:
		Polygon();
		
		Polygon( ofVec3f a, ofVec3f b, ofVec3f c );
		
		~Polygon();
		
		void addTriangle( ofVec3f a, ofVec3f b, ofVec3f c );
		
		void addTriangle( Triangle t );
		
		void set( ofVec3f a, ofVec3f b, ofVec3f c );
		
		void setClassification( Classification classification );
		
		ofVec3f getNormal();
		
		float getW();
		
		void clear();
		
		void flip();
		
		vector<ofPolyline> toPolylines();
		
		void split( Triangle& t );
		
		void split( Polygon& p );
		
		void split( ofVec3f t0, ofVec3f t1, ofVec3f t2 );
		
		bool intersectRay( ofVec3f rayOrigin, ofVec3f rayDir, float epsilon, ofVec3f* intersection = NULL );
		
		void classifyRay( Polygon& p, int& intersectionCount );
		
		void classify( vector<Polygon>& polygons );
		
		void draw(bool useNormalForColor = true);
		
		
		//
		vector<Triangle> triangles;
		BoundBox bb;
		
		vector< Polygon* > splitters;
		
		bool wasSplit;
		
		int rayIntersectionCount;
		
		
		//for coplanar splitting
//		vector<ofPolyline> polylines;
//		vector<LineSegment> lineSegments;
	};
}