/*
 * QRScanner.h
 *
 *  Created on: Feb 24, 2019
 *      Author: bro
 */

#ifndef QRSCANNER_H_
#define QRSCANNER_H_

#include <opencv2/opencv.hpp>
#include <iostream>
#include <cmath>
#include <zbar.h>
#include <mutex>

using namespace std;
using namespace cv;
using namespace zbar;

struct Location{
	Point position;
	String info;
};


class QRScanner
{
public:
    QRScanner();
    std::pair<int,int> find(std::string locationName);
    int scan();


private:
    std::vector<Location> _locations;
    std::mutex _locMtx;
    zbar::ImageScanner _scanner;
};
#endif /* QRSCANNER_H_ */
