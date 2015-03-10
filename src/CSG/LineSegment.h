//
//  LineSegment.h
//
//  Created by lars berg on 3/3/15.
//

#pragma once

#include <Utils.h>

namespace ofxCSG
{
	class LineSegment
	{
	public:
		LineSegment( ofVec3f a, ofVec3f b);
		
		LineSegment();
		
		void set( ofVec3f p0, ofVec3f p1 );
		
		void expandToPoint( ofVec3f p );
		
		float lengthSquared();
		
		bool subtract( LineSegment segment );
		
		bool subtract( ofVec3f p0, ofVec3f p1 );
		
		bool trimToTriangle( ofVec3f ta, ofVec3f tb, ofVec3f tc);
		
		ofVec3f* getPtr();
		
		ofVec3f& operator[]( int n );
		
		ofVec3f a, b, normal;
	};
}