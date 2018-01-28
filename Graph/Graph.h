#ifndef GRAPH_H
#define GRAPH_H
///////////////////////////////////////////////////////////////
// Graph.h      //
// ver 1.0                                                   //
// SUID 611189677//
// Name: Chie Chih Kao
// email: ckao01@syr.edu
// source: from Prof. Jim Fawcett
// Strong Conponent algorithm referenced from
//http://www.geeksforgeeks.org/tarjan-algorithm-find-strongly-connected-components/
///////////////////////////////////////////////////////////////
/*
* Description:
* -------------------
* This file is based on prof. Jim Fawcett's Graph package and add the functionality
to find the strong component in graph and buil the result XML file.
*
* Funtion I Add:
* -------------------
* In Graph Class:
* SCCrec(): A recursive funciton to use Trajan's Algoritm to find the strong component
* findSCC(): function to run the SCCrec();
* showSCC(): simple funtion to display SCC;
* SCCtoXML(): display SCC in XML format;
/*
Package Operations:
===================
A graph is a collection of vertices, connected by edges.  Each
vertex holds some kind of information held in an instance of
a type V and an integer id, which should be unique with a given
graph.  Each edge connects a parent vertex with a child
vertex and it too holds information in an instance of a type E.

This package provides two classes that support creation of
instances of a graph: Vertex<V,E> and Graph<V,E>.  These are
template-based with parameters V and E, as described above.

The graph holds an aggregated collection of vertices.  Each
vertex may hold one or more edges.  The edge is implemented
as an instance of std::pair<int,E> where the first parameter
is the id of the child vertex and the second parameter is
an instance of the edge information type E.

Note:
It is important that vertex ids be unique.  If you set any of
them with the second argument of the Vertex constructor or with
the id() function it is up to you to ensure uniqueness.  If you
don't explicitly set any ids then the constructor insures that
vertices get sequential ids in the order constructed.

Maintanence Information:
========================
Required files:
---------------
Graph.h, Graph.cpp,
XmlReader.h, XmlReader.cpp,
XmlWriter.h, XmlWriter.cpp

Build Process:
--------------
Using Visual Studio Command Prompt:
devenv Graph.sln /rebuild debug

Revision History:
-----------------
ver 1.5 : 16 Feb 13
- now supports dfs with both function pointers and functors
ver 1.4 : 11 Feb 13
- removed indent function, now part of XmlWriter,
replaced ctor with default param with two ctors,
one supplies sequential ids, the other accepts user defined id
ver 1.3 : 28 Jan 13
- added GraphToXmlString and GraphFromXmlString functions
ver 1.2 : 25 Jan 13
- changed id() return to reference so can be changed
ver 1.1 : 21 Jan 13
- added prologue comments, compile test directive
ver 1.0 : 14 Jan 13
- first release
*/

#include <vector>
#include <unordered_map>
#include <string>
#include "XmlWriter.h"
#include "XmlReader.h"
#include <iostream>
#include <sstream>
#include <list>
#include <stack>
#include <algorithm>
using namespace std;
namespace GraphLib
{
	///////////////////////////////////////////////////////////////
	// Vertex class

	template<typename V, typename E>
	class Vertex
	{
	public:
		typedef std::pair<size_t, E> Edge;  // graph index of target vertex, edge type
		typename typedef std::vector<Edge>::iterator iterator;
		iterator begin();
		iterator end();
		Vertex(V v, size_t id);
		Vertex(V v);
		Vertex(V v, std::string idstr);
		void add(Edge& edge);
		// Vertex(const Vertex<V,E>& v);                  compiler generated is correct
		// Vertex<V,E>& operator=(const Vertex<V,E>& v);  compiler generated is correct
		Edge& operator[](size_t i);
		Edge operator[](size_t i) const;
		V& value();
		std::string& id();
		size_t size();
		bool& mark();
	private:
		std::vector<Edge> _edges;
		V _v;
		size_t _id;
		std::string _idstr;
		static size_t count;
		bool _mark;
	};
	//----< reserve memory for, and initialize, static count >-----
	template<typename V, typename E>
	size_t Vertex<V, E>::count = 0;

	//----< set and return boolean mark, used for traversal >------
	template<typename V, typename E>
	bool& Vertex<V, E>::mark() { return _mark; }

	//----< return iterator pointing to first edge >---------------
	template<typename V, typename E>
	typename Vertex<V, E>::iterator Vertex<V, E>::begin() { return _edges.begin(); }

	//----< return iterator pointing to one past last edge >-------
	template<typename V, typename E>
	typename Vertex<V, E>::iterator Vertex<V, E>::end() { return _edges.end(); }

	//----< construct instance, specifying id - must be unique >---
	template<typename V, typename E>
	Vertex<V, E>::Vertex(V v, size_t id) : _v(v), _id(id), _mark(false) {}

	template<typename V, typename E>
	Vertex<V, E>::Vertex(V v, std::string idstr) : _v(v), _idstr(idstr), _mark(false) {}

	//----< construct instance - creates id sequentially >-------
	template<typename V, typename E>
	Vertex<V, E>::Vertex(V v) : _v(v), _id(count++), _mark(false) {}

	//----< add edge to vertex edge collection >-------------------
	template<typename V, typename E>
	void Vertex<V, E>::add(Edge& edge) { _edges.push_back(edge); }

	//----< index non-const vertex's edges >-----------------------
	template<typename V, typename E>
	typename Vertex<V, E>::Edge& Vertex<V, E>::operator[](size_t i) { return _edges[i]; }

	//----< index const vertex's edges >---------------------------
	template<typename V, typename E>
	typename Vertex<V, E>::Edge Vertex<V, E>::operator[](size_t i) const { return _edges[i]; }

	//----< set and read value of vertex's held type, V >----------
	template<typename V, typename E>
	V& Vertex<V, E>::value() { return _v; }

	//----< return vertex's id >-----------------------------------
	template<typename V, typename E>
	std::string& Vertex<V, E>::id() { return _idstr; }

	//----< return number of edges >-------------------------------
	template<typename V, typename E>
	size_t Vertex<V, E>::size() { return _edges.size(); }

	///////////////////////////////////////////////////////////////
	// Graph class

	template<typename V, typename E>
	class Graph
	{
	public:
		typedef std::pair<size_t, E> Edge;  // graph index of target vertex, edge type
		typename typedef std::vector<Edge>::iterator iterator2;
		typename typedef std::vector< Vertex<V, E> >::iterator iterator;
		iterator begin();
		iterator end();
		// Graph(const Graph<V,E>& g);                  compiler generated is correct
		// Graph<V,E>& operator=(const Graph<V,E>& g);  compiler generated is correct
		Vertex<V, E>& operator[](size_t i);
		Vertex<V, E> operator[](size_t i) const;
		void addVertex(Vertex<V, E> v);
		void addEdge(E eval, Vertex<V, E>& parent, Vertex<V, E>& child);
		size_t findVertexIndexById(std::string idstr);
		size_t size();
		template<typename F>
		void dfs(Vertex<V, E>& v, F f);
		Vertex<V, E> getVertexById(std::string idstr);
		void SCCrec(int u, int disc[], int low[], stack<int> *st, bool stackMember[]);
		void findSCC();
		void SCCtoXML();
		bool hasKey(std::string key)
		{
			if (idMap.find(key) == idMap.end())
				return false;
			return true;
		}
		void showSCC();
	private:
		std::vector< Vertex<V, E> > adj;
		std::unordered_map<std::string, size_t> idMap; // id maps to graph index
		vector<vector<string>> SCCLlist;
		template<typename F>
		void dfsCore(Vertex<V, E>& v, F f);
	};
	//show strong component
	template<typename V, typename E>
	void Graph<V, E>::showSCC() {
		cout << "\n\n----Req#7 Show Strong Component File List----\n";
		if (SCCLlist.size() == 0)
			cout << "\n No Strong Component found;";
		for (auto v : SCCLlist) {
			cout << "\n Strong Component List: ";
			for (auto data : v)
				cout << data << ", ";
		}
	}
	//convert Strong component to XML string
	template<typename V, typename E>
	void Graph<V, E>::SCCtoXML() {
		cout << "\n\n----Req#7 Show Strong Component in XML Format----\n";
		if (SCCLlist.size() == 0)
			cout << "\n No Strong Component found;";
		XmlWriter wrg;
		wrg.indent();
		wrg.start("StrongComponent");
		int count=0;
		for (auto scc : SCCLlist) {
			wrg.start("SCC" + ToString(count++));
			for (auto sccdata : scc) {
				wrg.start("dependency");
				wrg.addAttribute("filename", sccdata);
				wrg.end();
			}
			wrg.end();			
		}
		wrg.end();
		std::cout<<wrg.xml();
		std::ofstream out("Result.xml");
		out << wrg.xml() << "\n";
	}
	//----< return iterator pointing to first vertex >-------------
	template<typename V, typename E>
	typename Graph<V, E>::iterator Graph<V, E>::begin() { return adj.begin(); }

	//----< return iterator pointing one past last vertex >--------
	template<typename V, typename E>
	typename Graph<V, E>::iterator Graph<V, E>::end() { return adj.end(); }

	//----< index non-const graph's vertex collection >------------
	template<typename V, typename E>
	typename Vertex<V, E>& Graph<V, E>::operator[](size_t i) { return adj[i]; }

	//----< index const graph's vertex collection >----------------
	template<typename V, typename E>
	typename Vertex<V, E> Graph<V, E>::operator[](size_t i) const { return adj[i]; }

	//----< add vertex to graph's vertex collection >--------------
	template<typename V, typename E>
	void Graph<V, E>::addVertex(Vertex<V, E> v)
	{
		adj.push_back(v);
		idMap[v.id()] = adj.size() - 1;
	}
	//----< return number of vertices in graph's collection >------
	template<typename V, typename E>
	size_t Graph<V, E>::size() { return adj.size(); }

	//----< return index of vertex with specified id >-------------
	template<typename V, typename E>
	size_t Graph<V, E>::findVertexIndexById(std::string id)
	{
		return idMap[id];
	}
	template<typename V, typename E>
	Vertex<V, E> Graph<V, E>::getVertexById(std::string idstr) {
		return adj[idMap[idstr]];
	}
	//----< add edge from specified parent to child vertices >-----
	template<typename V, typename E>
	void Graph<V, E>::addEdge(E eVal, Vertex<V, E>& parent, Vertex<V, E>& child)
	{
		size_t childIndex = findVertexIndexById(child.id());
		if (childIndex == adj.size())
			throw std::exception("no edge child");
		size_t parentIndex = findVertexIndexById(parent.id());
		if (parentIndex == adj.size())
			throw std::exception("no edge parent");
		Vertex<V, E>::Edge e;
		e.first = childIndex;
		e.second = eVal;
		adj[parentIndex].add(e);
	}
	//----< recursive depth first search with action f >-----------
	template<typename V, typename E>
	template<typename F>
	void Graph<V, E>::dfsCore(Vertex<V, E>& v, F f)
	{
		f(v);
		v.mark() = true;
		for (auto edge : v)
		{
			if (adj[edge.first].mark() == false)
				dfsCore(adj[edge.first], f);
		}
		for (auto& vert : adj)
		{
			if (vert.mark() == false)
				dfsCore(vert, f);
		}
	}
	//----< depth first search, clears marks for next search >-----
	template<typename V, typename E>
	template<typename F>
	void Graph<V, E>::dfs(Vertex<V, E>& v, F f)
	{
		dfsCore(v, f);
		for (auto& vert : adj)
			vert.mark() = false;
	}
	// trajan's algorithm
	template<typename V, typename E>
	void Graph<V, E>::SCCrec(int u, int disc[], int low[], stack<int> *st, bool stackMember[]) {
		static int time = 0;
		// Initialize discovery time and low value
		disc[u] = low[u] = ++time;
		st->push(u);
		stackMember[u] = true;
		// Go through all vertices adjacent to this
		//list<int>::iterator i;
		Vertex<V, E> vertex = adj[u];
		static vector<string> tempsave;
		iterator2 iter;
		for (iter = vertex.begin(); iter != vertex.end(); ++iter)
		{
			Edge edge = *iter;
			int v = (int)edge.first; 
			if (disc[v] == -1)
			{
				SCCrec(v, disc, low, st, stackMember);
				low[u] = min(low[u], low[v]);
			}
			else if (stackMember[v] == true)
				low[u] = min(low[u], disc[v]);
		}
		int w = 0;  // To store stack extracted vertices

		if (low[u] == disc[u])
		{
			while (st->top() != u)
			{
				w = (int)st->top();
				vertex = adj[w];
				//cout <<"#--"<< vertex.id() << " ";
				tempsave.push_back(vertex.id());
				stackMember[w] = false;
				st->pop();
			}
			w = (int)st->top();
			vertex = adj[w];
			//cout << vertex.id() << "\n";
			tempsave.push_back(vertex.id());
			if (tempsave.size() > 1)
				SCCLlist.push_back(tempsave);
			tempsave.clear();
			stackMember[w] = false;
			st->pop();
		}
	}
	template<typename V, typename E>
	void Graph<V, E>::findSCC() {
		int v = (int)adj.size();
		int *disc = new int[v];
		int *low = new int[v];
		bool *stackMember = new bool[v];
		stack<int> *st = new stack<int>();

		// Initialize disc and low, and stackMember arrays
		for (int i = 0; i < v; i++)
		{
			disc[i] = -1;
			low[i] = -1;
			stackMember[i] = false;
		}

		// Call the recursive helper function to find strongly
		// connected components in DFS tree with vertex 'i'
		for (int i = 0; i < v; i++)
			if (disc[i] == -1)
				SCCrec(i, disc, low, st, stackMember);
	}

	///////////////////////////////////////////////////////////////
	// Display class

	template<typename V, typename E>
	class Display
	{
	public:
		typedef Graph<V, E> graph;
		typedef Vertex<V, E> vertex;
		typedef std::pair<int, E> edge;
		static std::vector< Vertex<V, E> > vertsWithNoParents(graph& g)
		{
			std::vector<size_t> parentCount;
			graph::iterator iter = g.begin();
			while (iter != g.end())
			{
				parentCount.push_back(0);
				++iter;
			}
			iter = g.begin();
			while (iter != g.end())
			{
				vertex v = *iter;
				for (size_t i = 0; i<v.size(); ++i)
				{
					edge e = v[i];
					parentCount[e.first]++;
				}
				++iter;
			}
			std::vector<vertex> noParents;
			for (size_t j = 0; j<g.size(); ++j)
				if (parentCount[j] == 0)
					noParents.push_back(g[j]);
			return noParents;
		}
		static void show(graph& g)
		{
			graph::iterator iter = g.begin();
			while (iter != g.end())
			{
				vertex v = *iter;
				std::cout << "\n  vertex id = " << v.id() << ", value = " << (v.value()).c_str();
				for (size_t i = 0; i<v.size(); ++i)
				{
					vertex::Edge edge = v[i];
					std::cout << "\n    edge points to vertex with id = " << g[edge.first].id();
					std::cout << " and value = " << g[edge.first].value().c_str();
					std::cout << ", edge value = " << (edge.second).c_str();
				}
				++iter;
			}
		}
	};
	//----< convert integer to string >--------------------------
	std::string ToString(int i)
	{
		std::ostringstream in;
		in << i;
		return in.str();
	}
	//----< convert string to integer >--------------------------
	int ToInt(const std::string& str)
	{
		std::istringstream in(str);
		int retn;
		in >> retn;
		return retn;
	}
	//----< construct graph from XML string >--------------------
	template<typename V, typename E>
	void GraphFromXmlString(Graph<V, E>& gr, const std::string& Xml)
	{
		XmlReader vrdr(Xml);// add vertices
		while (vrdr.next())
		{
			if (vrdr.tag() == "vertex")
			{
				XmlReader::attribElems elems = vrdr.attributes();
				if (elems.size() > 1)
				{
					Vertex<V, E> vert(elems[1].second);
					vert.id() = ToInt(elems[0].second);
					gr.addVertex(vert);
				}
				else
				{
					throw std::exception("ill-formed graph XML");
				}
			}
		}
		XmlReader erdr(Xml);// add edges
		int parentId;
		while (erdr.next())
		{
			if (erdr.tag() == "vertex")
			{
				XmlReader::attribElems elems = erdr.attributes();
				if (elems.size() > 0)
				{
					parentId = ToInt(elems[0].second);
				}
				else
				{
					throw std::exception("ill-formed graph XML");
				}
			}
			if (erdr.tag() == "edge")
			{
				XmlReader::attribElems eelems = erdr.attributes();
				if (eelems.size() > 1)
				{
					int childId = ToInt(eelems[0].second);
					E eval = eelems[1].second;
					int parentIndex = gr.findVertexIndexById(parentId);
					Vertex<V, E> parent = gr[parentIndex];
					int childIndex = gr.findVertexIndexById(childId);
					Vertex<V, E> child = gr[childIndex];
					gr.addEdge(eval, parent, child);
				}
				else
				{
					throw std::exception("ill-formed graph XML");
				}
			}
		}
	}
	//----< serialize graph to XML string >----------------------
	template<typename V, typename E>
	std::string GraphToXmlString(Graph<V, E>& gr)
	{
		XmlWriter wrg;
		wrg.indent();
		wrg.start("graph");
		for (auto vert : gr)
		{
			wrg.start("vertex");
			wrg.addAttribute("id", ToString(vert.id()));
			wrg.addAttribute("value", vert.value());
			for (auto edge : vert)
			{
				wrg.start("edge");
				wrg.addAttribute("targetId", ToString(gr[edge.first].id()));
				wrg.addAttribute("value", edge.second);
				wrg.end();
			}
			wrg.end();
		}
		wrg.end();
		return wrg.xml();
	}
}
#endif
