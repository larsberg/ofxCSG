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
		
		Polygon( ofVec3f a, ofVec3f b, ofVec3f c )
		{
			addTriangle(a, b, c);
		}
		
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
		
		void addTriangle( Vertex a, Vertex b, Vertex c )
		{
			triangles.push_back( Triangle( a, b, c ) );
		}
		
		void clear()
		{
			triangles.clear();
		}
		
		void flip()
		{
			for(auto& t: triangles)	t.flip();
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
		
		void split( Polygon& p )
		{
			for( auto& t: p.triangles )
			{
				split( t );
			}
		}
		
		void draw()
		{
			for(auto& t: triangles)
			{
				t.draw();
			}
		}
		
		vector<Triangle> triangles;
	};
}