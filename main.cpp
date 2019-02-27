#include <iostream>
#include "QRScanner.h"
#include "PatternDetection.h"

int main(){
	/*
	QRScanner qr;
	qr.scan();
	pair<int,int> position = qr.find("http://WWW.TP-LINK.COM/en/products/details/?model=TL-WR802N");
	cout << "x: " <<position.first<<" y: "<<position.second;
*/
	Pattern_Detection p;
	p.show_detection();

}
