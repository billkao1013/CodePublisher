#include "HtmlConverter.h";
int main() {
	HtmlConverter test;
	vector<string> child;
	child.push_back("HtmlConverter.h");
	test.toHtml("HtmlConverter.cpp", "HtmlConverter.cpp",child);
	system("Pause");
}