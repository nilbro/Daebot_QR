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


class Pattern_Detection {
public:
	Pattern_Detection();
	int show_detection();
};

#endif /* PATTERNDETECTION_H_ */
