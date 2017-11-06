

#include <iostream>
#include <fstream>
#include <string.h>

#include "AtHomeConfig.h"
#include "DataBase_Reader.h"


using namespace std;
using namespace cv;

int xini = -1;
int yini, xfin, yfin;
vector<pair<Point, int> > P;
vector<Point> op;
Mat img;
void CallBackFunc(int event, int x, int y, int flags, void* userdata)
{

	if (event == EVENT_LBUTTONDOWN)
	{
		if (P.size() == 6) {
			P.clear();
			op.clear();
		}
		int Z;
		cout << "Ps Gay Gordo - position (" << x << ", " << y << ")" << endl;
		cout << " Z = "; cin >> Z;
		P.push_back(pair<Point, int>(Point(x-320, y-240), Z));
		op.push_back(Point(x, y));
		//xfin = x;  yfin = y;
		/*if (xini >= 0)
		{
			line(img, Point(xini, yini), Point(xfin, yfin), Scalar(0, 0, 255), 2, 8);
		}
		xini = xfin; yini = yfin;*/
		//imshow("Mouse", img);
	}


	else if (event == EVENT_RBUTTONDOWN)
	{
		P.clear();
	}
	/*
	else if (event == EVENT_MBUTTONDOWN)
	{
	cout << "Middle button of the mouse is clicked - position (" << x << ", " << y << ")" << endl;
	}
	else if (event == EVENT_MOUSEMOVE)
	{
	cout << "Mouse move over the window - position (" << x << ", " << y << ")" << endl;
	}*/

}


int main()
{
  
  VideoCapture cap(1); // open the default camera
  if (!cap.isOpened())  // check if we succeeded
	 return -1;

  Mat edges; int f = 0;
  namedWindow("edges",1);
  setMouseCallback("edges", CallBackFunc, NULL);
  for (;;)
  {
	  if (P.size() == 6) {
		  int A[12][12];
		  int a=0;
		  for (int i = 0; i < 6; i++) {
			  A[a][0] = P[i].first.x;
			  A[a][1] = P[i].first.y;
			  A[a][2] = P[i].second;
			  A[a][3] = 1;
			  A[a][4] = 0;
			  A[a][5] = 0;
			  A[a][6] = 0;
			  A[a][7] = 0;
			  A[a][8] = -(P[i].first.x*P[i].first.x);
			  A[a][9] = -(P[i].first.x*P[i].first.y);
			  A[a][10] = -(P[i].first.x*P[i].second);
			  A[a][11] = -P[i].first.x;
			  A[a+1][0] = 0;
			  A[a+1][1] = 0;
			  A[a+1][2] = 0;
			  A[a+1][3] = 0;
			  A[a+1][4] = P[i].first.x;
			  A[a+1][5] = P[i].first.y;
			  A[a+1][6] = P[i].second;
			  A[a+1][7] = 1;
			  A[a+1][8] = -(P[i].first.y*P[i].first.x);
			  A[a+1][9] = -(P[i].first.y*P[i].first.y);
			  A[a+1][10] = -(P[i].first.y*P[i].second);
			  A[a+1][11] = -P[i].first.x;
			  
			  
			  a += 2;
			 
		  }
		  cout << "a11 a12 a13 a14 a21 a22 a23 a24 a41 a42 a43 a44" << endl;
		  for (int i = 0; i < 12; i++) {
			  for (int j = 0; j < 12; j++) {
				  cout << " " << A[i][j];
			  }
			  cout << endl;
		  }
		  system("PAUSE");
	  }

	  cap >> img; // get a new frame from camera
	  //cvtColor(frame, edges, CV_BGR2GRAY);
	  //GaussianBlur(edges, edges, Size(7, 7), 1.5, 1.5);
	  //Canny(edges, edges, 0, 30, 3);
	  for (int i = 0; i<P.size(); i++)
		  circle(img, Point(op[i].x, op[i].y), 2.0, CvScalar(0, 0, 255), 2, 8);
	  imshow("edges", img);
	  //if (waitKey(30) >= 0) break;
	  waitKey(1);
	  
	  string path = "";
	  string tipo = ".png";

	  path.append(VIDEO);
	  f++;
	  path = path + std::to_string(f) + tipo;
	  //path.append(tipo);

	  //imwrite(path.data(), frame);

	  char c = waitKey(1);
	  if (c == 27) exit(0);
  }

  return 0;
}