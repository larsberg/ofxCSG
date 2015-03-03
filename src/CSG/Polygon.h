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
		Polygon()
		{}
		~Polygon()
		{}
		
		
		void addTriangle( ofVec3f a, ofVec3f b, ofVec3f c )
		{
			triangles.push_back( Triangle( a, b, c ) );
		}
		
		void addTriangle( Triangle t )
		{
			triangles.push_back( t );
		}
		
		void set( ofVec3f a, ofVec3f b, ofVec3f c )
		{
			clear();
			triangles.push_back( Triangle( a, b, c ) );
		}
		
		void addTriangle( shared_ptr<Vertex> a, shared_ptr<Vertex> b, shared_ptr<Vertex> c )
		{
			triangles.push_back( Triangle( a, b, c ) );
		}
		
		void clear()
		{
			triangles.clear();
			vertices.clear();
		}
		
		void flip()
		{
			for(auto& t: triangles)	t.flip();
		}
		
		void insertPoint( shared_ptr<Vertex> p )
		{
			vector<Triangle> tri;
			for(auto& t: triangles)
			{
				auto subd = t.subdivide( p );
				tri.insert( tri.end(), subd.begin(), subd.end() );
			}
			
			triangles = tri;
		}
		
		void insertPoint( ofVec3f p )
		{
			auto v = shared_ptr<Vertex>( new Vertex( p ) );
			return insertPoint( v );
		}
		
		vector<LineSegment> intersection(Triangle& tri)
		{
			vector<LineSegment> segments;
			LineSegment segment;
			for(auto& t: triangles)
			{
				if(t.getIntersection( tri, &segment))
				{
					segments.push_back( segment );
				}
			}
			
			return segments;
		}
		
		void split( Triangle& t )
		{
			vector<Triangle> splitTriangles;
			for(auto& tri: triangles)
			{
				auto subd = tri.split( t );
				splitTriangles.insert( splitTriangles.end(), subd.begin(), subd.end() );
			}
			
			triangles = splitTriangles;
		}
		
		void draw()
		{
			for(auto& t: triangles)
			{
				ofSetColor( ofFloatColor( t.normal[0], t.normal[1], t.normal[2] ) );
				t.draw();
			}
		}
		
		vector<Triangle> triangles;
		vector< shared_ptr<Vertex> > vertices;
	};

}