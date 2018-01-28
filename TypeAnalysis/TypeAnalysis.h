#pragma once
///////////////////////////////////////////////////////////////
// TypeAnalysis.h      //
// ver 1.0                                                   //
// SUID 611189677//
// Name: Chie Chih Kao
// email: ckao01@syr.edu
// reference from Prof. Jim Fawcett
// 
//
///////////////////////////////////////////////////////////////
/*
* Package Operations :
*------------------ -
* The Package is to build the typetable and use the token from the Denpendency package
* to do the Depency analysis then use graph package to do the strong component search
*
* Required Files :
*-------------- -
* ActionsAndRules.h
* DepAnal.h
* Tokenizer.h
* NoSqlDb.h
* Display.h
* Persist.h

* function imnplement:
* --------------
*bool doDisplay(ASTNode* pNode) //display the AST
*void TypeAnal::DFS(ASTNode* pNode) //do dfs search for the AST and build the typetable
*void TypeAnal::doTypeAnal() //invoke the DFS to build the TypeTable
*bool bool TypeAnal::saveValue(Key key, TypeData data) //save value in type table
*void TypeAnal::show() { //display the type table
*void TypeAnal::doDepAnal(string filedir) { //do denpendency ananlyze
*TypeAnal::buildDepGraph(NoSqlDb<StrData> db) //Build Dependency graph

*
* Maintenance History:
* --------------------
*vserion 1.0 2017/3/13 Chien Chih Kao
*
*/


#include "../Parser/ActionsAndRules.h"
#include <iostream>
#include <functional>
#include "../DependencyAnanlysis/DepAnal.h"
#include "../Tokenizer/Tokenizer.h"
#include <windows.h>
#include "../NoSqlDb/NoSqlDb.h"
#include "../Display/Display.h"
#include "../Persist/Persist.h"
#include "../Graph/Graph.h"
#include "../HtmlConverter/HtmlConverter.h"

#pragma warning (disable : 4101)
using namespace NoSQLDB;
using namespace GraphLib;
using StrData = std::string;
typedef Graph<std::string, std::string> graph;
typedef Vertex<std::string, std::string> vertex;
typedef GraphLib::Display<std::string, std::string> display;


namespace CodeAnalysis 
{
	using namespace std;
	using Key = string;
	using Keys = vector<Key>;
	class TypeData
	{
	public:
		TypeData():category(""),filename(""),filepath("") {}
		string getcategory() { return category; }
		string getfilname() { return filename; }
		string getfilepath() { return filepath; }
		void set(string cat, string name, string path) {
			category = cat;
			filename = name;
			filepath = path;
		}
	private:
		string category;
		string filename;
		string filepath;
	};

	class TypeAnal
	{
	public:
		using SPtr = std::shared_ptr<ASTNode*>;
		TypeAnal();
		void doTypeAnal();
		void doDepAnal(string filedir,string option);
		bool saveValue(Key, TypeData);
		TypeData value(Key key);
		Keys keys();
		void show();
		void buildDepGraph(NoSqlDb<StrData> db);
		bool hasKey(string key)
		{
			if (TypeTable.find(key) == TypeTable.end())
				return false;
			return true;
		}
		
	private:
		void DFS(ASTNode* pNode);
		AbstrSynTree& ASTref_;
		ScopeStack<ASTNode*> scopeStack_;
		Scanner::Toker& toker_;
		unordered_map <Key, TypeData> TypeTable;
		using Item = std::pair<Key, TypeData>;
	};

	inline TypeAnal::TypeAnal() :
		ASTref_(Repository::getInstance()->AST()),
		scopeStack_(Repository::getInstance()->scopeStack()),
		toker_(*(Repository::getInstance()->Toker()))
	{
		std::function<void()> test = [] { int x; };  // This is here to test detection of lambdas.
	}                                              // It doesn't do anything useful for dep anal.

	inline bool doDisplay(ASTNode* pNode) //display the AST
	{
		static std::string toDisplay[] = {
			"function", "lambda", "class", "struct", "enum", "alias", "typedef"
		};
		for (std::string type : toDisplay)
		{
			if (pNode->type_ == type)
				return true;
		}
		return false;
	}
	inline void TypeAnal::DFS(ASTNode* pNode) //do dfs search for the AST and build the typetable
	{
		TypeData data;
		static std::string path = "";
		if (pNode->path_ != path)
		{
			//std::cout << "\n    -- " << pNode->path_ << "\\" << pNode->package_;
			path = pNode->path_;
		}
		if (doDisplay(pNode)&&pNode->parentType_!="class"&&pNode->name_!="main"&&pNode->parentType_ == "namespace")
		{
			//std::cout << "\n  " << pNode->name_;
			//std::cout << ", " << pNode->type_ << ", " << pNode->parentType_;
			data.set(pNode->type_, pNode->package_, pNode->path_);
			if (!hasKey(pNode->name_))
				saveValue(pNode->name_, data);
			/*if (!saveValue(pNode->name_, data))
				cout << "duplicate key" << endl;*/
			//cout << "\ncategory:" << TypeTable[pNode->name_].category<<", filename"<< TypeTable[pNode->name_].filename;
		}
		for (auto pChild : pNode->children_)
			DFS(pChild);
	}

	inline void TypeAnal::doTypeAnal() //invoke the DFS to build the TypeTable
	{
		//std::cout << "\n  --Req#4 starting type analysis:\n";
		std::cout << "\n  scanning AST and Building the TypeTable:";
		std::cout << "\n -----------------------------------------------";
		ASTNode* pRoot = ASTref_.root();
		DFS(pRoot);
	}
	inline bool TypeAnal::saveValue(Key key, TypeData data) { //save value in type table
		if (TypeTable.find(key) != TypeTable.end())
			return false;
		TypeTable[key] = data;
		return true;
	}
	inline TypeData TypeAnal::value(Key key) //retrieve data in type table
	{
		if (TypeTable.find(key) != TypeTable.end())
			return TypeTable[key];
		return TypeData();
	}
	inline Keys TypeAnal::keys() { //get all the keys in the type table
			Keys keys;
			for (Item item : TypeTable)
			{
				keys.push_back(item.first);
			}
			return keys;
	}
	inline void TypeAnal::show() { //display the type table
		cout <<"\nShow The Type Table, Table size: "<< TypeTable.size()<<"\n";
		//Key key = "putlines";
		for (Key key : keys()) {
			//value(key);
			std::ostringstream out;
			out.setf(std::ios::adjustfield, std::ios::left);
			out << "\n    " << std::setw(8) << "typename" << " : " << key;
			out << "\n    " << std::setw(8) << "category" << " : " << value(key).getcategory();
			out << "\n    " << std::setw(8) << "File Name" << " : " << value(key).getfilname();
			out << "\n    " << std::setw(8) << "File Path" << " : " << value(key).getfilepath();
			out << "\n";
			cout<<out.str();
		}
	}
	void TypeAnal::doDepAnal(string filedir,string option) { //do denpendency ananlyze
		DepAnal dep(filedir+"\\");
		dep.storeAlltoken();
		//cout << "\nSIZE: " << dep.gettokenvec().size();
		TypeData data;
		NoSqlDb<StrData> db;
		Element<StrData> childelem;
		for (tokendata tok : dep.gettokenvec()) {
			//cout << "\ntok :" << tok.tok << ", filename: " << tok.filename;
			data = value(tok.gettok());
			if (hasKey(tok.gettok())&&tok.gettokfilenam() != data.getfilname()) {
				//cout << "\n" << tok.filename << " depend on: " << data.filename;
				Element<StrData> elem;
				if (!db.hasKey(data.getfilname())) {//create element if not exit
					elem.name = data.getfilname();
					elem.data = data.getfilepath();
					db.saveRecord(elem.name, elem);
				}
				if (!db.hasKey(tok.gettokfilenam())) //create element if not exit
				{
					elem.name = tok.gettokfilenam();
					elem.data = tok.getfilepath()+tok.gettokfilenam();
					db.saveRecord(elem.name, elem);
				}
				elem = db.value(tok.gettokfilenam());
				elem.saveChild(data.getfilname()); //add dependency
				db.saveValue(elem.name,elem);
			}
		}
		//cout << "\n--Req#5 Demostrate dependecy in No SQL DB\n";
		/*Display<std::string> display(db);
		display.showAll();*/
		//cout << "\nDB size::"<<db.keys().size();
		HtmlConverter convert;
		for (Key key : db.keys()) {
			convert.toHtml(db.value(key).data,key,db.value(key).children,option);
			//std::cout << "\n  " << key << ":"<< db.value(key).show();
		}
			
		Persist<std::string> persist(db);
		Persist<std::string>::Xml xml = persist.save();
		//Utilities::subTitle("Persisting test db to XML");
		//std::cout << "\n" << xml << "\n";
		buildDepGraph(db);
	}
	void TypeAnal::buildDepGraph(NoSqlDb<StrData> db) //Build Dependency graph
	{
		//cout << "\n\n----Req#5#6 Build Depency Graph to Analyze Strong Component----\n";
		graph g;
		for (Key key : db.keys()) {
			if (!g.hasKey(key)) {
				vertex v(key, key);
				g.addVertex(v);
			}
			for (size_t i = 0; i < db.value(key).children.getValue().size();i++) {
				string childkey = db.value(key).children.getValue()[i];
				if (!g.hasKey(childkey)) {
					vertex childv(childkey, childkey);
					g.addVertex(childv);
				}
				g.addEdge(key + "->" + childkey, g.getVertexById(key),g.getVertexById(childkey));
				//cout << "\n" << key + "->" + childkey << endl;
			}	
		}
		//display::show(g);
		//g.findSCC();
		//g.showSCC();
		//g.SCCtoXML();
	}
}


