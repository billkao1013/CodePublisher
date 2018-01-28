#pragma once
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
	bool toHtml(string fullpath, string filename, vector<string>childs);
private:
	string styleset;
};

HtmlConverter::HtmlConverter()
{
	styleset = " <style>\n\
		body{\n\
	margin: 20px;\n\
	color: black;\n\
		background - color: #eee;\n\
		font-family: Consolas;\n\
		font-weight: 600;\n\
		font-size: 110% ;\n\
	}\n\
		.indent{\n\
		margin - left: 20px;\n\
	margin - right: 20px;\n\
	}\n\
		h4{\n\
		margin - bottom: 3px;\n\
	margin - top: 3px;\n\
	}</style>\n";
}

HtmlConverter::~HtmlConverter()
{
	cout << "Converting ends\n";
}
bool HtmlConverter::toHtml(string fullpath, string filename, vector<string>childs) {
	ifstream inStream;
	string dependFile;
	inStream.open(fullpath.c_str());
	if (inStream.fail()) {
		cout << "IO cannot find the file\n";
		return false;
	}
	string outputfile = "..\\Result\\" + filename + ".html";
	ofstream outStream;
	//dependFile = outputfile;
	outStream.open(outputfile.c_str());
	outStream << "<html>\n\t<head>\n" << styleset << "\t</head>\n";
	outStream << "<body>\n\t<h4>" << filename << "</h4>\n";
	outStream << "<hr/><div class=\"indent\">\t\n\t<h4>Dependencies</h4>\n"; 
	for (string child : childs) {
		outStream << "\t<a href=\"" << child+".html" + "\">" << child << "</a>\n";

	}
	outStream<<"</div>\t<hr/>\n";
	outStream << "<PRE>" << endl;
	char c;
	while (!inStream.eof()) {
		inStream.get(c);    // Get one character
							// Output &lt; or &gt; or original char
		if (c == '<') {
			outStream << "&lt;";
		}
		else if (c == '>') {
			outStream << "&gt;";
		}
		else outStream << c;
	}
	// Output end /PRE tag
	outStream << "</PRE>" << endl;
	outStream << "</body>\n</html>";
	inStream.close();
	outStream.close();
	cout << "Converstion done\n";
}
