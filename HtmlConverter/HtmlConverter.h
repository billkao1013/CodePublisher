#pragma once
///////////////////////////////////////////////////////////////
// HtmlConverter.h      //
// ver 1.0                                                   //
// SUID 611189677//
// Name: Chie Chih Kao
// email: ckao01@syr.edu
///////////////////////////////////////////////////////////////
/*
* Description:
* -------------------
* the file is to convert the code file to html file
*
* Funtion :
* -------------------
* 
* toHtml(): convert code file to html file
*/
#include <iostream>
#include <fstream>
#include <cstring>
#include <string>
#include <cstdlib>
#include <vector>

using namespace std;

class HtmlConverter
{
public:
	HtmlConverter();
	~HtmlConverter();
	bool toHtml(string fullpath, string filename, vector<string>childs,string option);
private:
	string styleset;
	string javascript;
};

HtmlConverter::HtmlConverter()
{
	
	styleset = "<link rel=\"stylesheet\" type=\"text/css\" href=\"style.css\">\n";
	javascript = "<script type=\"text/javascript\" src=\"jsuse.js\"></script>";
}

HtmlConverter::~HtmlConverter()
{
	cout << "Converting ends\n";
}
bool HtmlConverter::toHtml(string fullpath, string filename, vector<string>childs,string option) {
	ifstream inStream;
	string dependFile;
	if (option == "1"&&filename.find(".cpp") != string::npos)
		return false;
	if (option == "2"&&filename.find(".h") != string::npos)
		return false;
	inStream.open(fullpath.c_str());
	if (inStream.fail()) {
		cout << "IO cannot find the file\n";
		return false;
	}
	string outputfile = "..\\Result\\" + filename + ".html";
	ofstream outStream;
	//dependFile = outputfile;
	outStream.open(outputfile.c_str());
	outStream << "<html>\n\t<head>\n" << styleset<<javascript << "\t</head>\n";
	outStream << "<body>\n\t<h4>" << filename << "</h4>\n";
	outStream << "<hr/><div class=\"indent\">\t\n\t<h4>Dependencies</h4>\n"; 
	for (string child : childs) {
		outStream << "\t<a href=\"" << child+".html" + "\">" << child << "</a>\n";
	}
	outStream<<"</div>\t<hr/>\n";
	outStream << "<PRE>" << endl;
	char c;
	int i=0;
	string id;
	while (!inStream.eof()) {
		inStream.get(c);    
		if (c == '{') {
			id="id"+ to_string(i);
			outStream << "<button type=\"button\" sytle= \"display:inline\" onclick=\"showHide('"<<id<<"');\">+</button>";
			outStream << "<div id=\"" << id << "\" style=\"\">";
			i++;}
		if (c == '}')
			outStream << "</div>";
		if (c == '<')
			outStream << "&lt;";
		else if (c == '>') 
			outStream << "&gt;";
		else outStream << c;}
	// Output end /PRE tag
	outStream << "</PRE>" << endl;
	outStream << "</body>\n</html>";
	inStream.close();
	outStream.close();
	cout << filename+" Converstion done\n";
	return true;
}
