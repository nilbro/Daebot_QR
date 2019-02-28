/*
 * PatternDetection.h
 *
 *  Created on: Feb 26, 2019
 *      Author: bro
 */

#ifndef PATTERNDETECTION_H_
#define PATTERNDETECTION_H_

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


class Pattern_Detection {
public:
	Pattern_Detection();
	std::pair<int,int> find(std::string locationName);
	int show_detection();

private:
    std::vector<Location> _locations;
    std::mutex _locMtx;
    zbar::ImageScanner _scanner;
};


#endif /* PATTERNDETECTION_H_ */
