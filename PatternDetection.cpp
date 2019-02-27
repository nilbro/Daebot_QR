/*
 * PatternDetection.cpp
 *
 *  Created on: Feb 26, 2019
 *      Author: bro
 */
#include <opencv2/opencv.hpp>
#include <iostream>
#include <cmath>
#include <zbar.h>
#include <mutex>
#include "PatternDetection.h"
#include "QRScanner.h"


using namespace std;
using namespace cv;
using namespace zbar;

Pattern_Detection::Pattern_Detection()
{}

int Pattern_Detection::show_detection(){

	  VideoCapture cap(0);

	  // Check if camera opened successfully
	  if(!cap.isOpened()){
	    cout << "Error opening camera" << endl;
	    return -1;
	  }

	  while(1){

		Mat grey_frame, pattern, result, frame;
		//grey_frame = imread("/home/bro/eclipse-workspace/Daebot_QR/frame5.jpeg", IMREAD_GRAYSCALE);
		cap >> frame;
		cvtColor(frame, grey_frame, CV_BGR2GRAY);
		Mat frame1;
		//grey_frame.copyTo(frame1);
		//imshow( "frame", fTM_CCORRrame );
		//frame1.Canny();

		Image zimage(frame.cols, frame.rows, "Y800", (uchar *)grey_frame.data, frame.cols * frame.rows);

		zbar::ImageScanner scanner1;
		scanner1.set_config(zbar::ZBAR_NONE, zbar::ZBAR_CFG_ENABLE, 1);
		int decodeStatus = scanner1.scan(zimage);
		if(decodeStatus==1){

			//TODO: QR Decode
		}
		else {
			//convert frames to canny frames
		Canny( grey_frame, frame1, 50, 100, 3);
		pattern = imread("/home/bro/eclipse-workspace/Daebot_QR/template_small.jpg", IMREAD_GRAYSCALE);

		//convert template to canny form
		Canny(pattern, pattern, 50,100,3);

		//initilization of resultant frame
		int result_cols =  frame1.cols - pattern.cols + 1;
		int result_rows = frame1.rows - pattern.rows + 1;
		result.create( result_rows, result_cols, CV_32FC1 );

		//match Template and normalize
		matchTemplate(frame1,pattern,result, CV_TM_CCORR);
		normalize( result, result, 0, 1, NORM_MINMAX, -1, Mat() );

		//localize max and min values in the resultant frame
		double minVal; double maxVal; Point minLoc; Point maxLoc;
		Point matchLoc;
		minMaxLoc( result, &minVal, &maxVal, &minLoc, &maxLoc, Mat() );
		/*
		string match_method = CV_TM_CCORR;

		if( match_method  == TM_SQDIFF || match_method == TM_SQDIFF_NORMED )
			{ matchLoc = minLoc; }
		else
			{ matchLoc = maxLoc; }
	*/
		matchLoc = maxLoc;

		//draw bounding rectangle
		rectangle( frame, matchLoc, Point( matchLoc.x + pattern.cols , matchLoc.y + pattern.rows ), Scalar(255,0,255), 2, 8, 0 );
		string text = to_string(matchLoc.x + pattern.cols) + "," + to_string(matchLoc.y + pattern.rows);
		putText(frame, text, cvPoint(matchLoc.x + pattern.cols,matchLoc.y + pattern.rows),
			    	      	             FONT_HERSHEY_SIMPLEX, 0.8, cvScalar(255,0,0),1, CV_AA);


		Location location;
		location.position = Point(matchLoc.x,matchLoc.y);
		location.info = "Not close enough";

		imshow( "frame", frame );
		char c=(char)waitKey(25);
		if(c==27)
			break;
		}

}
	  return 0;
}



