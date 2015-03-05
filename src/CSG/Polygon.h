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
		
		
		vector<ofPolyline> toPolyline()
		{
			vector<ofPolyline> polylines;
			ofTessellator tess;
			
			for(auto& t: triangles)
			{
				ofPolyline p;
				p.addVertex( t.a );
				p.addVertex( t.b );
				p.addVertex( t.c );
				
				p.setClosed(true);
				
				polylines.push_back( p );
			}
			
			vector<ofPolyline> dstpoly = polylines;
//			tess.tessellateToPolylines( polylines, OF_POLY_WINDING_POSITIVE, dstpoly );
			
			return dstpoly;
		}
		
		void coplanarSplit( Polygon& p )
		{
			return triangles;
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
			//TODO: the coplanar splitting is slow! find someone who's smarter and ask them how to do this
			//
			//if they're coplanar split them differnetly
			float nDot = triangles[0].normal.dot( p.triangles[0].normal );
			if( abs( nDot ) >= 1 || abs( triangles[0].normal.x ) >= 1 )
			{
				cout << "nDot: " << nDot << endl;
//				if(abs( distanceToPlaneSigned(p.triangles[0].a, triangles[0].a, triangles[0].normal ) ) <= EPSILON)
				if( isPointOnPlane( p.triangles[0].a, triangles[0].normal, triangles[0].w) )
				{
//					coplanarSplit( p );
//					vector<Triangle> splitTriangles;
//					for(auto& t: triangles)
//					{
//						for(auto& pt: p.triangles)
//						{
//							auto subd = t.coplanarSplit( pt );
//							splitTriangles.insert( splitTriangles.end(), subd.begin(), subd.end() );
//						}
//					}
//					
//					triangles = splitTriangles;
				}
			}
			else
			{
				//otherwise split the triangles individually
				for( auto& t: p.triangles )
				{
					split( t );
				}
			}
		}
		
		void split( ofVec3f t0, ofVec3f t1, ofVec3f t2 )
		{
			Polygon p( t0, t1, t2 );
			
			split( p );
		}
		
		bool intersectRay( ofVec3f rayOrigin, ofVec3f rayDir, float epsilon, ofVec3f* intersection = NULL )
		{
			for(auto& t: triangles)
			{
				if( t.intersectRay( rayOrigin, rayDir, epsilon, intersection ) )
				{
					return true;
				}
			}
			
			return false;
		}
		
		void classify( vector<Polygon> polygons )
		{
			for(auto& t: triangles)
			{
				if(t.classification == BACK || t.classification == FRONT)	continue;
				
				ofVec3f rayOrigin = t.getCenter();
				ofVec3f rayDir = ofVec3f(ofRandomf(), ofRandomf(), ofRandomf()).normalize(); // ofVec3f(0,1,0);
				
				int intersectionCount = 0;
				
				for( auto& p: polygons )
				{
					if( p.intersectRay( rayOrigin, rayDir, EPSILON, NULL ) )
					{
						intersectionCount++;
					}
				}
				
				t.classification = intersectionCount % 2 ? ofxCSG::BACK : ofxCSG::FRONT;
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
		
		vector<ofPolyline> temp;
	};
}