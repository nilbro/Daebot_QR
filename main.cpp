#include <iostream>
#include "PatternDetection.h"



int main(){

/*
	QRScanner qr;
	qr.scan();
	pair<int,int> position = qr.find("Home");
	cout << "x: " <<position.first<<" y: "<<position.second;
*/
	Pattern_Detection p;
	p.show_detection();
	pair<int,int> position = p.find("Home");
	cout << "x: " <<position.first<<" y: "<<position.second;


}
