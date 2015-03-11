//
//  Triangle.cpp
//  example
//
//  Created by lars berg on 3/10/15.
//
//

#include <Triangle.h>

namespace ofxCSG
{
	Triangle::Triangle(ofVec3f a, ofVec3f b, ofVec3f c) :
	a( a ),
	b( b ),
	c( c ),
	centroid( (a+b+c) / 3 ),
	classification( UNDEFINED ),
	normal( normalFromPoints(a, b, c) ),
	w( normal.dot(a) )
	{}
	
	Triangle::Triangle() :
	classification( UNDEFINED )
	{}
	
	Triangle::~Triangle()
	{}
	
	ofVec3f* Triangle::getPtr()
	{
		return &a;
	}
	
	ofVec3f& Triangle::operator[]( int n )
	{
		return getPtr()[n];
	}
	
	void Triangle::set( ofVec3f _a, ofVec3f _b, ofVec3f _c )
	{
		a = _a;
		b = _b;
		c = _c;
		centroid = (a+b+c) / 3;
		calcNormal();
	}
	
	void Triangle::flip()
	{
		swap(b, c);
		normal *= -1;
		w = -w;
		
		if(classification == FRONT)	classification = BACK;
		else if(classification == BACK)	classification = FRONT;
	}
	
	void Triangle::calcNormal()
	{
		normal = normalFromPoints( a, b, c );
		w = normal.dot( a );
	}
	
	float Triangle::getArea()
	{
		return areaOfTriangle( a, b, c );
	}
	
	float Triangle::getAreaSquared()
	{
		return areaOfTriangleSquared( a, b, c );
	}
	
	//derived from Akira-Hayasaka's ofxRayTriangleIntersection
	//	https://github.com/Akira-Hayasaka/ofxRayTriangleIntersection/blob/master/src/ofxRayTriangleIntersection.h
	//	assume ray direction is normalized
	bool Triangle::intersectRay( ofVec3f rayOrigin, ofVec3f rayDir, ofVec3f* intersection )
	{
		float vn = rayDir.dot(normal);
		
		ofVec3f diff = rayOrigin - a;
		float xpn = diff.dot( normal );
		float distance = -xpn / vn;
		
		if (distance < NEG_EPSILON) return false; // behind ray origin. fail
		
		ofVec3f hitPos = rayDir * distance + rayOrigin;
		
		if( isPointInTriangle( hitPos, a, b, c, normal ) )
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
	
	bool Triangle::rayIntersect( ofVec3f rayOrigin, ofVec3f rayDir )
	{
		ofVec3f diff, edge1, edge2, norm;
		
		bool backfaceCulling = true;
		
		edge1 = b-a;
		edge2 = c-a;
		norm = edge1.cross( edge2 );
		
		float sign = 1;
		
		float DdN = rayDir.dot( normal );
		if ( DdN > 0 )
		{
			//			if ( backfaceCulling ) return false;
			sign = 1;
		}
		else if ( DdN < 0 )
		{
			sign = - 1;
			DdN = - DdN;
		}
		else
		{
			return false;
		}
		
		diff = rayOrigin - a;
		auto DdQxE2 = sign * rayDir.dot( diff.cross( edge2 ) );
		
		// b1 < 0, no intersection
		if ( DdQxE2 < 0 )
		{
			return false;
		}
		
		auto DdE1xQ = sign * rayDir.dot( edge1.cross( diff ) );
		
		// b2 < 0, no intersection
		if ( DdE1xQ < 0 )
		{
			return false;
		}
		
		// b1+b2 > 1, no intersection
		if ( DdQxE2 + DdE1xQ > DdN )
		{
			return false;
		}
		
		// Line intersects triangle, check if ray does.
		auto QdN = - sign * diff.dot( normal );
		
		// t < 0, no intersection
		if ( QdN < 0 )
		{
			return false;
		}
		
		// Ray intersects triangle.
		return true;// this.at( QdN / DdN, optionalTarget );
	}
	
	
	
	//derived from Akira-Hayasaka's ofxRayTriangleIntersection
	//	https://github.com/Akira-Hayasaka/ofxRayTriangleIntersection/blob/master/src/ofxRayTriangleIntersection.h
	//	assume ray direction is normalized
	bool Triangle::intersectRay( ofVec3f rayOrigin, ofVec3f rayDir, float epsilon, ofVec3f* intersection )
	{
		float vn = rayDir.dot(normal);
		
		ofVec3f diff = rayOrigin - a;
		float xpn = diff.dot( normal );
		float distance = -xpn / vn;
		
		if (distance < NEG_EPSILON) return false; // behind ray origin. fail
		
		ofVec3f hitPos = rayDir * distance + rayOrigin;
		
		if( isPointInTriangle( hitPos, a, b, c, normal, epsilon ) )
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
	
	ofVec3f Triangle::getCenter()
	{
		return centroid;//(a + b + c) / 3.;
	}
	
	void Triangle::draw(bool useNormalForColor )
	{
		if(classification == BACK)
		{
			return;
		}
		
		if(useNormalForColor)	ofSetColor( ofFloatColor(normal.x * .5 + .5, normal.y * .5 + .5, normal.z * .5 + .5) );
		ofDrawTriangle( a, b, c );
	}
	
	ofPolyline Triangle::toPolyline()
	{
		ofPolyline p;
		p.addVertices( getPtr(), 3);
		p.setClosed(true);
		
		return p;
	}
	
	Classification Triangle::getClassification( ofVec3f planeNormal, float planeW )
	{
		int frontCount = 0, backCount = 0;
		Classification classyFries;
		for(int i=0; i<3; i++)
		{
			classyFries = classifyPointWithPlane( (*this)[i], planeNormal, planeW);
			if( classyFries == FRONT)	frontCount++;
			else if( classyFries == BACK)	backCount++;
		}
		
		if(frontCount && backCount)	return SPANNING;
		else if( backCount  )	return BACK;
		else if( frontCount )	return FRONT;
		else return COPLANAR;
	}
	
	void Triangle::classifyWithPlane( ofVec3f planeNormal, float planeW )
	{
		classification = getClassification( planeNormal, planeW );
	}
	
	vector<ofVec3f> Triangle::intersectWithPlane( ofVec3f planeNormal, float planeW )
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
	
	bool Triangle::getIntersection( Triangle t, LineSegment* overlap )
	{
		//TODO:: have intersect with plane return a LineSegment
		auto i0 = intersectWithPlane( t.normal, t.w );
		auto i1 = t.intersectWithPlane( normal, w );
		
		
		if(i0.size() < 2 || i1.size() < 2)
		{
//			cout << "ofxCSG::Triangle::getIntersection() - does this ever happen?" << endl;
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
			//TODO: do we need this second pass?
			*overlap = l1;
			return true;
		}
		
		return false;
	}
	
	vector<Triangle> Triangle::insert( ofVec3f v )
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
	
	vector<Triangle> Triangle::splitWithCoplanarSegment(ofVec3f a, ofVec3f b)
	{
		return splitWithCoplanarSegment( LineSegment(a, b) );
	}
	
	vector<Triangle> Triangle::splitWithCoplanarSegment(LineSegment segment)
	{
		vector<Triangle> triangles;
		
		if(segment.trimToTriangle(a, b, c))
		{
			//they intersect, let's split using the overlap segment
			auto firstPass = insert( segment.a );
			
			//	we need to trim the line segment for each triangle because we're inserting the end points
			//	into each triangle to subdivide.
			for(auto& tri: firstPass)
			{
				auto trimedSegment = segment;
				if(trimedSegment.trimToTriangle( tri.a, tri.b, tri.c ))
				{
					
					auto subd = tri.insert( trimedSegment.b );
					if(subd.size() == 1)
					{
						subd = tri.insert( trimedSegment.a );
					}
					
					triangles.insert( triangles.end(), subd.begin(), subd.end() );
				}
				else
				{
					triangles.push_back( tri );
				}
			}
			
			return triangles;
		}
		
		triangles.push_back( *this );
		return triangles;
	}
	
	vector<Triangle> Triangle::split( Triangle t )
	{
		vector<Triangle> triangles;
		
		auto cl = getClassification( t.normal, t.w );
		
		if( cl == SPANNING )
		{
			//otherwise check if the other triangle spans this one
			cl = t.getClassification(normal, w);
			
			//if they both span then they might intersect. so we'll find the line segment where they overlap
			//	and split it with that segment
			LineSegment overlap;
			if( cl == SPANNING && getIntersection( t, &overlap ) )
			{
				return splitWithCoplanarSegment( overlap );
			}
			else
			{
				//no intersection
				triangles.push_back( *this );
			}
		}
		else if( cl == COPLANAR )
		{
			//shit...
			triangles.push_back( *this );
		}
		else
		{
			//no intersection
			triangles.push_back( *this );
		}
		
		return triangles;
	}
	
	vector<Triangle> Triangle::meshToTriangles(ofMesh& m)
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
