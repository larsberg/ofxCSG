//
//  ofxCSG.h
//
//  Created by lars berg on 3/3/15.
//

#pragma once

#include <CSG/Polygon.h>


namespace ofxCSG
{
	
	static void addPolygonsToMesh(ofMesh& m, vector<ofxCSG::Polygon>& polygons)
	{
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
	
	static vector<ofPolyline> polygonsToPolylines( vector<Polygon>& polygons )
	{
		vector<ofPolyline> polylines;
		for(auto& p: polygons)
		{
			auto pl = p.toPolylines();
			
			polylines.insert(polylines.end(), pl.begin(), pl.end() );
		}
		
		return polylines;
	}
	
	
	static void meshBoolean( ofMesh& a, ofMesh& b, ofMesh& m, bool flipA, bool flipB)
	{
		
		// get our polygons
		auto polygonsA = meshToPolygons( a );
		auto polygonsB = meshToPolygons( b );
		
		auto orig_polygonsA = polygonsA;
		auto orig_polygonsB = polygonsB;
		
		//split the polygons with eachother
		for(auto& pa: polygonsA)
		{
			for( auto& pb: orig_polygonsB )
			{
				pa.split( pb );
			}
		}
		
		for(auto& pb: polygonsB)
		{
			for( auto& pa: orig_polygonsA )
			{
				pb.split( pa );
			}
		}
		
		//classy the triangles
		ofVec3f rayDir(0,1,0);
		for(auto& p: polygonsA)
		{
			p.classify( orig_polygonsB );
		}
		
		for(auto& p: polygonsB)
		{
			p.classify( orig_polygonsA );
		}
		
		if(flipA)
		{
			for(auto& p: polygonsA)	p.flip();
		}
		
		if(flipB)
		{
			for(auto& p: polygonsB)	p.flip();
		}
		
		//add the polygons to out outMesh
		m.clear();
		addPolygonsToMesh( m, polygonsA );
		addPolygonsToMesh( m, polygonsB );
	}
	
	static void meshUnion( ofMesh& a, ofMesh& b, ofMesh& outMesh )
	{
		meshBoolean( a, b, outMesh, false, false );
	}
	
	static void meshIntersection( ofMesh& a, ofMesh& b, ofMesh& outMesh )
	{
		meshBoolean( a, b, outMesh, true, true );
	}
	
	static void meshDifference( ofMesh& a, ofMesh& b, ofMesh& outMesh )
	{
		meshBoolean( a, b, outMesh, false, true );
	}
}