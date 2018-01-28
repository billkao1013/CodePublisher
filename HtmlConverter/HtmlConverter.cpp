/////////////////////////////////////////////////////////////////
// HtmlConverter.h      //
// ver 1.0                                                   //
// SUID 611189677//
// Name: Chie Chih Kao
// email: ckao01@syr.edu
///////////////////////////////////////////////////////////////
//Description:
//test the Htmlconverter tohtml fuction
#include "HtmlConverter.h"
int main() {
	HtmlConverter test;
	vector<string> child;
	child.push_back("HtmlConverter.h");
	test.toHtml("HtmlConverter.cpp", "HtmlConverter.cpp",child,"3");
	system("Pause");
	return 0;
}