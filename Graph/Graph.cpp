///////////////////////////////////////////////////////////////
// Graph.cpp      //
// ver 1.0                                                   //
// SUID 611189677//
// Name: Chie Chih Kao
// email: ckao01@syr.edu
///////////////////////////////////////////////////////////////
/*
* Description:
* -------------------
* This cpp do the test for the Graph.cpp file.
* It test the funtion to build the graph and find the strong component
*
*Required Files:
* ---------------
* Graph.h
*/
#include <iostream>
#include <fstream>
#include "Graph.h"
using namespace GraphLib;

typedef Graph<std::string, std::string> graph;
typedef Vertex<std::string, std::string> vertex;
typedef Display<std::string, std::string> display;

void showVert(Vertex<std::string, std::string>& v)
{
	std::cout << "\n  " << v.id();
}

template<typename V, typename E>
void TshowVert(Vertex<V, E>& v)
{
	std::cout << "\n  " << v.id();
}

//#ifdef TEST_GRAPH

int main()
{
	std::cout << "\n  Testing Graph Library";
	std::cout << "\n =======================\n";
	try
	{
		std::cout << "\n  Constructing Graph instance";
		std::cout << "\n -----------------------------";
		graph g;
		vertex v1("v1", "v1");
		vertex v2("v2", "v2");
		vertex v3("v3", "v3");
		vertex v4("v4", "v4");
		vertex v5("v5", "v5");
		g.addVertex(v2);
		g.addVertex(v1);
		g.addVertex(v3);
		g.addVertex(v4);
		g.addVertex(v5);
		g.addEdge("e1", v2, v1);
		g.addEdge("e2", v1, v3);
		g.addEdge("e3", v3, v2);
		g.addEdge("e4", v1, v4);
		g.addEdge("e5", v4, v5);

		display::show(g);
		cout << endl;
		g.findSCC();
		g.showSCC();
		g.SCCtoXML();
		std::cout << "\n";
	}
	catch (std::exception& ex)
	{
		std::cout << "\n\n  " << ex.what() << "\n\n";
	}
	std::cout << "\n\n";
	system("pause");
	return 0;
}

//#endif

