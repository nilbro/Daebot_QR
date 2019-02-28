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


using namespace std;
using namespace cv;
using namespace zbar;

Pattern_Detection::Pattern_Detection():
  _locations {} {
    _scanner.set_config(zbar::ZBAR_NONE, zbar::ZBAR_CFG_ENABLE, 1);

  }

std::pair < int, int > Pattern_Detection::find(std::string locationName) {
  auto pos = std::make_pair(int(-1), int(-1));
  std::vector < Location > ::reverse_iterator revIt;
  _locMtx.lock();
  for (revIt = _locations.rbegin(); revIt != _locations.rend(); revIt++) {
    if (revIt->info == locationName) {
      pos.first = revIt->position.x;
      pos.second = revIt->position.y;
      break;
    }
  }
  _locMtx.unlock();
  return pos;
}

int Pattern_Detection::show_detection() {

  VideoCapture cap(0);

  // Check if camera opened successfully
  if (!cap.isOpened()) {
    cout << "Error opening camera" << endl;
    return -1;
  }

  while (1) {

    Mat grey_frame, pattern, result, frame,frame1;
    cap >> frame;
    cvtColor(frame, grey_frame, CV_BGR2GRAY);
    Image zimage(frame.cols, frame.rows, "Y800", (uchar * ) grey_frame.data, frame.cols * frame.rows);
    int decodeStatus = _scanner.scan(zimage);
    _locations.clear();

    //check if qr is decodable
    if (decodeStatus == 1) {

      for (Image::SymbolIterator symbol = zimage.symbol_begin(); symbol != zimage.symbol_end(); ++symbol) {
        vector < Point > vp;
        Location location;
        location.info = symbol->get_data();
        for (int i = 0; i < symbol->get_location_size(); i++) {
          vp.push_back(Point(symbol-> get_location_x(i), symbol-> get_location_y(i)));
          location.position.x = symbol-> get_location_x(i) / 2;
          location.position.y = symbol-> get_location_y(i) / 2;

        }
        _locations.push_back(Location {
          location
        });
        RotatedRect r = minAreaRect(vp);
        Point2f pts[4];
        r.points(pts);
        string text = to_string(location.position.x) + "," + to_string(location.position.y);
        for (int i = 0; i < 4; i++) {
          line(frame, pts[i], pts[(i + 1) % 4], Scalar(255, 0, 0), 3);
          putText(frame, text, cvPoint(location.position.x, location.position.y),
            FONT_HERSHEY_SIMPLEX, 0.8, cvScalar(255, 0, 0), 1, CV_AA);

        }
      }
    } else {

      //convert frames to canny frames
      Canny(grey_frame, frame1, 50, 100, 3);
      pattern = imread("/home/bro/eclipse-workspace/Daebot_QR/home_qr_small.jpg", IMREAD_GRAYSCALE);

      //convert template to canny form
      Canny(pattern, pattern, 50, 100, 3);

      //Initialization of resultant frame for template matching
      int result_cols = frame1.cols - pattern.cols + 1;
      int result_rows = frame1.rows - pattern.rows + 1;
      result.create(result_rows, result_cols, CV_32FC1);

      //match Template and normalize
      matchTemplate(frame1, pattern, result, CV_TM_CCORR);
      normalize(result, result, 0, 1, NORM_MINMAX, -1, Mat());

      //localize max and min values in the resultant frame
      double minVal;
      double maxVal;
      Point minLoc;
      Point maxLoc;
      Point matchLoc;
      minMaxLoc(result, & minVal, & maxVal, & minLoc, & maxLoc, Mat());

      matchLoc = maxLoc;

      //draw bounding box
      rectangle(frame, matchLoc, Point(matchLoc.x + pattern.cols, matchLoc.y + pattern.rows), Scalar(255, 0, 255), 2, 8, 0);
      string text = to_string(matchLoc.x + pattern.cols) + "," + to_string(matchLoc.y + pattern.rows);
      putText(frame, text, cvPoint(matchLoc.x + pattern.cols / 2, matchLoc.y + pattern.rows / 2),
        FONT_HERSHEY_SIMPLEX, 0.8, cvScalar(255, 0, 0), 1, CV_AA);

      //update structure
      Location location;
      location.position = Point(matchLoc.x + pattern.cols / 2, matchLoc.y + pattern.rows / 2);
      location.info = "Not close enough";

      //push location in
      _locations.push_back(location);

    }

    imshow("frame", frame);
    char c = (char) waitKey(1);
    if (c == 27)
      break;
  }

  return 0;
}
