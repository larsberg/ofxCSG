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
		Triangle(ofVec3f a, ofVec3f b, ofVec3f c) :
		a( a ),
		b( b ),
		c( c ),
		classification( UNDEFINED ),
		normal( normalFromPoints(a, b, c) ),
		w( normal.dot(a) )
		{}
		
		Triangle() :
		classification( UNDEFINED )
		{}
		
		~Triangle()
		{}
		
		ofVec3f* getPtr() {
			return &a;
		}
		
		ofVec3f& operator[]( int n ){
			return getPtr()[n];
		}
		
		void set( ofVec3f _a, ofVec3f _b, ofVec3f _c )
		{
			a = _a;
			b = _b;
			c = _c;
			calcNormal();
		}
		
		void flip()
		{
			swap(b, c);
			normal *= -1;
			w = -w;
			
			if(classification == FRONT)	classification = BACK;
			else if(classification == BACK)	classification = FRONT;
		}
		
		void calcNormal()
		{
			normal = normalFromPoints( a, b, c );
			w = normal.dot( a );
		}
		
		float getArea()
		{
			return areaOfTriangle( a, b, c );
		}
		
		float getAreaSquared()
		{
			return areaOfTriangleSquared( a, b, c );
		}
		
		
		
		//derived from Akira-Hayasaka's ofxRayTriangleIntersection
		//	https://github.com/Akira-Hayasaka/ofxRayTriangleIntersection/blob/master/src/ofxRayTriangleIntersection.h
		//	assume ray direction is normalized
		bool intersectRay( ofVec3f rayOrigin, ofVec3f rayDir, ofVec3f* intersection = NULL )
		{
			float vn = rayDir.dot(normal);
			
			ofVec3f diff = rayOrigin - a;
			float xpn = diff.dot( normal );
			float distance = -xpn / vn;
			
			if (distance < 0) return false; // behind ray origin. fail
			
			ofVec3f hitPos = rayDir * distance + rayOrigin;
			
			if( isPointInTriangle( hitPos, a, b, c ) )
			{
				//it's a hit
				if(intersection!= NULL)
				{
					*intersection = hitPos;
				}
				return true;
			}
			
			//nada
			return false;
		}
		
		
		ofVec3f getCenter()
		{
			return (a + b + c) / 3.;
		}
		
		void draw()
		{
			if(classification == FRONT)
			{
				ofSetColor(0, 255, 255, 255);
				ofDrawTriangle( a, b, c );
			}
			else if(classification == BACK)
			{
				ofSetColor(66, 255, 55, 255);
				ofDrawTriangle( a, b, c );
			}
			else
			{
				ofSetColor( 255, 255 );
				ofDrawTriangle( a, b, c );
			}
			
			ofSetColor( ofFloatColor(normal.x * .5 + .5, normal.y * .5 + .5, normal.z * .5 + .5) );
			ofDrawTriangle( a, b, c );
		}
		
		Classification getClassification( ofVec3f planeNormal, float planeW )
		{
			
			int frontCount = 0, backCount = 0;
			for(int i=0; i<3; i++)
			{
				auto c = classifyPointWithPlane( (*this)[i], planeNormal, planeW);
				if(c == FRONT)	frontCount++;
				else if(c == BACK)	backCount++;
			}
			
			if(frontCount && backCount)	return SPANNING;
			else if( backCount )	return BACK;
			else if( frontCount )	return FRONT;
			else return COPLANAR;
		}
		
		void classifyWithPlane( ofVec3f planeNormal, float planeW )
		{
			classification = getClassification( planeNormal, planeW );
		}
		
		vector<Triangle> splitWithCoplanarTriangle(Triangle& t, float normalDotNormal)
		{
			vector<Triangle> triangles;
			
			//then lets subtract one form the other
			//
			//ofTessellator seemed like the easiest solution here... we should probably go a little lower level though
			vector<ofPolyline> polylines(2);
			
			polylines[0].addVertex( a );
			polylines[0].addVertex( b );
			polylines[0].addVertex( c );
			
			polylines[1].addVertex( t.a );
			polylines[1].addVertex( t.b );
			polylines[1].addVertex( t.c );
			
			ofTessellator tess;
			ofMesh m;
			
			//if they are facing opposite directions we'll cut out the wholes
			ofPolyWindingMode windMode = normalDotNormal > 0 ? OF_POLY_WINDING_POSITIVE : OF_POLY_WINDING_ODD;
			
			tess.tessellateToMesh( polylines, windMode, m);
			
			auto v = m.getVertices();
			auto indices = m.getIndices();
			
			for(int i=0; i<indices.size(); i+=3)
			{
				triangles.push_back( Triangle(v[indices[i]], v[indices[i+1]], v[indices[i+2]] ) );
			}
			
			return triangles;
		}
		
		vector<ofVec3f> intersectWithPlane( ofVec3f planeNormal, float planeW )
		{
			vector<ofVec3f> intersections;
			
			ofVec3f intersection;
			
			for(int i=0, j=1; i<3; i++, j++)
			{
				if( splitLineSegmentWithPlane( (*this)[i], (*this)[j%3], planeNormal, planeW, &intersection) )
				{
					intersections.push_back( intersection );
				}
			}
			
			return intersections;
		}
		
		bool getIntersection( Triangle& t, LineSegment* overlap )
		{
			//TODO:: have intersect with plane return a LineSegment
			auto i0 = intersectWithPlane( t.normal, t.w );
			auto i1 = t.intersectWithPlane( normal, w );
			
			if(i0.size() < 2 || i1.size() < 2)
			{
				return false;
			}
			
			LineSegment l0( i0[0], i0[1] );
			LineSegment l1( i1[0], i1[1] );
			
			for(int i=2;i<i0.size();i++)	l0.expandToPoint( i0[i] );
			for(int i=2;i<i1.size();i++)	l1.expandToPoint( i1[i] );
			
			if( l0.subtract( l1 )  )
			{
				*overlap = l0;
				return true;
			}
			else if( l1.subtract( l0 ) )
			{
				*overlap = l1;
				return true;
			}
			
			return false;
		}
		
		vector<Triangle> insert( ofVec3f v )
		{
			vector<Triangle> triangles;
			
			if( abs(distanceToPlaneSigned( v, a, normal) ) > EPSILON)
			{
				triangles.push_back( *this );
				return triangles;
			}
			
			//make three triangles
			for(int i=0; i<3; i++)
			{
				Triangle t( (*this)[i], (*this)[(i+1)%3], v);
				t.classification - classification;
				if(t.getAreaSquared() > EPSILON)
				{
					triangles.push_back(t);
				}
			}
			
			return triangles;
		}
		
		vector<Triangle> split( Triangle& t )
		{
			vector<Triangle> triangles;
			
			auto c = getClassification( t.normal, t.w );
			
			if( c == SPANNING || c == COPLANAR )
			{
				//are they coplanar? if so, subtract or add the triangles
				auto nDot = normal.dot( t.normal );
				if( abs( nDot ) >= 1.)//do we need EPSILON here?
				{
					return splitWithCoplanarTriangle( t, nDot);
				}
				
				//otherwise check if the other triangle spans this one
				c = t.getClassification(normal, w);
				
				//if they both span they might intersect. so we'll find the line segment where they overlap
				//	and split it with that segment
				LineSegment overlap;
				if( c == SPANNING && getIntersection( t, &overlap ) )
				{
					//they intersect, let's split using the overlap segment
					auto firstPass = insert( overlap.a );
					
					for(auto& tri: firstPass)
					{
						auto trimedOverlap = overlap;
						trimedOverlap.trimToTriangle( tri.a, tri.b, tri.c );
						
						auto subd = tri.insert( trimedOverlap.a );
						if(subd.size() == 1)
						{
							subd = tri.insert( trimedOverlap.b );
						}
						
						//if(subd.size() > 1)
						//{
						//	for(auto& st: subd)
						//	{
						//		c = st.getClassification( t.normal, t.w );
						//		if( c == FRONT || c == BACK )
						//		{
						//			st.classification = c;
						//		}
						//	}
						//}
						
						triangles.insert( triangles.end(), subd.begin(), subd.end() );
					}
				}
				else
				{
					triangles.push_back( *this );
				}
			}
			else
			{
				triangles.push_back( *this );
			}
			
			return triangles;
		}
		
		
		
		ofVec3f a, b, c;
		ofVec3f normal;
		float w;
		Classification classification;
	};
}