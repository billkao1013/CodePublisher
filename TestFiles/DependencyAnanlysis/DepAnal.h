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
* The goal of this package is to store all the wanted tolken of the file in the directory
* to let the TypeAnalysis package to do the analysis
*
* Required Files :
*-------------- -
* Tokenizer.h

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

#include <iostream>
#include <locale>
#include <string>
#include <vector>
#include <fstream> 
#include "../Tokenizer/Tokenizer.h"
#include <windows.h>
//#include "../Tokenizer/Tokenizer.cpp"
using namespace Scanner;
using namespace std;
class tokendata
{
public:
	tokendata(string tokstr, string filenamestr,string filepathstr) {
		filename = filenamestr;
		tok = tokstr;
		filepath = filepathstr;
	}
	string gettokfilenam() { return filename; }
	string gettok() { return tok; }
	string getfilepath() { return filepath; }
private:
	std::string filename;
	std::string tok;
	std::string filepath;
};

class DepAnal
{
public:
	void storeAlltoken();
	DepAnal(string filepath);
	void collectAllFiles(string folder);
	vector<tokendata> gettokenvec() { return tokvec; }
private:
	void getfile(string exten);
	string filedir_;
	struct fileinfo
	{
		string filepath_;
		string filename_;
	};
	vector<fileinfo> filelist_;
	vector<tokendata> tokvec;
	
	
};
DepAnal::DepAnal(string filedir) { //initiate directory
	filedir_ = filedir;
}
void DepAnal::storeAlltoken() {
	std::string fileSpec;
	collectAllFiles(filedir_);
	cout<<"\nfile anoumt::"<<filelist_.size()<<endl;
	for (auto file : filelist_) {
		fileSpec = file.filepath_ + file.filename_;
		try
		{
			std::ifstream in(fileSpec);
			if (!in.good())
			{
				std::cout << "\n  can't open " << fileSpec << "\n\n";
			}
			else
			{
				Toker toker;
				toker.returnComments();
				toker.attach(&in);
				do
				{
					std::string tok = toker.getTok();
					if (tok != "\n"&& tok.find("//") == std::string::npos&& tok.find("/*") == std::string::npos) {
						//cout << "\ntok-- "<<tok;
						tokendata tokdata(tok, file.filename_,file.filepath_);
						tokvec.push_back(tokdata);
					}	
				} while (in.good());
			}
		}
		catch (std::logic_error& ex)
		{
			std::cout << "\n  " << ex.what();
		}
	}
}
void DepAnal::collectAllFiles(string folder)
{
	fileinfo fileinfo;// This is where I will hold file details temporarily
	WIN32_FIND_DATA file;// This will store return value of the FindFirstFile()
	HANDLE fileHandle;// Give wildcard like e:\someFolder\*
	char wildCard[1024];
	sprintf_s(wildCard, "%s\\*", folder.c_str());
	fileHandle = FindFirstFile(wildCard, &file);
	do
	{
		string filename = file.cFileName;
		if ((file.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0)
		{
			if (filename != "."&& filename != "..")
			{
				collectAllFiles(folder + filename+"/");

			}
		}
		else  // It is a file
		{
			// Store a file
			if (filename.find(".h") != std::string::npos || filename.find(".cpp") != std::string::npos || filename.find(".cs") != std::string::npos)
			{
				fileinfo.filename_ = filename;
				fileinfo.filepath_ = folder;
				filelist_.push_back(fileinfo);
				//cout <<"#####"<< folder << filename << "\n ";
			}
		}
		// Go on and find next file(s) in current dir
	} while (FindNextFile(fileHandle, &file));
}



