//
//  ofxCSG.h
//
//  Created by lars berg on 3/3/15.
//

#pragma once

#include <CSG/Polygon.h>


namespace ofxCSG
{
	static void setMeshFromPolygons(ofMesh& m, vector<ofxCSG::Polygon>& polygons)
	{
		m.clear();
		
		for(auto& p: polygons)
		{
			for(auto& t: p.triangles)
			{
				if(t.classification == FRONT)
				{
					m.addVertex( t.a );
					m.addVertex( t.b );
					m.addVertex( t.c );
				}
			}
		}
	}
	
	static vector<ofxCSG::Polygon> meshToPolygons(ofMesh& m)
	{
		vector<ofxCSG::Polygon> polygons;
		
		auto indices = m.getIndices();
		auto v = m.getVertices();
		
		if(indices.size())
		{
			for(int i=0; i<indices.size(); i+=3)
			{
				polygons.push_back( ofxCSG::Polygon( v[ indices[i] ], v[ indices[i+1] ], v[ indices[i+2] ] ) );
			}
		}
		else
		{
			for(int i=0; i<v.size(); i+=3)
			{
				polygons.push_back( ofxCSG::Polygon( v[i], v[i+1], v[i+2] ) );
			}
		}
		
		
		return polygons;
	}
}