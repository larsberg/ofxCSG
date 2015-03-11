//
//  Triangle.h
//
//  Created by lars berg on 3/3/15.
//

#pragma once

#include <LineSegment.h>

namespace ofxCSG
{	
	class Triangle
	{
	public:
		Triangle(ofVec3f a, ofVec3f b, ofVec3f c);
		
		Triangle();
		~Triangle();
		
		ofVec3f* getPtr();
		
		ofVec3f& operator[]( int n );
		
		void set( ofVec3f _a, ofVec3f _b, ofVec3f _c );
		
		void flip();
		
		void calcNormal();
		
		float getArea();
		
		float getAreaSquared();
		
		bool rayIntersect( ofVec3f rayOrigin, ofVec3f rayDir );
		
		bool intersectRay( ofVec3f rayOrigin, ofVec3f rayDir, ofVec3f* intersection = NULL );

		bool intersectRay( ofVec3f rayOrigin, ofVec3f rayDir, float epsilon, ofVec3f* intersection = NULL );
		
		ofVec3f getCenter();
		
		void draw(bool useNormalForColor = true);
		
		ofPolyline toPolyline();
		
		Classification getClassification( ofVec3f planeNormal, float planeW );
		
		void classifyWithPlane( ofVec3f planeNormal, float planeW );
		
		vector<ofVec3f> intersectWithPlane( ofVec3f planeNormal, float planeW );
		
		bool getIntersection( Triangle t, LineSegment* overlap );
		
		vector<Triangle> insert( ofVec3f v );

		vector<Triangle> splitWithCoplanarSegment(ofVec3f a, ofVec3f b);
		
		vector<Triangle> splitWithCoplanarSegment(LineSegment segment);
		
		vector<Triangle> split( Triangle t );
		
		vector<Triangle> meshToTriangles(ofMesh& m);
		
		ofVec3f a, b, c, centroid;
		ofVec3f normal;
		float w;
		Classification classification;
	};
	
	
	//STATIC METHODS
	static vector<Triangle> meshToTriangles(ofMesh& m)
	{
		vector<Triangle> triangles;
		
		auto indices = m.getIndices();
		auto v = m.getVertices();
		
		if(indices.size())
		{
			for(int i=0; i<indices.size(); i+=3)
			{
				triangles.push_back( Triangle( v[ indices[i] ], v[ indices[i+1] ], v[ indices[i+2] ] ) );
			}
		}
		else
		{
			for(int i=0; i<v.size(); i+=3)
			{
				triangles.push_back( Triangle( v[i], v[i+1], v[i+2] ) );
			}
		}
		
		return triangles;
	}
}