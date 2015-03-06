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
		
		ofVec3f getNormal()
		{
			return triangles[0].normal;
		}
		
		float getW()
		{
			return triangles[0].w;
		}
		
		void clear()
		{
			triangles.clear();
		}
		
		void flip()
		{
			for(auto& t: triangles)	t.flip();
		}
		
		
		vector<ofPolyline> toPolylines()
		{
			vector<ofPolyline> polylines;
			
			for(auto& t: triangles)
			{
				ofPolyline p;
				p.addVertex( t.a );
				p.addVertex( t.b );
				p.addVertex( t.c );
				
				p.setClosed(true);
				
				polylines.push_back( p );
			}
			
			return polylines;
		}
		
		//campare two RampItems using their u values
		static bool compareOfVec3f ( ofVec3f v0, ofVec3f v1)
		{
			return v0.x > v1.x || v0.y > v1.y || v0.z > v1.z;
		}
		
		vector<ofVec3f> getVertices()
		{
			vector<ofVec3f> v;
			for(auto& t: triangles)
			{
				v.push_back( t.a );
				v.push_back( t.b );
				v.push_back( t.c );
			}
			
			cout << "v.size(): " << v.size() << " -> ";
			sort( v.begin(), v.end(), compareOfVec3f );
			unique( v.begin(), v.end() );
			cout << v.size() << endl;;
		
			return v;
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
			float nDot = getNormal().dot( p.getNormal() );
			if( abs(nDot) <= 1 - EPSILON )
			{
				
//				if( distanceToPlane( p.triangles[0].a, triangles[0].a, getNormal() ) <= EPSILON )
				//if( abs( distanceToPlaneSigned( p.triangles[0].a, triangles[0].a, triangles[0].normal ) ) <= EPSILON )
				if( isPointOnPlane( p.triangles[0].a, getNormal(), getW(), .0001 ) )
				{
					cout << "isPointOnPlane" << endl;
//					auto p0 = toPolylines();
//					auto p1 = p.toPolylines();
//					
//					ofTessellator tess;
//					tess.tessellateToPolylines( p0, OF_POLY_WINDING_POSITIVE, p0 );
//					
////					p1.insert( p0.end(), p1.begin(), p1.end() );
//					
//					ofMesh m;
//					tess.tessellateToMesh( p0, OF_POLY_WINDING_POSITIVE, m );
//					
//					triangles = meshToTriangles( m );
//					cout << "coplanarSplit:: triangles.size(): " << triangles.size() << endl;
					
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
				ofVec3f rayDir = getNormal();
				
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
		
		void draw(bool useNormalForColor = true)
		{
			for(auto& t: triangles)
			{
				t.draw(useNormalForColor);
			}
		}
		
		vector<Triangle> triangles;
		
		vector<ofPolyline> temp;
	};
}