

#ifndef PSRSEGMENTATION
#define PSRSEGMENTATION


#include "AtHomeConfig.h"
#include "psrSegmentation.h"

#include "Time.h"
#include "math.h"

using namespace cv;

void psrCutHSVsegmentation(Mat hsv1, Mat hsv, double cut);
std::vector<Mat> SplitRegions(Mat im, int flag, double precision);



void psrCutHSVsegmentation(Mat img, Mat hsv_cut, double cut)
{
	int i,x,y;

	/////  PRIMEIRA CARACTERISTICA 
	// EXTRAI CARACTERISICAS DE COR - HISTOGRAMA HSV162
	double HSV_hist[DIMENSAO_COR];
	double HSV_cut_hist[DIMENSAO_COR];

	for (i = 0; i < DIMENSAO_COR; i++)  HSV_hist[i] = 0.0;
	for (i = 0; i < DIMENSAO_COR; i++)  HSV_cut_hist[i] = 0.0;

	/////  PRIMEIRA CARACTERISTICA 
	// EXTRAI CARACTERISICAS DE COR - HISTOGRAMA HSV162
	getHSVSplitHistogram1(img, HSV_hist);

	int total_pixels = img.rows * img.cols;

	for (i = 0; i < DIMENSAO_COR; i++)
	{
		double intensity_whole = HSV_hist[i]; // / (double)total_pixels;

		//cout << i << " - " << HSV_hist[i];
		if (intensity_whole >= cut)
			HSV_cut_hist[i] = intensity_whole;
		else
			HSV_cut_hist[i] = 0;
		
		//cout << " n " << HSV_cut_hist[i] << endl;
	}
	

	int row1 = (int)(img.rows / 3);

	Mat hsv;
	cvtColor(img, hsv, CV_BGR2HSV);
	
	
	//Back no primeiro terço
	Vec3b intensity;
	for (y = 0; y < row1; y++)
		for (x = 0; x < hsv.cols; x++)
		{
			intensity = hsv.at<Vec3b>(y, x);

			int Hn = floor((((int)intensity.val[0] / 179.0)*(Hbin - 1)) + 0.5);
			int Sn = floor((((int)intensity.val[1] / 255.0)*(Sbin - 1)) + 0.5);
			int Vn = floor((((int)intensity.val[2] / 255.0)*(Vbin - 1)) + 0.5);

			int indx = (Sbin*Vbin) * Hn + Vbin * Sn + Vn;

			if (!HSV_cut_hist[indx]) hsv_cut.at<uchar>(y, x) = (uchar)(255);
		}
   
   
	for (y = row1; y < 2*row1; y++)
		for (x = 0; x < hsv.cols; x++)
		{
			intensity = hsv.at<Vec3b>(y, x);

			int Hn = floor((((int)intensity.val[0] / 179.0)*(Hbin - 1)) + 0.5);
			int Sn = floor((((int)intensity.val[1] / 255.0)*(Sbin - 1)) + 0.5);
			int Vn = floor((((int)intensity.val[2] / 255.0)*(Vbin - 1)) + 0.5);

			int indx = (int)(DIMENSAO_COR/3) + (Sbin*Vbin) * Hn + Vbin * Sn + Vn;

			if (!HSV_cut_hist[indx]) hsv_cut.at<uchar>(y, x) = (uchar)(255);
		}

	
	for (y = 2*row1; y < 3*row1; y++)
		for (x = 0; x < hsv.cols; x++)
		{
			intensity = hsv.at<Vec3b>(y, x);

			int Hn = floor((((int)intensity.val[0] / 179.0)*(Hbin - 1)) + 0.5);
			int Sn = floor((((int)intensity.val[1] / 255.0)*(Sbin - 1)) + 0.5);
			int Vn = floor((((int)intensity.val[2] / 255.0)*(Vbin - 1)) + 0.5);

			int indx = (int)(2 * DIMENSAO_COR / 3) + (Sbin*Vbin) * Hn + Vbin * Sn + Vn;

			if (!HSV_cut_hist[indx]) hsv_cut.at<uchar>(y, x) = (uchar)(255);
		}
     
}




std::vector<Mat> SplitRegions(Mat im, int flag, double precision)
{
	Mat imroi(im.size(), CV_8UC1);
	int nLabels = connectedComponents(im, imroi, 8);

	cout << "Analisando " << nLabels << " regioes candidatas ..." << endl;

	std::vector<Mat> Regioes;
	std::vector<Mat> ImRegions;
	std::vector<int> NumPixelsRegions;


	for (int i = 0; i < nLabels; i++)
	{
		Mat reg(im.rows, im.cols, CV_8UC1, Scalar(0));
		ImRegions.push_back(reg);
		NumPixelsRegions.push_back(0);
	}


	for (int y = 0; y < imroi.rows; ++y) {
		for (int x = 0; x < imroi.cols; ++x) {
		  int label = (int)imroi.at<int>(y, x);
		  ImRegions[label].at<uchar>(y, x) = 255;
		  NumPixelsRegions[label] = NumPixelsRegions[label] + 1;
		}
	}
	 

	for (int r = 0; r < nLabels; r++){
	   double pnpixels = (double)NumPixelsRegions[r]/ (im.rows * im.cols);
	   if (pnpixels >= precision) {
		   Regioes.push_back(ImRegions[r]);
	   }
	}

	cout << "Achou " << Regioes.size() << " regioes candidatas" << endl;


	return Regioes;
}



#endif