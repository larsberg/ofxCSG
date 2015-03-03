//
//  Triangle.h
//
//  Created by lars berg on 3/3/15.
//

#pragma once

#include <Vertex.h>
#include <LineSegment.h>

namespace ofxCSG
{
	class Triangle
	{
	public:
		Triangle(shared_ptr<Vertex> a, shared_ptr<Vertex> b, shared_ptr<Vertex> c) :
		a( a ),
		b( b ),
		c( c ),
		classification( UNDEFINED ),
		normal( normalFromPoints( *a, *b, *c ) )
		{}
		
		Triangle(ofVec3f a, ofVec3f b, ofVec3f c) :
		a( shared_ptr<Vertex>( new Vertex( a ) ) ),
		b( shared_ptr<Vertex>( new Vertex( b ) ) ),
		c( shared_ptr<Vertex>( new Vertex( c ) ) ),
		classification( UNDEFINED ),
		normal( normalFromPoints(a, b, c) )
		{}
		
		Triangle() :
		classification( UNDEFINED )
		{}
		
		~Triangle()
		{}
		
		shared_ptr<Vertex> * getPtr() {
			return &a;
		}
		
		shared_ptr<Vertex>& operator[]( int n ){
			return getPtr()[n];
		}
		
		
		void set( shared_ptr<Vertex> _a, shared_ptr<Vertex> _b, shared_ptr<Vertex> _c )
		{
			a = _a, b = _b, c = _c;
			calcNormal();
		}
		
		void set( ofVec3f _a, ofVec3f _b, ofVec3f _c )
		{
			a = shared_ptr<Vertex>( new Vertex( _a ) ), b = shared_ptr<Vertex>( new Vertex( _b ) ), c = shared_ptr<Vertex>( new Vertex( _c ) );
			calcNormal();
		}
		
		void flip()
		{
			swap(b, c);
			normal *= -1;
			if(classification == FRONT)	classification = BACK;
			else if(classification == BACK)	classification = FRONT;
		}
		
		void calcNormal()
		{
			normal = normalFromPoints( *a, *b, *c );
		}
		
		float getArea()
		{
			return areaOfTriangle(*a, *b, *c);
		}
		
		float getAreaSquared()
		{
			return areaOfTriangleSquared(*a, *b, *c);
		}
		
		void draw()
		{
			if(a != NULL && b != NULL && c != NULL)
			{
				if(classification == FRONT)
				{
					ofSetColor(0, 255, 255, 55);
				}
				else if(classification == BACK)
				{
					ofSetColor(66, 255, 55, 55);
				}
				else
				{
					ofSetColor(255, 55);
				}
				ofDrawTriangle( *a, *b, *c );
			}
		}
		
		
		//COME UP WITH SOMETHIN TO WRITE HERE
		
		bool isPointInside( ofVec3f p )
		{
			return isPointInTriangle( p, *a, *b, *c );
		}
		
		bool isPointInside( shared_ptr<Vertex>& v )
		{
			return isPointInTriangle( *v, *a, *b, *c );
		}
		
		int intersectLineSegment(ofVec3f p0, ofVec3f p1, ofVec3f* intersection = NULL)
		{
			auto d0 = distanceToPlaneSigned( p0, *a, normal );
			auto d1 = distanceToPlaneSigned( p1, *a, normal );
			
			//if( (d0 > 0 && d1 > 0) || ( d0 < 0 && d1 < 0 ) )
			if( (d0 >= EPSILON && d1 >= EPSILON) || ( d0 <= NEG_EPSILON && d1 <= NEG_EPSILON ) )
			{
				//no intersection
				return 0;
			}
			if( fabs(d0) < EPSILON && fabs(d1) < EPSILON )
			{
				//it's coplanar
				return 2;
			}
			
			//it's a hit
			ofVec3f pos = lerp( p0, p1, d0 / (d0 - d1) );
			
			if( intersection != NULL )
			{
				//lerp using the distance to plane values
				*intersection = pos;
			}
			return isPointInside( pos );
		}
		
		vector<Triangle> subdivide( ofVec3f p )
		{
			auto v = shared_ptr<Vertex>( new Vertex( p ) );
			return subdivide( v );
		}
		
		vector<Triangle> subdivide( shared_ptr<Vertex> v )
		{
			vector<Triangle> triangles;
			
			if( isPointInside( *v ) )
			{
				//make three triangles
				for(int i=0; i<3; i++)
				{
					Triangle t( (*this)[i], (*this)[(i+1)%3], v);
					if(t.getAreaSquared() > EPSILON)
					{
						triangles.push_back(t);
					}
				}
			}
			else
			{
				triangles.push_back( *this );
			}
			
			return triangles;
		}
		
		
		//ripped from https://github.com/chandlerprall/ThreeCSG/blob/master/ThreeCSG.js
		Classification classifyWithPlane( ofVec3f planePos, ofVec3f planeNormal )
		{
			int num_positive = 0, num_negative = 0;
			for(int i=0; i<3; i++)
			{
				auto c = classifyPointWithPlane( *(*this)[i], planePos, planeNormal );
				if(c == FRONT)	num_positive++;
				else if(c == BACK)	num_negative++;
			}
			
			if ( num_positive > 0 && num_negative == 0 )
			{
				classification = FRONT;
			}
			else if ( num_positive == 0 && num_negative > 0 )
			{
				classification = BACK;
			}
			else if ( num_positive == 0 && num_negative == 0 )
			{
				classification = COPLANAR;
			}
			else
			{
				classification = SPANNING;
			}
			
			return classification;
		}
		
		vector<Triangle> insert( ofVec3f p )
		{
			auto v = shared_ptr<Vertex>( new Vertex( p ) );
			return insert( v );
		}
		
		vector<Triangle> insert( shared_ptr<Vertex> v )
		{
			vector<Triangle> triangles;
			
			if( abs(distanceToPlaneSigned( *v, *a, normal) ) > EPSILON)
			{
				triangles.push_back( *this );
				return triangles;
			}
			
			//make three triangles
			for(int i=0; i<3; i++)
			{
				Triangle t( (*this)[i], (*this)[(i+1)%3], v);
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
			
			bool spanning = classifyWithPlane( *t.a, t.normal ) == SPANNING;
			
			if( spanning )
			{
				//intersect and subdivide them
				
				//are they coplanar?
				auto nDot = normal.dot( t.normal );
				if( abs( nDot ) >= 1.)//do we need EPSILON here?
				{
					//then lets subtract one form the other
					//
					//ofTessellator seemed like the easiest solution here... we should probably go a little lower level though
					vector<ofPolyline> polylines(2);
					
					polylines[0].addVertex( *a );
					polylines[0].addVertex( *b );
					polylines[0].addVertex( *c );
					
					polylines[1].addVertex( *t.a );
					polylines[1].addVertex( *t.b );
					polylines[1].addVertex( *t.c );
					
					ofTessellator tess;
					ofMesh m;
					
					//if they are facing opposite directions we'll cut out the wholes
					ofPolyWindingMode windMode = nDot > 0 ? OF_POLY_WINDING_POSITIVE : OF_POLY_WINDING_ODD;
					
					tess.tessellateToMesh( polylines, windMode, m);
					
					auto v = m.getVertices();
					auto indices = m.getIndices();
					
					for(int i=0; i<indices.size(); i+=3)
					{
						triangles.push_back( Triangle(v[indices[i]], v[indices[i+1]], v[indices[i+2]] ) );
					}
					
					return triangles;
				}
				
				//OTHERWISE let's split it using the line segment where they intersect
				triangles = overlapSplit( t );
				
				return triangles;
			}
			
			triangles.push_back( *this );
			return triangles;
		}
		
		
		vector<ofVec3f> intersectWithPlane( ofVec3f planePos, ofVec3f planeNormal, Classification* classification_ptr = NULL )
		{
			Classification ca, cb, cc;
			ca = classifyPointWithPlane( *a, planePos, planeNormal );
			cb = classifyPointWithPlane( *b, planePos, planeNormal );
			cc = classifyPointWithPlane( *c, planePos, planeNormal );
			
			vector<ofVec3f> intersections;
			
			//if they're all the same we don't need to intersect edges
			if(ca == cb && ca == cc)
			{
				if(ca == SPANNING)
				{
					//it's COPLANAR
					intersections.push_back( *a );
					intersections.push_back( *b );
					intersections.push_back( *c );
					
					if(classification_ptr != NULL)	*classification_ptr = COPLANAR;
					return intersections;
				}
				
				//it's either FRONT or BACK. return an empty vector
				if(classification_ptr != NULL)	*classification_ptr = ca;
				return intersections;
			}
			
			//we know its spanning so let's intersect the edges with the plane
			ofVec3f p;
			if(ca != cb)
			{
				if( intersectLineSegmentPlane( *a, *b, planePos, planeNormal, &p ) == 1)
				{
					intersections.push_back( p );
				}
			}
			if(cb != cc)
			{
				if( intersectLineSegmentPlane( *b, *c, planePos, planeNormal, &p ) == 1)
				{
					intersections.push_back( p );
				}
			}
			if(cc != ca )
			{
				//				if(intersections.size() != 1)	cout << "intersections.size(): " << intersections.size() << endl;
				if( intersectLineSegmentPlane( *c, *a, planePos, planeNormal, &p ) == 1 && intersections.size() < 2)
				{
					intersections.push_back( p );
				}
			}
			
			return intersections;
		}
		
		
		bool getIntersection( Triangle& t, LineSegment* overlap )
		{
			//TODO:: have intersect with plane return a LineSegment
			auto i0 = intersectWithPlane( *t.a, t.normal );
			auto i1 = t.intersectWithPlane( *a, normal );
			
			
			if( i0.size() == 2 && i1.size() == 2 )
			{
				
				if( i0[0] == i0[1] || i1[0] == i1[1] )
				{
					//cout << "i0[0] == i0[1] || i1[0] == i1[1]" << endl;
					return false;
				}
				
				LineSegment l0( i0[0], i0[1] );
				LineSegment l1( i1[0], i1[1] );
				
				
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
			}
			
			return false;
		}
		
		vector<Triangle> overlapSplit( Triangle& t )
		{
			vector<Triangle> triangles;
			
			LineSegment overlap;
			if( getIntersection(t, &overlap) )
			{
				auto firstPass = insert( overlap.a );
				
				for(auto& tri: firstPass)
				{
					auto trimedOverlap = overlap;
					trimedOverlap.trimToTriangle( *tri.a, *tri.b, *tri.c );
					
					auto subd = tri.insert( trimedOverlap.b );
					if(subd.size() == 1)
					{
						subd = tri.insert( trimedOverlap.a );
					}
					triangles.insert( triangles.end(), subd.begin(), subd.end() );
				}
			}
			else
			{
				triangles.push_back( *this );
			}
			
			return triangles;
		}
		
		
		
		
		shared_ptr<Vertex> a, b, c;
		ofVec3f normal;
		Classification classification;
	};
}