/*
 * QRScanner.cpp
 *
 *  Created on: Feb 24, 2019
 *      Author: bro
 */

#include "QRScanner.h"


using namespace std;
using namespace cv;
using namespace zbar;

QRScanner::QRScanner() :
    _locations{}
{

    // configure the reader
    _scanner.set_config(zbar::ZBAR_NONE, zbar::ZBAR_CFG_ENABLE, 1);
}

std::pair<int, int> QRScanner::find(std::string locationName)
{
    auto pos = std::make_pair(int(-1),int(-1));
    std::vector<Location>::reverse_iterator revIt;
    _locMtx.lock();
    for (revIt = _locations.rbegin(); revIt != _locations.rend(); revIt++){
    	if (revIt->info == locationName){
    		pos.first=revIt->position.x;
    		pos.second=revIt->position.y;
    		break;
        }
    }
    _locMtx.unlock();
    return pos;
}


int QRScanner::scan(){
	  VideoCapture cap(0);

	  // Check if camera opened successfully
	  if(!cap.isOpened()){
	    cout << "Error opening camera" << endl;
	    return -1;
	  }

	  while(1){

	      Mat frame;
	      // Capture frame-by-frame
	      cap >> frame;
	      //Convert to grey-scale
	      Mat grey;
	      cvtColor(frame,grey,CV_BGR2GRAY);

	      //convert to zbar image
	      Image zimage(frame.cols, frame.rows, "Y800", (uchar *)grey.data, frame.cols * frame.rows);

	      //scan and decode the converted image
	      _scanner.scan(zimage);
	      //_locMtx.lock();
	      _locations.clear();
	      for(Image::SymbolIterator symbol = zimage.symbol_begin();symbol != zimage.symbol_end();++symbol){
	    	  vector<Point> vp;
	    	  Location location;
	    	  location.info = symbol->get_data();
	    	  for(int i = 0; i< symbol->get_location_size(); i++)
	    	      	    {
	    	      			vp.push_back(Point(symbol->get_location_x(i),symbol->get_location_y(i)));
	    	      			location.position.x = symbol->get_location_x(i);
	    	      			location.position.y = symbol->get_location_y(i);

	    	      	    }
	    	  _locations.push_back(Location{location});



	    	  RotatedRect r = minAreaRect(vp);
	    	      	Point2f pts[4];
	    	      	       r.points(pts);
	    	      	       string text = to_string(location.position.x) + "," + to_string(location.position.y);
	    	      	       for(int i=0;i<4;i++){
	    	      	         line(frame,pts[i],pts[(i+1)%4],Scalar(255,0,0),3);
	    	      	         putText(frame, text, cvPoint(location.position.x,location.position.y),
	    	      	             FONT_HERSHEY_SIMPLEX, 0.8, cvScalar(255,0,0),1, CV_AA);

	    	      	       }
	    	      }

	      imshow( "Frame", frame );

	         // Press  ESC on keyboard to exit
	         char c=(char)waitKey(25);
	         if(c==27)
	           break;
	      }
	  return 0;
}




