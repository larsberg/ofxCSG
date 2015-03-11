//
//  Vertex.h
//
//  Created by lars berg on 3/3/15.
//

#pragma once

#include <Utils.h>

namespace ofxCSG
{
	class Vertex : public ofVec3f
	{
	public:
		Vertex( float x=0, float y=0, float z=0) :
		ofVec3f(x, y, z),
		classification(UNDEFINED)
		{}
		
		Vertex( ofVec3f v3 ) :
		ofVec3f( v3 ),
		classification(UNDEFINED)
		{}
		
		~Vertex()
		{}
		
		Classification classification;
	};
}