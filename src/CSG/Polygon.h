//
//  Polygon.h
//
//  Created by lars berg on 3/3/15.
//

#pragma once

#include <Triangle.h>
#include <LineSegment.h>

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
		
		void addTriangle( Vertex a, Vertex b, Vertex c );
		
		ofVec3f getNormal();
		
		float getW();
		
		void clear();
		
		void flip();
		
		vector<ofPolyline> toPolylines();
		
		//campare two RampItems using their u values
		static bool compareOfVec3f ( ofVec3f v0, ofVec3f v1);
		
		vector<ofVec3f> getVertices();
		
//		void split( LineSegment segment );
		
		void split( Triangle& t );
		
		void split( Polygon& p );
		
		void split( ofVec3f t0, ofVec3f t1, ofVec3f t2 );
		
		bool intersectRay( ofVec3f rayOrigin, ofVec3f rayDir, float epsilon, ofVec3f* intersection = NULL );
		
		void classify( vector<Polygon> polygons );
		
		void draw(bool useNormalForColor = true);
		
		vector<Triangle> triangles;
		
		//for coplanar splitting
		vector<ofPolyline> polylines;
		vector<LineSegment> lineSegments;
	};
}