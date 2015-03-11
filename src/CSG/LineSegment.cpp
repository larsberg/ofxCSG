//
//  LineSegment.cpp
//  example
//
//  Created by lars berg on 3/10/15.
//
//

#include <LineSegment.h>



namespace ofxCSG
{
	LineSegment::LineSegment( ofVec3f a, ofVec3f b) :
	a(a), b(b)
	{}
	
	LineSegment::LineSegment()
	{}
	
	void LineSegment::set( ofVec3f p0, ofVec3f p1 )
	{
		a = p0, b = p1;
	}
	
	void LineSegment::expandToPoint( ofVec3f p )
	{
		if(a == b)
		{
			b = p;
			return;
		}
		
		float u = getLineSegmentUValue(a, b, p);
		if(u < 0)	a = p;
		else if(u > 1)	b = p;
	}
	
	float LineSegment::lengthSquared()
	{
		return a.squareDistance( b );
	}
	
	bool LineSegment::subtract( LineSegment segment )
	{
		return subtract( segment.a, segment.b );
	}
	
	bool LineSegment::subtract( ofVec3f p0, ofVec3f p1 )
	{
		if(!isPointInLineSegment( a, b, p0 ) && !isPointInLineSegment( a, b, p1 ))
		{
			//				cout << "!isPointInLineSegment( a, b, p0 ) && !isPointInLineSegment( a, b, p1 )" << endl;
			return false;
		}
		
		float uP0 = getLineSegmentUValue( a, b, p0 );
		float uP1 = getLineSegmentUValue( a, b, p1 );
		
		if( uP1 < uP0 )
		{
			swap(p0, p1);
			swap(uP0, uP1);
		}
		
		if(uP1 == uP0)
		{
			cout << "SHIT!" << p0 << ", " << p1 << endl << endl;
		}
		
		a = uP0 > 0 ? p0 : a;
		b = uP1 < 1 ? p1 : b;
		
		return true;
	}
	
	bool LineSegment::trimToTriangle( ofVec3f ta, ofVec3f tb, ofVec3f tc)
	{
		//get the intersections
		vector<ofVec3f> intersections;
		ofVec3f intersection;
		ofVec3f normal = normalFromPoints( ta, tb, tc);
		
		//if the points are inside let's keep them
		if( isPointInTriangle( a, ta, tb, tc, normal, NEG_EPSILON ))
		{
			intersections.push_back( a );
		}
		if( isPointInTriangle( b, ta, tb, tc, normal, NEG_EPSILON ))
		{
			intersections.push_back( b );
		}
		
		// if there both inside we're done
		if(intersections.size() == 2)	return true;
		
		
		// otherwise let's intersect with the triangle edges
		if(intersectLineSegments(a, b, ta, tb, &intersection))
		{
			intersections.push_back( intersection );
		}
		if(intersectLineSegments(a, b, tb, tc, &intersection))
		{
			intersections.push_back( intersection );
		}
		if(intersectLineSegments(a, b, tc, ta, &intersection))
		{
			intersections.push_back( intersection );
		}
		
		//rebuild a ine segment if there's enough intersections
		if( intersections.size() > 1 )
		{
			LineSegment trimmedLine(intersections[0], intersections[1]);
			
			for(int i=2; i<intersections.size(); i++)
			{
				trimmedLine.expandToPoint( intersections[i] );
			}
			
			a = trimmedLine.a;
			b = trimmedLine.b;
			
			return true;
		}
		
		return false;
	}
	
	ofVec3f* LineSegment::getPtr()
	{
		return &a;
	}
	
	ofVec3f& LineSegment::operator[]( int n )
	{
		return getPtr()[n];
	}
}