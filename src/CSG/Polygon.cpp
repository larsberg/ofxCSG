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
	Polygon::Polygon() :
	rayIntersectionCount( 0 )
	{}
	
	Polygon::Polygon( ofVec3f a, ofVec3f b, ofVec3f c ) :
	rayIntersectionCount( 0 )
	{
		addTriangle(a, b, c);
	}
	
	Polygon::~Polygon()
	{}
	
	void Polygon::addTriangle( ofVec3f a, ofVec3f b, ofVec3f c )
	{
		bb.addPoints( a, b, c );
		triangles.push_back( Triangle( a, b, c ) );
	}
	
	void Polygon::addTriangle( Triangle t )
	{
		bb.addPoints( t.a, t.b, t.c );
		triangles.push_back( t );
	}
	
	void Polygon::set( ofVec3f a, ofVec3f b, ofVec3f c )
	{
		clear();
		addTriangle( Triangle( a, b, c ) );
	}
	
	void Polygon::setClassification( Classification classification )
	{
		for(auto& t: triangles)
		{
			t.classification = classification;
		}
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
		bb.clear();
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
		////TODO: if they're coplanar we should split them differnetly
		//float nDot = getNormal().dot( p.getNormal() );
		//if( false && abs(nDot) >= 1 - EPSILON )
		//{
		//	cout << "CSG::Polygon::split() - we should create a coplanar split method." << endl;
		//}
		//else
		//{
			//otherwise split the triangles individually
			for( auto& t: p.triangles )
			{
				split( t );
			}
		//}
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
	
	void Polygon::classifyRay( Polygon& p, int& intersectionCount )
	{
		ofVec3f rayDir = getNormal();
		for(auto& t: triangles)
		{
			ofVec3f rayOrigin = t.getCenter();
			
			if( p.intersectRay( rayOrigin, rayDir, EPSILON, NULL ) )
			{
				intersectionCount++;
			}
		}
	}
	
	void Polygon::classify( vector<Polygon>& polygons )
	{
		ofVec3f rayDir = getNormal();
		for(auto& t: triangles)
		{	
			ofVec3f rayOrigin = t.getCenter();
			
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
