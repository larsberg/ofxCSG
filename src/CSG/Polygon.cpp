//
//  Polygon.cpp
//  example
//
//  Created by lars berg on 3/10/15.
//
//

#include <Polygon.h>

namespace ofxCSG
{
	Polygon::Polygon()
	{}
	
	Polygon::Polygon( ofVec3f a, ofVec3f b, ofVec3f c )
	{
		addTriangle(a, b, c);
	}
	
	Polygon::~Polygon()
	{}
	
	void Polygon::addTriangle( ofVec3f a, ofVec3f b, ofVec3f c )
	{
		triangles.push_back( Triangle( a, b, c ) );
	}
	
	void Polygon::addTriangle( Triangle t )
	{
		triangles.push_back( t );
	}
	
	void Polygon::set( ofVec3f a, ofVec3f b, ofVec3f c )
	{
		clear();
		triangles.push_back( Triangle( a, b, c ) );
	}
	
	void Polygon::addTriangle( Vertex a, Vertex b, Vertex c )
	{
		triangles.push_back( Triangle( a, b, c ) );
	}
	
	ofVec3f Polygon::getNormal()
	{
		return triangles[0].normal;
	}
	
	float Polygon::getW()
	{
		return triangles[0].w;
	}
	
	void Polygon::clear()
	{
		triangles.clear();
	}
	
	void Polygon::flip()
	{
		for(auto& t: triangles)	t.flip();
	}
	
	
	vector<ofPolyline> Polygon::toPolylines()
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
	bool Polygon::compareOfVec3f ( ofVec3f v0, ofVec3f v1)
	{
		return v0.x > v1.x || v0.y > v1.y || v0.z > v1.z;
	}
	
	vector<ofVec3f> Polygon::getVertices()
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
	
//	void Polygon::split( LineSegment segment )
//	{
//		vector<Triangle> splitTriangles;
//		
//		for(auto t: triangles)
//		{
//			auto result = t.splitWithCoplanarSegment( segment );
//			appendVectors( splitTriangles, result );
//		}
//		
//		triangles = splitTriangles;
//	}
	
	void Polygon::split( Triangle& t )
	{
		vector<Triangle> splitTriangles;
		for(auto& tri: triangles)
		{
			auto subd = tri.split( t );
			splitTriangles.insert( splitTriangles.end(), subd.begin(), subd.end() );
		}
		
		triangles = splitTriangles;
	}
	
	void Polygon::split( Polygon& p )
	{
		
		//TODO: the coplanar splitting is slow! find someone who's smarter and ask them how to do this
		//
		//if they're coplanar split them differnetly
		float nDot = getNormal().dot( p.getNormal() );
		if(false && abs(nDot) >= 1 - EPSILON )
		{
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
	
	void Polygon::split( ofVec3f t0, ofVec3f t1, ofVec3f t2 )
	{
		Polygon p( t0, t1, t2 );
		
		split( p );
	}
	
	bool Polygon::intersectRay( ofVec3f rayOrigin, ofVec3f rayDir, float epsilon, ofVec3f* intersection )
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
	
	void Polygon::classify( vector<Polygon> polygons )
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
	
	void Polygon::draw(bool useNormalForColor )
	{
		for(auto& t: triangles)
		{
			t.draw(useNormalForColor);
		}
	}
	
}
