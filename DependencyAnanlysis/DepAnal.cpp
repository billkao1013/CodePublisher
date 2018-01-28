#pragma once


#include "DepAnal.h"

int main(){
	string filedir="../TestFiles";
	DepAnal dep(filedir);
	dep.storeAlltoken();
	cout << "SIZE: " << dep.gettokenvec().size();
	for (tokendata tok : dep.gettokenvec()) {
		cout << "\ntok :" << tok.gettok() << ", filename: " << tok.gettokfilenam();
	}
	
	system("pause");
}

