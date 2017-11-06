

#include <iostream>
#include <fstream>
#include <string.h>

#include "AtHomeConfig.h"
#include "DataBase_Reader.h"
#include "Firefly_Optimization.h"
#include "multilimiarizacao.h"

using namespace std;
using namespace cv;

void check_inside(std::vector <FoundObject> &TPList);
void mathTemplateColor(Mat src, Mat tmpl, double rat, double* maxVal, Point* maxLoc, Mat mask);
void mathTemplateEdge(Mat src, Mat tmpl, double rat, double* maxVal, Point* maxLoc, Mat mask);
void showFinalResult(Mat scene, std::vector <FoundObject>  LisOfFounds);
FoundObject Individual_evolution(Mat scene, Mat tmpl, Mat sub_tmpl, std::vector  <SceneLoc> SL, int ind);
std::vector <Mat> EdgeScharr(Mat src1, Mat tmpl1, double rat);
std::vector <Mat> Colorfeatures(Mat src, Mat tmpl, double rat);
Mat find_general_roi(Mat src, std::vector <imageandindx> TableOfObjectsCrops);
std::vector<imageandindx> getExpandCropRegions(Mat img, std::vector<Mat> ConectedRegions, int Dx, int Dy);
std::vector<imageandindx> object_recognition_first_evaluation(Mat img);
std::vector<Mat> SplitRegions(Mat im, int flag, double precision);
std::vector <std::vector <Mat>> loadtemplatepartslist(string mainpathparts);
std::vector <std::vector <Mat>> loadtemplatelist(string maintmpl);
std::vector <FoundObject> Remainder_Evolution(Mat scn0, std::vector <FoundObject> TPList, std::vector <std::vector <Mat>> TemplateList, std::vector <std::vector <Mat>> TemplatePartsList);
std::vector <Rect> buscalinhas(Mat img);
Mat makemask(Mat scn1);
Mat buildmask(Mat mask1);




void in_video_recognition(Mat im0, std::vector <FoundObject>  TPlist);



void in_video_recognition(Mat im0, std::vector <FoundObject>  TPlist)
{

	string filename = "C:\\Users\\Paulo\\Dropbox\\PROJETOS\\Judite\\Base-de-Dados\\video_prateleira\\2017-03-20-180346.WEBM";
	VideoCapture capture(filename);
	Mat frame;

	if (!capture.isOpened())
		throw "Error when reading steam_avi";

	int tot = 0;
	for (int i = 0; i < 400; i++)
	{

		if (i < 50)
		{
			capture >> frame;
			int newcol = 1030;
			int newlin = 579;
			cv::resize(frame, frame, cv::Size(newcol, newlin), 0, 0, CV_INTER_LINEAR);
			imshow("w", frame);
			waitKey(1); // waits to display frame
			tot = tot + 1;
			//imwrite("video.png", frame);
			//cout << i << endl;
		}
		else
		{
			capture >> frame;
			imshow("w", frame);
			int newcol = 1030;
			int newlin = 579;
			cv::resize(frame, frame, cv::Size(newcol, newlin), 0, 0, CV_INTER_LINEAR);
		 

		
		}

	}
	cout << tot << endl;
	waitKey(0);

}




Mat makemask(Mat scn1)
{

	Mat scn2;
	scn1.copyTo(scn2);
	Mat dst(scn2.rows, scn2.cols, CV_8U, Scalar(0));

	cvtColor(scn2, scn2, CV_BGR2GRAY);

	for (int y = 0; y < scn2.rows; y++)
		for (int x = 0; x < scn2.cols; x++)
		{
			if ((int)scn2.at<uchar>(y, x))
			   dst.at<uchar>(y, x) = 0;
			else
			   dst.at<uchar>(y, x) = 255;
		}

	return dst;
}




Mat buildmask(Mat mask1)
{
	Mat mask2(mask1.rows, mask1.cols, CV_8U, Scalar(0));

	std::vector<Rect> RECTS = buscalinhas(mask1);

	//cvtColor(mask1, mask1, CV_BGR2GRAY);

	for (int r = 0; r < RECTS.size(); r++)
	{
	  for (int y = 0; y < mask1.rows; y++)
	   for (int x = 0; x < mask1.cols; x++)
	   {
		 if ((x >= RECTS[r].x*0.5) && (x <= RECTS[r].x*0.5 + RECTS[r].width*2.0) && (y >= RECTS[r].y*0.5) && (y <= RECTS[r].y*0.5 + RECTS[r].height*2.0))
		 {
			 mask2.at<uchar>(y, x) = 255;
			 if ((int)mask1.at<uchar>(y, x))
				mask2.at<uchar>(y, x) = 0;
		 }
	   }
	}

	return mask2;
}


std::vector <Rect> buscalinhas(Mat img)
{
	std::vector <Rect> RECTS;

	int D = 10;

	//cvtColor(img, img, CV_BGR2GRAY);


	// procura primeira linha estrategica
	int y1 = 0, x1 = 0;
	int L1 = 0;
	while ((y1 < img.rows) && (!L1))
	{
		x1 = 0;
		while ((x1 < img.cols) && (!L1))
		{
			L1 = (int)img.at<uchar>(y1, x1);
			x1++;
		}
		y1++;
	}
	y1--;
	x1--;

	//procura a segunda linha estrategica
	int y2 = y1; int x2 = 0;
	int L2 = (int)img.at<uchar>(y1, x1);
	while ((y2 < img.rows-1) && (L2))
	{
		x2 = 0;
		do
		{
			L2 = (int)img.at<uchar>(y2, x2);
			x2++;
		}while((x2 < img.cols - 1) && (!L2));
		y2++;
	}
	y2--;
	x2--;

	//procura a terceira linha estrategica
	int y3 = y2; int x3 = 0;
	int L3 = (int)img.at<uchar>(y3, x3);
	while ((y3 < img.rows) && (!L3))
	{
		x3 = 0;
		while((x3 < img.cols) && (!L3))
		{
			L3 = (int)img.at<uchar>(y3, x3);
			x3++;
		}
		y3++;
	}
	y3--;
	x3--;

	//procura a quarta linha estrategica
	int y4 = y3; int x4 = 0;
	int L4 = (int)img.at<uchar>(y3, x3);
	while ((y4 < img.rows) && (L4))
	{
		x4 = 0;
		do
		{
			L4 = (int)img.at<uchar>(y4, x4);
			x4++;
		} while ((x4 < img.cols) && (!L4));
		y4++;
	}

	//procura a primeira vertical estrategica
	int y5 = 0; int x5 = 0;
	int L5 = (int)img.at<uchar>(0, 0);
	while ( (x5 < img.cols) && (L5 == 0) )
	{
		y5 = 0;
		while ((y5 < img.rows) && (L5 == 0))
		{
			L5 = (int)img.at<uchar>(y5, x5);
			y5++;
		}
		x5++;
	}
	
	//procura a segunda vertical estrategica
	int y6 = 0;  int x6 = img.cols - 1;
	int L6 = (int)img.at<uchar>(y6, x6);
	while ((x6 > 0) && (!L6))
	{
		y6 = 0;
		while ((y6 < img.rows) && (!L6))
		{
			L6 = (int)img.at<uchar>(y6, x6);
			y6++;
		}
		x6--;
	}
	

	Rect Rs;
	Rs.x = x5 - D;
	Rs.y = y1 - D * 1.3;
	Rs.width = x6 - x5 + 2 * D;
	Rs.height = y2 - y1 + D;

	RECTS.push_back(Rs);
	
	Rect Ri;
	Ri.x = x5 - D;
	Ri.y = y3 - D * 1.3;
	Ri.width = x6 - x5 + 2 * D;
	Ri.height = y4 - y3 + D;

	RECTS.push_back(Ri);

	return RECTS;
}


std::vector <FoundObject> More_Evolution(Mat scn0, std::vector <FoundObject> TPList, std::vector <std::vector <Mat>> TemplateList, std::vector <std::vector <Mat>> TemplatePartsList)
{

	Mat scn1(scn0.rows, scn0.cols, CV_8UC1, Scalar(0));

	for (int i = 0; i < TPList.size(); i++)
	{
		if (TPList[i].valid)
		{
			for (int y = 0; y < scn1.rows; y++)
				for (int x = 0; x < scn1.cols; x++)
				{
					if ((y >= TPList[i].Crop.y) && (y <= TPList[i].Crop.y + TPList[i].Crop.height) &&
						(x >= TPList[i].Crop.x) && (x <= TPList[i].Crop.x + TPList[i].Crop.width))
					{
						scn1.at<uchar>(y, x) = 255;
					}
				}
		}
	}


	Mat mask = buildmask(scn1);

	
	std::vector <FoundObject> ExtraTPList;
	
	// procura por um suco amarelo: Atenção Isso é só um teste
	// só funciona para um objeto extra no momento!!
	FoundObject  FO;
	FO.obj = 12;
	FO.valid = 0;
	ExtraTPList.push_back(FO);


	int ind;
	int tmplId;
	for (int i = 0; i < ExtraTPList.size(); i++)
	{
		ind = ExtraTPList[i].obj;

		if (!ExtraTPList[i].valid)
		{

			double rat1; // , rat2, rat3, rat4, rat5;
			std::vector  <SceneLoc> ListOfValidScales; SceneLoc SL;
			double maxValmax = -1000; Point maxLoc1max; Mat tmpl1max; int rmax;
			for (int r = 0; r < maxRAT; r++)
			{
				Mat src1;
				scn0.copyTo(src1);

				Mat mask1;
				mask.copyTo(mask1);

				double maxVal1; Point maxLoc1; rat1 = RAT[r];

				Mat tmpl1;
				
				for (int t = 0; t < (TemplateList[ind]).size(); t++)
				{
					Mat tmpl1 = (TemplateList[ind])[t];

					mathTemplateEdge(src1, tmpl1, rat1, &maxVal1, &maxLoc1, mask1);
					if (maxVal1 > maxValmax)
					{
						maxValmax = maxVal1;
						maxLoc1max = maxLoc1;
						tmpl1max = tmpl1;
						rmax = r;
						tmplId = t;
					}
				}
			}

			if (maxValmax > 0.60)
			{
				cout << NEW_OBJECTS_DIR_NAMES[ind] << " : ANOTHER MORE FOUND!!!" << endl;
				ExtraTPList[i].valid = 1;
				ExtraTPList[i].Crop.x = maxLoc1max.x;
				ExtraTPList[i].Crop.y = maxLoc1max.y;
				ExtraTPList[i].Crop.width = (int)round(tmpl1max.cols * RAT[rmax]);
				ExtraTPList[i].Crop.height = (int)round(tmpl1max.rows * RAT[rmax]);
				ExtraTPList[i].indTmpl = tmplId;
				ExtraTPList[i].r = rmax;
				ExtraTPList[i].img = scn0(ExtraTPList[i].Crop);
			}
		}
	}


	for (int i = 0; i < ExtraTPList.size(); i++)
	{
		if (ExtraTPList[i].valid)
			TPList.push_back(ExtraTPList[i]);
	}

	return TPList;

}

void splitRecognition(Mat scn0) {
	int row1 = (int)(scn0.rows / NREGIONS_Y);
	int col1 = (int)(scn0.cols / NREGIONS_X);
	for (int i = 0; i < NREGIONS_Y; i++) {
		for (int j = 0; j < NREGIONS_X; j++) {
			Rect R(j*col1, i*row1, ((j + 1)*col1) - (j*col1), ((i + 1)*row1) - (i*row1));
			Mat temp = scn0(R);
			//imshow("region", temp);
			//waitKey();
		}
	}
}

std::vector <FoundObject> Remainder_Evolution(Mat scn0, std::vector <FoundObject> TPList, std::vector <std::vector <Mat>> TemplateList, std::vector <std::vector <Mat>> TemplatePartsList)
{

	//std::vector <std::vector <Mat>> TPList;
	
	// prepara um novo cenario
	//Mat scn1 = imread("scn1.png");
	//imshow("scn1",scn1);
	//waitKey(0);

	Mat scn1(scn0.rows, scn0.cols, CV_8UC1, Scalar(0));

	for (int i = 0; i < TPList.size(); i++)
	{
		if (TPList[i].valid)
		{
			for (int y = 0; y < scn1.rows; y++)
				for (int x = 0; x < scn1.cols; x++)
				{
					if ((y >= TPList[i].Crop.y) && (y <= TPList[i].Crop.y + TPList[i].Crop.height) &&
						(x >= TPList[i].Crop.x) && (x <= TPList[i].Crop.x + TPList[i].Crop.width))
					{
						scn1.at<uchar>(y, x) = 255;
					}
				}
		}
	}
	Mat mask = buildmask(scn1);
	imshow("mask", mask);
	waitKey();
	int tmplId;
	//splitRecognition(scn0);
	for (int i = 0; i < NUMOBJS; i++)
	{
		if (!TPList[i].valid)
		{
			double rat1; // , rat2, rat3, rat4, rat5;
			std::vector  <SceneLoc> ListOfValidScales; SceneLoc SL;
			double maxValmax = -1000; Point maxLoc1max; Mat tmpl1max; int rmax;
			for (int r = 0; r < maxRAT; r++)
			{
				Mat src1;
				scn0.copyTo(src1);

				Mat mask1;
				mask.copyTo(mask1);

				double maxVal1; Point maxLoc1; rat1 = RAT[r];

				//imshow("tmpl0", mask1);
				//waitKey(0);

				Mat tmpl1;
				for (int t=0; t < (TemplateList[i]).size(); t++)
				{
					Mat tmpl1 = (TemplateList[i])[t];

					mathTemplateEdge(src1, tmpl1, rat1, &maxVal1, &maxLoc1, mask1);
					//cout << "maxVal = " << maxVal1 << endl;
					if (maxVal1 > maxValmax)
					{
						maxValmax = maxVal1;
						maxLoc1max = maxLoc1;
						tmpl1max = tmpl1;
						rmax = r;
						tmplId = t;
					}
				}
			}
			cout<< NEW_OBJECTS_DIR_NAMES[i]<<" = " << maxValmax << endl;
			if (maxValmax > 0.87)
			{
				cout << NEW_OBJECTS_DIR_NAMES[i] << " : FINALLY FOUND :)) !!" << endl;
				TPList[i].valid = 1;
				TPList[i].firstValid = 1;
				TPList[i].Crop.x = maxLoc1max.x;
				TPList[i].Crop.y = maxLoc1max.y;
				TPList[i].Crop.width = (int)round(tmpl1max.cols * RAT[rmax]);
				TPList[i].Crop.height = (int)round(tmpl1max.rows * RAT[rmax]);
				TPList[i].indTmpl = tmplId;
				TPList[i].r = rmax;
				TPList[i].img = scn0(TPList[i].Crop);
				TPList[i].fase = 2;
			}
		}
	}

	return TPList;
}






Mat projtemplate(Mat src, Mat dst, Point P)
{

	Mat src1, dst1;
	src.copyTo(src1);
	dst.copyTo(dst1);

	for (int y=0; y < src.rows; y++)
		for (int x = 0; x < src1.cols; x++)
			dst1.at<uchar>(y + P.y, x + P.x) = src1.at<uchar>(y,x);

	return dst1;
}


double DIFSEG(Mat img1, Mat img2)
{
	double DIF = 0; 
	int ymin1 = 1000; int xmin1 = 1000; int ymax1 = -1000; int xmax1 = -1000;
	int ymin2 = 1000; int xmin2 = 1000; int ymax2 = -1000; int xmax2 = -1000;
	for (int y=0; y < img1.rows; y++)
		for (int x = 0; x < img1.cols; x++)
		{
			int L1 = (int)img1.at<uchar>(y, x);
			int L2 = (int)img2.at<uchar>(y, x);
			DIF = DIF + abs(L1 - L2);

			if (L1 != 0)
			{
				if (y < ymin1)  ymin1 = y;
				if (y > ymax1)  ymax1 = y;
				
				if (x < xmin1)  xmin1 = x;
				if (x > xmax1)  xmax1 = x;
			}

			if (L2 != 0)
			{
				if (y < ymin2)  ymin2 = y;
				if (y > ymax2)  ymax2 = y;

				if (x < xmin2)  xmin2 = x;
				if (x > xmax2)  xmax2 = x;
			}

		}

	double d1 = sqrt((ymin1 - ymax1)*(ymin1 - ymax1) + (xmin1 - xmax1)*(xmin1 - xmax1));
	double d2 = sqrt((ymin2 - ymax2)*(ymin2 - ymax2) + (xmin2 - xmax2)*(xmin2 - xmax2));

	double ratdiag;
	if (d1 > d2)  ratdiag = d2 / d1;
	else          ratdiag = d1 / d2;

	//cout << ratdiag << endl;

	return ratdiag*(1 - (DIF/(255*(img1.rows * img1.cols))));
}



std::vector<Mat> SplitRegions(Mat im, int flag, double precision)
{
	Mat imroi(im.size(), CV_8UC1);
	int nLabels = connectedComponents(im, imroi, 8);

	//cout << "Analisando " << nLabels << " regioes candidatas ..." << endl;

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


	for (int r = 0; r < nLabels; r++) {
		double pnpixels = (double)NumPixelsRegions[r] / (im.rows * im.cols);
		if (pnpixels >= precision) {
			Regioes.push_back(ImRegions[r]);
		}
	}

	cout << "Achou " << Regioes.size() << " regioes candidatas" << endl;


	return Regioes;
}

void ColorHistograms(Mat src0, double HIST[], int DIM)
{

	// converte ambas as imagens para HSV
	Mat img1;
	src0.copyTo(img1);

	const float ratio = 255;
	const int C1 = 18;
	const int C2 =  3;
	const int C3 =  2;

	cvtColor(img1, img1, CV_BGR2HSV);


	Vec3b intensity1;
	int total_pixels1 = 0;
	// calcula o histograna para img1
	for (int y = 0; y < img1.rows; y++) {
		for (int x = 0; x < img1.cols; x++)
		{
			intensity1 = img1.at<Vec3b>(y, x);
			int I1 = floor(((float)intensity1.val[0] / ratio) * (C1 - 1) + 0.5);
			int I2 = floor(((float)intensity1.val[1] / ratio) * (C2 - 1) + 0.5);
			int I3 = floor(((float)intensity1.val[2] / ratio) * (C3 - 1) + 0.5);

			int indx1 = C2 * C3 * I1 + C3 * I2 + I3;
			HIST[indx1] = HIST[indx1] + 1.0;
			total_pixels1 = total_pixels1 + 1;
		}
	}

	for (int i = 0; i < DIM; i++)  HIST[i] = HIST[i] / (float)total_pixels1;

}



std::vector <Mat>  psrCColorsegmentation(Mat src, double cut)
{
	int i, x, y;

	Mat img1;
	src.copyTo(img1);

	Mat dst;
	dst.create(src.rows, src.cols, CV_8UC3);

	std::vector <Mat> ListSeg;

	const int C1 = 18;
	const int C2 =  3;
	const int C3 =  3;

	const int DIM = C1 * C2 * C3;

	/////  PRIMEIRA CARACTERISTICA 
	// EXTRAI CARACTERISICAS DE COR - HISTOGRAMA HSV162
	double HIST[DIM];
	int INDX[DIM];

	for (i = 0; i < DIM; i++)  HIST[i] = 0.0;

	// EXTRAI CARACTERISICAS DE COR - HISTOGRAMA HSV162
	ColorHistograms(img1, HIST, DIM);

	int tot = 0;
	for (int i = 0; i < DIM; i++)
	{
		if (HIST[i] <= cut){
			HIST[i] = 0;
			INDX[i] = 0;
		}
		else {
			INDX[i] = tot;
			Mat imgt(src.rows, src.cols, CV_8UC1, Scalar(0));
			ListSeg.push_back(imgt);
			tot++;
		}
	}

	/*for (int i = 0; i < DIM; i++)
		cout << i << " " << INDX[i] << " " << HIST[i] << endl;

	system("pause");
	exit(0);*/

	cvtColor(img1, img1, CV_BGR2HSV);

	Vec3b intensity;
	for (y = 0; y < img1.rows; y++)
		for (x = 0; x < img1.cols; x++)
		{
			intensity = src.at<Vec3b>(y, x);

			int L1 = floor((((int)intensity.val[0] / 255.0)*(C1 - 1)) + 0.5);
			int L2 = floor((((int)intensity.val[1] / 255.0)*(C2 - 1)) + 0.5);
			int L3 = floor((((int)intensity.val[2] / 255.0)*(C3 - 1)) + 0.5);

			int indx =  C2 * C3 * L1 + C3 * L2 + L3;

			if (INDX[indx])
			   ListSeg[INDX[indx]].at<uchar>(y, x) = 255;
			//else
			  // ListSeg[INDX[indx]].at<uchar>(y, x) = 255;
		}

	return ListSeg;

}
bool sort_by_segment(segmentos A, segmentos B)
{
	if (A.area > B.area)
		return true;
	return false;
}
std::vector <segmentos> tsallis_segmentation(Mat src0)
{

	double q = 0.5;

	int t0max = 0, t1max = 0, t2max = 0;

	Mat src1;

	src0.copyTo(src1);

	// calculo do histograma
	double hist[256];

	for (int i = 0; i < 256; i++) hist[i] = 0;


	double tot = 0;
	for (int y = 0; y < src1.rows; y++)
		for (int x = 0; x < src1.cols; x++)
		{
			int L = src1.at<uchar>(y, x);
			hist[L] = hist[L] + 1;
			tot++;
		}

	for (int i = 0; i < 256; i++) hist[i] = hist[i] / tot;


	// calcula o melhor limiar geral
	double ET1max = -1000;
	for (int t = 1; t < 255; t++)
	{
		// normaliza a distribuicao antes de t
		double t1 = 0;
		for (int i = 0; i < t; i++)  t1 = t1 + hist[i];

		// calcula a entropia de tsallis antes de t
		double S1 = 0;
		for (int i = 0; i < t; i++)  S1 = S1 + pow(hist[i] / t1, q);

		double ET1 = (1 - S1) / (q - 1);


		// normaliza a distribuicao apos t
		double t2 = 0;
		for (int i = t; i < 256; i++)  t2 = t2 + hist[i];

		// calcula a entropia de tsallis depois de t
		double S2 = 0;
		for (int i = t; i < 256; i++)  S2 = S2 + pow(hist[i] / t2, q);

		double ET2 = (1 - S2) / (q - 1);

		double ET = ET1 + ET2;

		if (ET > ET1max)
		{
			ET1max = ET;
			t1max = t;
		}
	}


	// calcula o melhor limiar antes de t1max
	double ET0max = -1000;
	for (int t = 1; t < t1max; t++)
	{
		// normaliza a distribuicao antes de t0max
		double t11 = 0;
		for (int i = 0; i < t; i++)  t11 = t11 + hist[i];

		// calcula a entropia de tsallis antes de t0max	
		double S11 = 0;
		for (int i = 0; i < t; i++)  S11 = S11 + pow(hist[i] / t11, q);

		double ET11 = (1 - S11) / (q - 1);

		// normaliza a distribuicao apos t0max
		double t12 = 0;
		for (int i = t; i <= t1max; i++)  t12 = t12 + hist[i];

		// calcula a entropia de tsallis depois de t0max
		double S12 = 0;
		for (int i = t; i <= t1max; i++)  S12 = S12 + pow(hist[i] / t12, q);

		double ET12 = (1 - S12) / (q - 1);

		double ET0 = ET11 + ET12;

		if (ET0 > ET0max)
		{
			ET0max = ET0;
			t0max = t;
		}
	}


	// calcula o melhor limiar apos t1max
	double ET2max = -1000;
	for (int t = t1max + 1; t < 256; t++)
	{
		// normaliza a distribuicao antes de t2max
		double t21 = 0;
		for (int i = t1max; i < t; i++)  t21 = t21 + hist[i];

		// calcula a entropia de tsallis antes de t2max	
		double S21 = 0;
		for (int i = t1max; i < t; i++)  S21 = S21 + pow(hist[i] / t21, q);

		double ET21 = (1 - S21) / (q - 1);


		// normaliza a distribuicao apos t2max
		double t22 = 0;
		for (int i = t; i < 256; i++)  t22 = t22 + hist[i];


		// calcula a entropia de tsallis depois de t2max
		double S22 = 0;
		for (int i = t; i < 256; i++)  S22 = S22 + pow(hist[i] / t22, q);

		double ET22 = (1 - S22) / (q - 1);

		double ET2 = ET21 + ET22;

		if (ET2 > ET2max)
		{
			ET2max = ET2;
			t2max = t;
		}
	}

	//cout << t0max << " " << t1max << " " << t2max << endl;

	// segmenta
	Mat dst(src1.rows, src1.cols, CV_8UC1, Scalar(0));

	Mat seg1(src1.rows, src1.cols, CV_8UC1, Scalar(0));
	Mat seg2(src1.rows, src1.cols, CV_8UC1, Scalar(0));
	Mat seg3(src1.rows, src1.cols, CV_8UC1, Scalar(0));
	Mat seg4(src1.rows, src1.cols, CV_8UC1, Scalar(0));


	int area1 = 0, area2 = 0, area3 = 0, area4 = 0;
	for (int y = 0; y <src1.rows; y++)
		for (int x = 0; x < src1.cols; x++)
		{
			int L = src1.at<uchar>(y, x);
			if (L < t0max)
			{
				dst.at<uchar>(y, x) = 0;
				seg1.at<uchar>(y, x) = 255;
				area1 = area1 + 1;
			}
			else
			{
				if ((L > t0max) && (L < t1max))
				{
					dst.at<uchar>(y, x) = 64;
					seg2.at<uchar>(y, x) = 255;
					area2 = area2 + 1;
				}
				else
				{
					if ((L > t1max) && (L < t2max))
					{
						dst.at<uchar>(y, x) = 160;
						seg3.at<uchar>(y, x) = 255;
						area3 = area3 + 1;
					}
					else
					{
						dst.at<uchar>(y, x) = 255;
						seg4.at<uchar>(y, x) = 255;
						area4 = area4 + 1;
					}
				}
			}
		}

	std::vector <segmentos> Listseg;

	segmentos DST, SEG1, SEG2, SEG3, SEG4;

	DST.gyseg = dst;
	DST.area = 0;
	Listseg.push_back(DST);

	SEG1.bwseg = seg1;
	SEG1.area = area1;
	Listseg.push_back(SEG1);

	SEG2.bwseg = seg2;
	SEG2.area = area2;
	Listseg.push_back(SEG2);

	SEG3.bwseg = seg3;
	SEG3.area = area3;
	Listseg.push_back(SEG3);

	SEG4.bwseg = seg4;
	SEG4.area = area4;
	Listseg.push_back(SEG4);

	return Listseg;
}




void psrCutHSV23segmentation(Mat img, Mat hsv_cut, double cut)
{
	int i, x, y;

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


	for (y = row1; y < 2 * row1; y++)
		for (x = 0; x < hsv.cols; x++)
		{
			intensity = hsv.at<Vec3b>(y, x);

			int Hn = floor((((int)intensity.val[0] / 179.0)*(Hbin - 1)) + 0.5);
			int Sn = floor((((int)intensity.val[1] / 255.0)*(Sbin - 1)) + 0.5);
			int Vn = floor((((int)intensity.val[2] / 255.0)*(Vbin - 1)) + 0.5);

			int indx = (int)(DIMENSAO_COR / 3) + (Sbin*Vbin) * Hn + Vbin * Sn + Vn;

			if (!HSV_cut_hist[indx]) hsv_cut.at<uchar>(y, x) = (uchar)(255);
		}


	for (y = 2 * row1; y < 3 * row1; y++)
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



double DifCos(double V1[], double V2[], int INI, int DIM)
{

	double S0 = 0, S1 = 0, S2 = 0;
	for (int i = INI; i < DIM; i++)
	{
		S0 = S0 + V1[i] * V2[i];
		S1 = S1 + V1[i] * V1[i];
		S2 = S2 + V2[i] * V2[i];
	}

	double DIF = 0;
	if (S1 && S2)
		DIF = S0 / (sqrt(S1) * sqrt(S2));

	return DIF;
}



double matchColHistograms(Mat src1, Mat src2)
{

	// converte ambas as imagens para HSV
	Mat img1, img2;
	src1.copyTo(img1);
	src2.copyTo(img2);

	cvtColor(img1, img1, CV_BGR2Luv);
	cvtColor(img2, img2, CV_BGR2Luv);

	//GaussianBlur(img1, img1, Size(3, 3), 0, 0, BORDER_DEFAULT);
	//GaussianBlur(img2, img2, Size(3, 3), 0, 0, BORDER_DEFAULT);

	// configura a discretizacao do histograma HSV
	// o melhor é o 50, 10, 2
	const int C1 = 50;
	const int C2 = 10;
	const int C3 = 2;

	// define a dimensao final do histograma extendido
	const int DIM = C1 * C2 * C3;

	// define histogramas extendidos
	double* H1 = new double[DIM];
	double* H2 = new double[DIM];

	// garante que estejam zerados
	for (int i = 0; i < DIM; i++) H1[i] = 0;
	for (int i = 0; i < DIM; i++) H2[i] = 0;


	Vec3b intensity1;
	int total_pixels1 = 0;
	// calcula o histograna para img1
	for (int y = 0; y < img1.rows; y++) {
		for (int x = 0; x < img1.cols; x++)
		{
			intensity1 = img1.at<Vec3b>(y, x);
			int I1 = floor(((float)intensity1.val[0] / 255.0) * (C1 - 1) + 0.5);
			int I2 = floor(((float)intensity1.val[1] / 255.0) * (C2 - 1) + 0.5);
			int I3 = floor(((float)intensity1.val[2] / 255.0) * (C3 - 1) + 0.5);

			//cout << ((int)intensity1.val[0] / 179.0) << " " << ((int)intensity1.val[1] / 255.0) << " " << ((int)intensity1.val[2] / 255.0) << endl;
			//cout << I1 << " " << I2 << " " << I3 << endl;
			//system("pause");
			//exit(0);

			int indx1 = C2 * C3 * I1 + C3 * I2 + I3;
			H1[indx1] = H1[indx1] + 1.0;
			total_pixels1 = total_pixels1 + 1;
		}
	}
	//cout << "start" << endl;
	for (int i = 0; i < DIM; i++)
	{
		H1[i] = H1[i] / (float)total_pixels1;
	}

	//system("pause");
	//exit(0);


	Vec3b intensity2;
	int total_pixels2 = 0;
	// calcula o histograna para img2
	for (int y = 0; y < img2.rows; y++) {
		for (int x = 0; x < img2.cols; x++)
		{
			intensity2 = img2.at<Vec3b>(y, x);
			int I1 = floor(((float)intensity2.val[0] / 255.0) * (C1 - 1) + 0.5);
			int I2 = floor(((float)intensity2.val[1] / 255.0) * (C2 - 1) + 0.5);
			int I3 = floor(((float)intensity2.val[2] / 255.0) * (C3 - 1) + 0.5);

			int indx2 = C2 * C3 * I1 + C3 * I2 + I3;
			H2[indx2] = H2[indx2] + 1.0;
			total_pixels2 = total_pixels2 + 1;
		}
	}
	for (int i = 0; i < DIM; i++) {
		H2[i] = H2[i] / (float)total_pixels2;
	}

	double S0 = 0, S1 = 0, S2 = 0;
	for (int i = 0; i < DIM; i++)
	{
		S0 = S0 + H1[i] * H2[i];
		S1 = S1 + H1[i] * H1[i];
		S2 = S2 + H2[i] * H2[i];
	}

	double DIFCOS = 0;
	if ((S1 != 0) && (S2 != 0))
		DIFCOS = S0 / (sqrt(S1) * sqrt(S2));

	//cout << "DIFCOS = " << DIFCOS << endl;
	//system("pause");
	//exit(0);

	return DIFCOS;
}





double matchBlocColHistograms(Mat img1, Mat img2)
{

	// constante para dimensao de blocos
	const int b = 2;

	// converter a imagem 1 pra multiplo de b, reescaloando-a
	int lin1 = img1.rows;
	int col1 = img1.cols;
	float Fl1 = b * floor(lin1 / b) / lin1;
	float Fc1 = b * floor(col1 / b) / col1;
	int newlin1 = Fl1 * lin1;
	int newcol1 = Fc1 * col1;


	// converter a imagem 2 pra multiplo de b, reescaloando-a
	int lin2 = img2.rows;
	int col2 = img2.cols;
	float Fl2 = b * floor(lin2 / b) / lin2;
	float Fc2 = b * floor(col2 / b) / col2;
	int newlin2 = Fl2 * lin2;
	int newcol2 = Fc2 * col2;



	// reescalonamento
	Mat img1r, img2r;
	cv::resize(img1, img1r, cv::Size(newcol1, newlin1), 0, 0, CV_INTER_LINEAR);
	cv::resize(img2, img2r, cv::Size(newcol2, newlin2), 0, 0, CV_INTER_LINEAR);


	// converte ambas as imagens para HSV
	Mat hsv1, hsv2;
	//cvtColor(img1r, hsv1, CV_BGR2HSV);
	//cvtColor(img2r, hsv2, CV_BGR2HSV);
	cvtColor(img1r, hsv1, CV_BGR2Luv);
	cvtColor(img2r, hsv2, CV_BGR2Luv);


	// configura a discretizacao do histograma HSV
	const int Rdisc = 10;
	const int Gdisc = 10;
	const int Bdisc =  2;

	// define a dimensao final do histograma extendido
	const int DIM = Rdisc * Gdisc * Bdisc;
	const int DIM_EXP = b * b * DIM;

	// define histogramas extendidos
	double* RGB1 = new double[DIM_EXP];
	double* RGB2 = new double[DIM_EXP];

	// garante que estejam zerados
	for (int i = 0; i < DIM_EXP; i++) RGB1[i] = 0;
	for (int i = 0; i < DIM_EXP; i++) RGB2[i] = 0;


	// calcula histograma da imagem 1, um para cada bloco
	int y1 = 0; int y2 = newlin1 / b;
	int HEXP = 0;
	for (int j = 0; j < b; j++)
	{
		int x1 = 0; int x2 = newcol1 / b;
		for (int i = 0; i < b; i++)
		{
			Vec3b intensity;
			int total_pixels1 = 0;
			// calcula o histograna HSV1 para 1 bloco
			for (int y = y1; y < y2; y++) {
				for (int x = x1; x < x2; x++)
				{
					intensity = hsv1.at<Vec3b>(y, x);
					int Rn = floor(((float)intensity.val[0] / 179.0) * (Bdisc - 1) + 0.5);
					int Gn = floor(((float)intensity.val[1] / 255.0) * (Gdisc - 1) + 0.5);
					int Bn = floor(((float)intensity.val[2] / 255.0) * (Rdisc - 1) + 0.5);

					int indx1 = Gdisc * Bdisc * Rn + Bdisc * Gn + Bn;
					RGB1[HEXP + indx1] = RGB1[HEXP + indx1] + 1.0;
					total_pixels1 = total_pixels1 + 1;
				}
			}
			for (int i = HEXP; i < DIM + HEXP; i++)  RGB1[i] = RGB1[i] / (float)total_pixels1;
			x1 = x2;
			x2 = x2 + newcol1 / b;
			HEXP = HEXP + DIM;
		}
		y1 = y2;
		y2 = y2 + newlin1 / b;
	}


	HEXP = 0;
	y1 = 0; y2 = newlin2 / b;
	for (int j = 0; j < b; j++)
	{
		int x1 = 0; int x2 = newcol2 / b;
		for (int i = 0; i < b; i++)
		{
			Vec3b intensity;
			int total_pixels2 = 0;
			// calcula o histograna HSV1 para 1 bloco
			for (int y = y1; y < y2; y++)
				for (int x = x1; x < x2; x++)
				{
					intensity = hsv2.at<Vec3b>(y, x);
					int Rn = floor(((float)intensity.val[0] / 255.0) * (Bdisc - 1) + 0.5);
					int Gn = floor(((float)intensity.val[1] / 255.0) * (Gdisc - 1) + 0.5);
					int Bn = floor(((float)intensity.val[2] / 255.0) * (Rdisc - 1) + 0.5);

					int indx2 = Rdisc * Bdisc * Rn + Bdisc * Gn + Bn;
					RGB2[HEXP + indx2] = RGB2[HEXP + indx2] + 1.0;
					total_pixels2 = total_pixels2 + 1;
				}
			for (int i = HEXP; i < DIM + HEXP; i++)  RGB2[i] = RGB2[i] / (float)total_pixels2;
			x1 = x2;
			x2 = x2 + newcol2 / b;
			HEXP = HEXP + DIM;
		}
		y1 = y2;
		y2 = y2 + newlin2 / b;
	}


	// diferenca quadratica entre os histogramas extendidos de  b * b blocos
	HEXP = 0; double S = 0;
	for (int i = 0; i < b*b; i++)
	{
		S = S + DifCos(RGB1, RGB2, HEXP, HEXP + DIM);
		HEXP = HEXP + DIM;
	}

	return S / (b*b);
}



std::vector <Mat> Colorfeatures(Mat src, Mat tmpl, double rat)
{

	Mat src1, tmpl1;
	
	src.copyTo(src1);
	tmpl.copyTo(tmpl1);

	std::vector <Mat> im_finds;

	//GaussianBlur(src1, src1, Size(3, 3), 0, 0, BORDER_DEFAULT);
	//GaussianBlur(tmpl1, tmpl1, Size(3, 3), 0, 0, BORDER_DEFAULT);

	cvtColor(tmpl1, tmpl1, CV_BGR2Luv);
	cvtColor(src1, src1, CV_BGR2Luv);

    int newrows = floor(tmpl1.rows * rat + 0.5);
	int newcols = floor(tmpl1.cols * rat + 0.5);
	cv::resize(tmpl1, tmpl1, cv::Size(newcols, newrows), 0, 0, CV_INTER_LINEAR);

	Mat result;
	int result_rows = src1.rows - tmpl1.rows + 1;
	int result_cols = src1.cols - tmpl1.cols + 1;
	result.create(result_rows, result_cols, CV_32FC1);

	im_finds.push_back(src1);
	im_finds.push_back(tmpl1);
	im_finds.push_back(result);

	return im_finds;
}




void mathTemplateColor(Mat src, Mat tmpl, double rat, double* maxVal, Point* maxLoc, Mat mask)
{

	std::vector <Mat> im_finds = Colorfeatures(src, tmpl, rat);
	Mat src_col = im_finds[0];
	Mat tmpl_col = im_finds[1];
	Mat result = im_finds[2];

	matchTemplate(src_col, tmpl_col, result, CV_TM_CCORR_NORMED);

	double minVal2; double maxVal2; Point minLoc2; Point maxLoc2;

	if (mask.data != NULL)
		minMaxLoc(result, &minVal2, &maxVal2, &minLoc2, &maxLoc2, mask);
	else
		minMaxLoc(result, &minVal2, &maxVal2, &minLoc2, &maxLoc2, Mat());

	(*maxVal) = maxVal2;
	(*maxLoc) = maxLoc2;

}



void mathTemplateEdge(Mat src, Mat tmpl, double rat, double* maxVal, Point* maxLoc, Mat mask)
{

	std::vector <Mat> im_finds = EdgeScharr(src, tmpl, rat);
	Mat src_grad = im_finds[0];
	Mat tmpl_grad = im_finds[1];
	Mat result = im_finds[2];

	matchTemplate(src_grad, tmpl_grad, result, CV_TM_CCORR_NORMED);
	double minVal2; double maxVal2; Point minLoc2; Point maxLoc2;

	double S = 0;
	if (mask.data != NULL)
	{
		minMaxLoc(result, &minVal2, &maxVal2, &minLoc2, &maxLoc2, mask);

		Rect R;
		if (maxLoc2.x < 0)
			maxLoc2.x = 0;
		if (maxLoc2.y < 0)
			maxLoc2.y = 0;
		R.x = maxLoc2.x;
		R.y = maxLoc2.y;
		R.width = tmpl_grad.cols;
		R.height = tmpl_grad.rows;
		Mat tmplcrop = mask(R);

		for (int y = 0; y < tmplcrop.rows; y++)
		 for (int x = 0; x < tmplcrop.cols; x++)
			 S = S + (double)((int)tmplcrop.at<uchar>(y, x));

		S = (double) S / (double)(255 * tmplcrop.rows * tmplcrop.cols);
	}
	else
	{
		minMaxLoc(result, &minVal2, &maxVal2, &minLoc2, &maxLoc2, Mat());
		S = 1;
	}

	(*maxVal) = maxVal2 * S;
	(*maxLoc) = maxLoc2;

}


void showNextFrameBoxes(Mat scene, std::vector<FoundObject> NextFrameList) {
	Mat scn;
	scene.copyTo(scn);
	for (int i = 0; i < NextFrameList.size(); i++) {
		Point P1, P2;
		P1.x = NextFrameList[i].Crop.x;
		P1.y = NextFrameList[i].Crop.y;
		P2.x = P1.x + NextFrameList[i].Crop.width;
		P2.y = P1.y + NextFrameList[i].Crop.height;

		cv::rectangle(scn, P1, P2, Scalar(255, 0, 0), 2, 8);
	}
}

void showFinalResult(Mat scene, std::vector <FoundObject>  LisOfFounds)
{
	int R =  rand() % 255;
	int G = rand() % 255;
	int B = rand() % 255;
	Mat scene2;
	scene.copyTo(scene2);

	for (int i = 0; i < LisOfFounds.size(); i++)
	{
		if (LisOfFounds[i].valid)
		{
			Point P1, P2;
			P1.x = P1.x + LisOfFounds[i].Crop.x;
			P1.y = P1.y + LisOfFounds[i].Crop.y;
			P2.x = P1.x + LisOfFounds[i].Crop.width;
			P2.y = P1.y + LisOfFounds[i].Crop.height;
			if(LisOfFounds[i].fase == 1)
				cv::rectangle(scene2, P1, P2, cv::Scalar(0,0,255), 2, 8, 0);
			if (LisOfFounds[i].fase == 2)
				cv::rectangle(scene2, P1, P2, cv::Scalar(255, 0, 0), 2, 8, 0);
			int obj = LisOfFounds[i].obj;
			cv::putText(scene2, NEW_OBJECTS_DIR_NAMES[obj], cv::Point(P1.x - 10, P1.y - 10), cv::FONT_HERSHEY_COMPLEX_SMALL, 0.5, cv::Scalar(255, 255, 255));
		}

		//Mat win = scene2(LisOfFounds[i].Crop);
		//string strwin = "win" + std::to_string(i) + ".png";
		//imwrite(strwin, win);
    }
	//putText(scene2, "CHUPA CAPONE", Point(150, 360), cv::FONT_HERSHEY_SIMPLEX, 1, Scalar(0, 0, 0), 3);
	//putText(scene2, "CHUPA PLINEX", Point(150, 450), cv::FONT_HERSHEY_SIMPLEX, 1, Scalar(255, 0, 255), 3);
	//resize(scene, scene, Size((int)round(scene.cols*0.6), (int)round(scene.rows*0.6)));
	imshow("FEI @home Final Tracking - Versao Beta 001", scene2);
}




FoundObject Individual_evolution(Mat scene, Mat tmpl, Mat sub_tmpl, std::vector  <SceneLoc> SL, int ind)
{

	FoundObject FO;
	double maxVal1 = 0, maxVal2 = 0, maxVal3 = 0, maxVal4 = 0, maxVal5 = 0, maxVal = 0, winVal = -1000;
	int winind;
	for (int i = 0; i < SL.size(); i++)
	{
		maxVal1 = SL[i].maxVal1;
		maxVal2 = SL[i].maxVal2;
		maxVal3 = SL[i].maxVal3;
		maxVal4 = SL[i].maxVal4;
		maxVal5 = SL[i].maxVal5;

		maxVal = maxVal1 + maxVal2 + maxVal3 + maxVal4 + maxVal5;

		if (maxVal > winVal)
		{
			winVal = maxVal;
			winind = i;
		}
	}

	int x0 = SL[winind].C.x;
	int y0 = SL[winind].C.y;
	int L0 = SL[winind].C.width;
	int A0 = SL[winind].C.height;

	int dx = L0 * 0.0;
	int dy = A0 * 0.2;

	int x1 = x0 - dx;
	int y1 = y0 - dy;
	int L1 = L0 + 2 * dx;
	int A1 = A0 + 2 * dy;
	if (x1 + L1 > scene.cols)
		L1 = scene.cols - x1 - 1;
	if (x1 < 0)
		x1 = 0;
	
	if (y1 + A1 > scene.rows)
		A1 = scene.rows - y1 - 1;
	if (y1 < 0)
		y1 = 0;
	
	FO.Crop.x = x1; // SL[winind].C.x;
	FO.Crop.y = y1; // SL[winind].C.y;
	FO.Crop.width = L1; // SL[winind].C.width;
	FO.Crop.height = A1; // SL[winind].C.height;
	FO.valid = 1;
	FO.obj = ind;
	FO.firstValid = 0;
	resize(tmpl,FO.img,FO.Crop.size());
	FO.indTmpl = 0;
	FO.r = winind;
	FO.fase = 1;
	Mat win = scene(FO.Crop);

	Mat win2(A1, L1, CV_8UC1, Scalar(0));

	//cout << "OK1" << endl;


	std::vector<unsigned int> bests;
	std::vector<unsigned int> bests2;

	Mat gry;
	cvtColor(win, gry, CV_RGB2GRAY);

	//cout << "OK1" << endl;

	Mat tmpl_gray;
	cvtColor(tmpl, tmpl_gray, CV_RGB2GRAY);
	cv::resize(tmpl_gray, tmpl_gray, cv::Size(L1, A1), 0, 0, CV_INTER_LINEAR);
	//cv::resize(gry, gry, cv::Size(tmpl.cols, tmpl.rows), 0, 0, CV_INTER_LINEAR);

	/*bests = beststhresholds(gry, 2, 50, 100);
	bests2 = beststhresholds(tmpl_gray, 2, 50, 100);

	std::vector<Mat> SetOfSegmentedRegions1 = SplitSegmentedRegions(gry, bests, notbackwards);
	std::vector<Mat> SetOfSegmentedRegions2 = SplitSegmentedRegions(tmpl_gray, bests2, notbackwards);
	bests.clear();
	*/

	std::vector <segmentos> Listseg1, Listseg2; 

	Listseg1 = tsallis_segmentation(gry);
	Listseg2 = tsallis_segmentation(tmpl_gray);

	std::sort(Listseg1.begin(),Listseg1.end(), sort_by_segment);
	std::sort(Listseg2.begin(),Listseg2.end(), sort_by_segment);

	double rat;
	if (tmpl.rows > tmpl.cols)   rat = (double) win.rows / tmpl.rows;
	else                         rat = (double) win.cols / tmpl.cols;

	double T = 0;
	for (int i = 0; i < 4; i++)
	{
		double dif = DIFSEG(Listseg1[i].bwseg, Listseg2[i].bwseg);

		T = T + dif;
	}

	//cout << NEW_OBJECTS_DIR_NAMES[ind] << " " << T << endl;

	cout<<"T = " << T << endl;
	if (T < 2.4187)
	{
		cout << NEW_OBJECTS_DIR_NAMES[ind] << " :NOT FOUND Yet  :( !!" << endl;
		FO.valid = 0;
	}
	else
	{
			cout << NEW_OBJECTS_DIR_NAMES[ind] << " : FOUND :) !!" << endl;
			FO.valid = 1;
			FO.firstValid = 1;
			FO.Crop.x = x0;
			FO.Crop.y = y0;
			FO.Crop.width = L0;
			FO.Crop.height = A0;
			FO.obj = ind;
			FO.indTmpl = 0;
			FO.r = winind;
			FO.img = scene(FO.Crop);
			FO.T = T;
	}

	return FO;
}



std::vector <Mat> EdgeScharr(Mat src1, Mat tmpl1, double rat)
{

	int scale = 1;
	int delta = 0;
	int ddepth = CV_16S;

	std::vector <Mat> im_finds;

	Mat src_gray, src_grad_x, src_abs_grad_x, src_grad_y, src_abs_grad_y;
	Mat tmpl_gray, tmpl_grad_x, tmpl_abs_grad_x, tmpl_grad_y, tmpl_abs_grad_y;

	cvtColor(tmpl1, tmpl_gray, CV_BGR2GRAY);
	cvtColor(src1, src_gray, CV_BGR2GRAY);

	/// Gradient X
	//Scharr(src_gray, grad_x, ddepth, 1, 0, scale, delta, BORDER_DEFAULT);
	Scharr(src_gray, src_grad_x, ddepth, 1, 0, scale, delta, BORDER_DEFAULT);
	Scharr(tmpl_gray, tmpl_grad_x, ddepth, 1, 0, scale, delta, BORDER_DEFAULT);
	convertScaleAbs(src_grad_x, src_abs_grad_x);
	convertScaleAbs(tmpl_grad_x, tmpl_abs_grad_x);


	/// Gradient Y
	//Scharr(src_gray, grad_y, ddepth, 0, 1, scale, delta, BORDER_DEFAULT);
	Scharr(src_gray, src_grad_y, ddepth, 0, 1, scale, delta, BORDER_DEFAULT);
	Scharr(tmpl_gray, tmpl_grad_y, ddepth, 0, 1, scale, delta, BORDER_DEFAULT);
	convertScaleAbs(src_grad_y, src_abs_grad_y);
	convertScaleAbs(tmpl_grad_y, tmpl_abs_grad_y);

	/// Total Gradient (approximate)
	Mat src_grad, tmpl_grad;
	addWeighted(src_abs_grad_x, 0.5, src_abs_grad_y, 0.5, 0, src_grad);
	addWeighted(tmpl_abs_grad_x, 0.5, tmpl_abs_grad_y, 0.5, 0, tmpl_grad);


	int newrows = floor(tmpl1.rows * rat + 0.5);
	int newcols = floor(tmpl1.cols * rat + 0.5);
	cv::resize(tmpl_grad, tmpl_grad, cv::Size(newcols, newrows), 0, 0, CV_INTER_LINEAR);

	Mat result;
	int result_rows = src_grad.rows - tmpl_grad.rows + 1;
	int result_cols = src_grad.cols - tmpl_grad.cols + 1;
	result.create(result_rows, result_cols, CV_32FC1);

	im_finds.push_back(src_grad);
	im_finds.push_back(tmpl_grad);
	im_finds.push_back(result);

	return im_finds;
}




Mat find_general_roi(Mat src, std::vector <imageandindx> TableOfObjectsCrops)
{

	Mat dst(src.rows, src.cols, CV_8UC1, Scalar(0));
	Mat tmp(src.rows, src.cols, CV_8UC1, Scalar(0));
	Mat dst2(src.rows, src.cols, CV_8UC1, Scalar(0));


	Mat luv, all_edges;

	cvtColor(src, luv, CV_BGR2Luv);
	cvtColor(luv, all_edges, CV_BGR2GRAY);
	Canny(all_edges, all_edges, 5, 5, 3); // (funciona bem com 15, 10) //30 é bom, mas aumanta o numero de matchs


	for (int i = 0; i < TableOfObjectsCrops.size(); i++)
	{
		for (int y = TableOfObjectsCrops[i].R.y; y < (TableOfObjectsCrops[i].R.y + TableOfObjectsCrops[i].R.height); y++)
		{
			for (int x = TableOfObjectsCrops[i].R.x; x < (TableOfObjectsCrops[i].R.x + TableOfObjectsCrops[i].R.width); x++)
			{
				tmp.at<uchar>(y, x) = 255;
			}
		}
	}


	Mat element = getStructuringElement(0, Size(2 * 1 + 1, 2 * 1 + 1), Point(2, 2));
	dilate(all_edges, all_edges, element);
	dilate(all_edges, all_edges, element);
	dilate(all_edges, all_edges, element);

	for (int y = 0; y < dst.rows; y++)
	{
		for (int x = 0; x < dst.cols; x++)
		{
			if (all_edges.at<uchar>(y, x) && tmp.at<uchar>(y, x))
				dst.at<uchar>(y, x) = 255;
		}
	}


	return dst;
}




std::vector<imageandindx> getExpandCropRegions(Mat img, std::vector<Mat> ConectedRegions, int Dx, int Dy)
{

	std::vector<imageandindx> Crops;

	for (int i = 0; i < ConectedRegions.size(); i++) {
		Rect R = boundingRect(ConectedRegions[i]);
		if (R.area() != ConectedRegions[i].rows*ConectedRegions[i].cols)
		{
			R.x = R.x - Dx;  if (R.x < 0) R.x = 0;
			R.y = R.y - Dy;  if (R.y < 0) R.y = 0;
			R.width = R.width + 2 * Dx;  if ((R.x + R.width) > img.cols) R.width = img.cols - R.x;
			R.height = R.height + 2 * Dy;  if ((R.y + R.height) > img.rows) R.height = img.rows - R.y;

			imageandindx Crop;

			Crop.img_crop = img(R);
			Crop.R.x = R.x;
			Crop.R.y = R.y;
			Crop.R.width = R.width;
			Crop.R.height = R.height;
			Crops.push_back(Crop);
		}
	}

	return Crops;
}



std::vector<Mat> SplitRegions2(Mat im, int flag, double precision)
{
	Mat imroi(im.size(), CV_8UC1);
	int nLabels = connectedComponents(im, imroi, 8);

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


	for (int r = 0; r < nLabels; r++) {
		double pnpixels = (double)NumPixelsRegions[r] / (im.rows * im.cols);
		if (pnpixels >= precision) {
			Regioes.push_back(ImRegions[r]);
		}
	}

	return Regioes;
}



std::vector<imageandindx> object_recognition_first_evaluation(Mat img)
{

	std::vector<imageandindx> Crops;

	Mat luv, gry;

	cvtColor(img, luv, CV_BGR2Luv);
	cvtColor(luv, gry, CV_BGR2GRAY);
	Canny(gry, gry, 5, 100, 3);

	//imshow("canny", gry);

	Mat element = getStructuringElement(0, Size(2 * 1 + 1, 2 * 1 + 1), Point(2, 2));
	dilate(gry, gry, element);
	
	//erode(gry, gry, element);

	imshow("morph", gry);
	waitKey(0);
	//exit(0);

	std::vector<Mat> Imregioes = SplitRegions(gry, 1, 0);

	int Dx = 120; int Dy = 40;
	Crops = getExpandCropRegions(img, Imregioes, Dx, Dy);

	return Crops;
}




// this function load 2 internal instance viwers for each object and insert it in a vector list 
// in the future, it can be adapted to load specific viwers
std::vector <std::vector <Mat>> loadtemplatepartslist(string mainpathparts)
{
	std::vector <std::vector <Mat>> TemplatePartsList;

	// for each object
	for (int i = 0; i < NUMOBJS; i++)
	{
		std::vector <Mat> tmp0;
		TemplatePartsList.push_back(tmp0);

		// choose 2 object internal viwers
		for (int j = 1; j <= 5; j++)
		{
			// set object parts path
			string objectpartspath = mainpathparts + NEW_OBJECTS_DIR_NAMES[i] + "_c" + std::to_string(j) + ".png";
			// loading 
			Mat im1 = imread(objectpartspath);

			// save on template list 
			TemplatePartsList[i].push_back(im1);
		}
	}

	return TemplatePartsList;
}



// this function load 10 instance viwers for each object and insert it in a vector list 
// in the future, it can be adapted to load specific viwers
std::vector <std::vector <Mat>> loadtemplatelist(string maintmpl)
{
	// temporary template list
	std::vector <std::vector <Mat>> TemplateList;

	// for each object
	for (int i = 0; i < NUMOBJS; i++)
	{
		std::vector <Mat> tmp0;
		TemplateList.push_back(tmp0);

		// choose ten object viwers
		for (int j = 1; j < 100; j = j + 10)
		{
			// set object path
			string objectpath = maintmpl + NEW_OBJECTS_DIR_NAMES[i] + "\\" + NEW_OBJECTS_DIR_NAMES[i] + std::to_string(j) + ".png";

			// loading 
			Mat im1 = imread(objectpath);
			if (j != 1)
				resize(im1, im1, TemplateList[i][0].size());
			// save on template list 
			TemplateList[i].push_back(im1);
		}
	}

	return TemplateList;
}

std::vector<FoundObject> create_next_object_list(std::vector<FoundObject> TPListAnterior, Mat scn0, std::vector <std::vector <Mat>> TemplateList, 
																				std::vector <std::vector <Mat>> TemplatePartsList, int reserva) {
	std::vector<FoundObject> TPList;

	for (int i = 0; i < TPListAnterior.size(); i++){
		if (TPListAnterior[i].firstValid == 1) {
			TPList.push_back(TPListAnterior[i]);
			int x0 = TPListAnterior[i].Crop.x;
			int y0 = TPListAnterior[i].Crop.y;
			int L0 = TPListAnterior[i].Crop.width;
			int A0 = TPListAnterior[i].Crop.height;
			
			TPList[TPList.size()-1].Crop.x = x0 - delX;
			TPList[TPList.size()-1].Crop.y = y0 - delY;
			TPList[TPList.size()-1].Crop.width = L0 + 2 * delX;
			TPList[TPList.size()-1].Crop.height = A0 + 2 * delY;
			TPList[TPList.size()-1].valid = 0;
			TPList[TPList.size() - 1].img = TPListAnterior[i].img;
			
			/*imshow("template",TPListAnterior[i].img);
			waitKey();
			cvDestroyAllWindows();*/
		}
	}

	for (int i = 0; i < TPList.size(); i++) {
		//Mat tmpl0 = (TemplateList[TPList[i].obj])[TPList[i].indTmpl];
		Mat tmpl0 = TPList[i].img;
		double rat1;
		Mat src1;
		scn0.copyTo(src1);
		bool is_inside = (TPList[i].Crop & cv::Rect(0, 0, src1.cols, src1.rows)) == TPList[i].Crop; //verifica se o retangulo do Crop cabe dentro da img
		if (!is_inside) {
			if (TPList[i].Crop.x < src1.cols)
				TPList[i].Crop.x = 0;
			if (TPList[i].Crop.x + TPList[i].Crop.width > src1.cols)
				TPList[i].Crop.width = src1.cols - TPList[i].Crop.x;
			if(TPList[i].Crop.y < src1.rows)
				TPList[i].Crop.y = 0;
			if (TPList[i].Crop.y + TPList[i].Crop.height > src1.rows)
				TPList[i].Crop.height = src1.rows - TPList[i].Crop.y;
			
		}
		
		src1 = src1(TPList[i].Crop);
			

		Mat tmpl1;
		tmpl0.copyTo(tmpl1);


		double maxVal1; Point maxLoc1; rat1 = 1;//RAT[TPList[i].r];
		mathTemplateEdge(src1, tmpl1, rat1, &maxVal1, &maxLoc1, Mat());
		
		Rect R1;
		R1.x = maxLoc1.x + TPList[i].Crop.x;
		R1.y = maxLoc1.y + TPList[i].Crop.y;
		R1.width = (int)round(tmpl1.cols * rat1);
		R1.height = (int)round(tmpl1.rows * rat1);
		

		if (maxVal1 > 0.87) {
			TPList[i].valid = 1;
			TPList[i].Crop = R1;
			is_inside = (TPList[i].Crop & cv::Rect(0, 0, scn0.cols, scn0.rows)) == TPList[i].Crop; //verifica se o retangulo do Crop cabe dentro da img
			if (!is_inside) {
				if (TPList[i].Crop.x + TPList[i].Crop.width > scn0.cols)
					TPList[i].Crop.width = scn0.cols - TPList[i].Crop.x;
				if (TPList[i].Crop.x < scn0.cols)
					TPList[i].Crop.x = 0;
				if (TPList[i].Crop.y + TPList[i].Crop.height > scn0.rows)
					TPList[i].Crop.height = scn0.rows - TPList[i].Crop.y;
				if (TPList[i].Crop.y < scn0.rows)
					TPList[i].Crop.y = 0;
			}
			TPList[i].img = scn0(TPList[i].Crop);
		}
		/*else {
			TPList[i].Crop.x = TPList[i].Crop.x - (delX/2);
			TPList[i].Crop.y = TPList[i].Crop.y - (delY/2);
			TPList[i].Crop.width = TPList[i].Crop.width + 2 * delX;
			TPList[i].Crop.height = TPList[i].Crop.height + 2 * delY;
		}*/
	}

	return TPList;
}

void check_inside(std::vector <FoundObject> &TPList) {
	for (int i = 0; i < TPList.size(); i++) {
		if (TPList[i].valid) {
			for (int j = 0; j < TPList.size(); j++) {
				if (TPList[j].valid) {
					if (i != j) {
						if ((TPList[i].Crop & TPList[j].Crop).area() > 0) {
							if (TPList[i].T > TPList[j].T)
								TPList[j].valid = 0;
							else
								TPList[i].valid = 0;
						}
					}
				}
			}
		}
	}
}

void first_avaliation(Mat scn0, Mat ROImask, std::vector <std::vector <Mat> > TemplateList, std::vector <std::vector <Mat> > TemplatePartsList, std::vector <FoundObject> &TPList) {

	
	for (int i = 0; i < NUMOBJS; i++)
	{
		Mat tmpl0;
		std::vector  <SceneLoc> ListOfValidScales;
		if (!TPList[i].valid) {
			// le um template
			for (int t = 0; t < TemplateList[i].size(); t++) {
				tmpl0 = (TemplateList[i])[t];
				//SceneLoc winCrop;
				//double winVal = -1000, maxVal;
				double rat1, rat2, rat3, rat4, rat5;
				SceneLoc SL;
				for (int r = 0; r < maxRAT; r++)
				{
					// first search: whole scene under mask based on gradient Scharr edge
					Mat src1;
					scn0.copyTo(src1);

					Mat tmpl1;
					tmpl0.copyTo(tmpl1);

					Mat mask1;
					ROImask.copyTo(mask1);

					double maxVal1; Point maxLoc1; rat1 = RAT[r];
					mathTemplateEdge(src1, tmpl1, rat1, &maxVal1, &maxLoc1, mask1);

					Rect R1;
					R1.x = maxLoc1.x;
					R1.y = maxLoc1.y;
					R1.width = (int)round(tmpl1.cols * RAT[r]);
					R1.height = (int)round(tmpl1.rows * RAT[r]);
					// second search: internal crop based on Scharr space
					Mat src2 = scn0(R1);
					Mat tmpl2 = (TemplatePartsList[i])[0];

					double maxVal2; Point maxLoc2;

					if (src2.rows > src2.cols)
						rat2 = (double)src2.rows / tmpl1.rows;
					else
						rat2 = (double)src2.cols / tmpl1.cols;

					mathTemplateEdge(src2, tmpl2, rat2, &maxVal2, &maxLoc2, Mat());

					// third search: internal crop based on Scharr space
					Mat src3 = scn0(R1);
					Mat tmpl3 = (TemplatePartsList[i])[1];

					double maxVal3; Point maxLoc3;

					if (src3.rows > src3.cols)
						rat3 = (double)src3.rows / tmpl1.rows;
					else
						rat3 = (double)src3.cols / tmpl1.cols;

					mathTemplateEdge(src3, tmpl3, rat3, &maxVal3, &maxLoc3, Mat());

					// fourth search: internal crop based on Luv color space
					Mat src4 = scn0(R1);
					Mat tmpl4 = (TemplatePartsList[i])[2];

					double maxVal4; Point maxLoc4;

					if (src4.rows > src4.cols)
						rat4 = (double)src4.rows / tmpl1.rows;
					else
						rat4 = (double)src4.cols / tmpl1.cols;

					//mathTemplateColor(src4, tmpl4, rat4, &maxVal4, &maxLoc4, Mat());
					mathTemplateEdge(src4, tmpl4, rat4, &maxVal4, &maxLoc4, Mat());

					SL.C.x = R1.x;
					SL.C.y = R1.y;
					SL.C.width = R1.width;
					SL.C.height = R1.height;
					SL.maxVal1 = maxVal1;
					SL.maxVal2 = maxVal2;
					SL.maxVal3 = maxVal3;
					SL.maxVal4 = maxVal4;
					SL.maxVal5 = 0; // maxVal5;

					ListOfValidScales.push_back(SL);
				}
			}
			TPList[i] = Individual_evolution(scn0, tmpl0, (TemplatePartsList[i])[1], ListOfValidScales, i);
			ListOfValidScales.clear();
			
		}
	}
}
void find_object_frame2(Mat scn0, std::vector <std::vector <Mat> > TemplateList, std::vector <std::vector <Mat> > TemplatePartsList, std::vector <FoundObject> &TPList)
{
	/*
	// first phase: generating several scene crops where there is a high probability to find an object
	cout << "Phase 1/4: generating crops" << endl;
	std::vector<imageandindx> TableOfObjectsCrops;
	TableOfObjectsCrops = object_recognition_first_evaluation(scn0);
	cout << "end of phase 1/4" << endl << endl;


	// second phase: crop expandings
	cout << "Phase 2/4: crop expandings" << endl;
	Mat ROImask = find_general_roi(scn0, TableOfObjectsCrops);
	imshow("mascara", ROImask);
	waitKey();
	cout << "end of phase 2/4" << endl << endl;*/
	Mat scn1(scn0.rows, scn0.cols, CV_8UC1, Scalar(0));

	for (int i = 0; i < TPList.size(); i++)
	{
		if (TPList[i].valid)
		{
			for (int y = 0; y < scn1.rows; y++)
				for (int x = 0; x < scn1.cols; x++)
				{
					if ((y >= TPList[i].Crop.y) && (y <= TPList[i].Crop.y + TPList[i].Crop.height) &&
						(x >= TPList[i].Crop.x) && (x <= TPList[i].Crop.x + TPList[i].Crop.width))
					{
						scn1.at<uchar>(y, x) = 255;
					}
				}
		}
	}
	Mat mask = buildmask(scn1);
	//imshow("mask", mask);
	//waitKey(1);
	// third phase: template matchings
	cout << "Phase(2) 3/4: template matching" << endl;

	first_avaliation(scn0, mask, TemplateList, TemplatePartsList, TPList);

	cout << "end of phase 3/4" << endl << endl;
	destroyAllWindows();
	cout << "Phase 4/5: Looking for hard objects" << endl;
	//TPList = Remainder_Evolution(scn0, TPList, TemplateList, TemplatePartsList);
	cout << "end of phase 4/5" << endl << endl;

	cout << "Phase 5/5: Looking for a bit more objects" << endl;
	//TPList = More_Evolution(scn0, TPList, TemplateList, TemplatePartsList);
	cout << "end of phase 5/5" << endl << endl;

}
std::vector <FoundObject>  find_object_frame(Mat scn0, std::vector <std::vector <Mat> > TemplateList, std::vector <std::vector <Mat> > TemplatePartsList)
{

	// first phase: generating several scene crops where there is a high probability to find an object
	cout << "Phase 1/4: generating crops" << endl;
	std::vector<imageandindx> TableOfObjectsCrops;
	TableOfObjectsCrops = object_recognition_first_evaluation(scn0);
	cout << "end of phase 1/4" << endl << endl;


	// second phase: crop expandings
	cout << "Phase 2/4: crop expandings" << endl;
	Mat ROImask = find_general_roi(scn0, TableOfObjectsCrops);
	imshow("mascara", ROImask);
	waitKey(1);
	cout << "end of phase 2/4" << endl << endl;

	// third phase: template matchings
	cout << "Phase 3/4: template matching" << endl;

	std::vector <FoundObject> TPList;
	for (int i = 0; i < NUMOBJS; i++)
	{
		FoundObject FO;
		FO.valid = false;
		TPList.push_back(FO);
	}
	first_avaliation(scn0, ROImask, TemplateList, TemplatePartsList, TPList);
	
	cout << "end of phase 3/4" << endl << endl;
	destroyAllWindows();
	cout << "Phase 4/5: Looking for hard objects" << endl;
	//TPList = Remainder_Evolution(scn0, TPList, TemplateList, TemplatePartsList);
	cout << "end of phase 4/5" << endl << endl;

	cout << "Phase 5/5: Looking for a bit more objects" << endl;
	//TPList = More_Evolution(scn0, TPList, TemplateList, TemplatePartsList);
	cout << "end of phase 5/5" << endl << endl;
	
	return TPList;
	
}



int main()
{
    // count time
	clock_t t;
	t = clock();
	srand(NULL);
	//path for initial frame
	string mainpath = "C:\\Users\\muril\\Pictures\\prateleira.png";

	//path for video
	string vidpath = "C:\\Users\\muril\\Videos\\VideoPrateleira1.mp4";

	//path for the set of object templates 
	string maintmpl = "C:\\Users\\muril\\Pictures\\OBJECTS_DATABSE\\POSITIVOS\\";

	//path for the sub-set of parts of object templates
	string pathparts = "C:\\Users\\muril\\Pictures\\OBJECTS_DATABSE\\PARTS\\";

	//load a set of template lernnings: this is a vector of Mat vectors
	std::vector <std::vector <Mat>> TemplateList = loadtemplatelist(maintmpl);

	//load a set of template internal part lernnings: this also is a vector of Mat vectors
	std::vector <std::vector <Mat>> TemplatePartsList = loadtemplatepartslist(pathparts);

	//load the main scene
	Mat scn0 = imread(mainpath);

	//load the video
	VideoCapture cap(1);

	int cont = 0;
	std::vector <FoundObject>  TPListAnterior;
	std::vector <FoundObject>  TPListAtual;
	for (;;) {
		cont++;
		Mat frame;
		cap >> frame;
		Mat resframe;
		int newcol = (int)round(frame.cols*(0.6));
		int newrow = (int)round(frame.rows*(0.6));
		if (cont == 151) {
			frame.copyTo(resframe);
			t = clock();
			//resize(resframe, resframe, Size(newcol,newrow));
			//imshow("video", resframe);
			//waitKey();
			TPListAtual = find_object_frame(resframe,TemplateList,TemplatePartsList);
			check_inside(TPListAtual);
			t = clock() - t;
			cout << "Found " << TPListAtual.size() << " objects in " << t << " clicks (" << ((float)t) / CLOCKS_PER_SEC << " seconds)." << endl << endl;
			cout << "show results" << endl;
			showFinalResult(resframe, TPListAtual);
			TPListAnterior = TPListAtual;
			waitKey(1);
		}
		else if (cont == 152) {
			frame.copyTo(resframe);
			//resize(resframe, resframe, Size(newcol,newrow));
			//imshow("video", resframe);
			//waitKey();
			t = clock();
			find_object_frame2(resframe, TemplateList, TemplatePartsList,TPListAtual);
			check_inside(TPListAtual);
			for (int i = 0; i < TPListAtual.size(); i++) {
				if (TPListAtual[i].valid != TPListAnterior[i].valid) {
					cont--;
					break;
				}
			}
			
			t = clock() - t;
			cout << "Found " << TPListAtual.size() << " objects in " << t << " clicks (" << ((float)t) / CLOCKS_PER_SEC << " seconds)." << endl << endl;
			cout << "show results" << endl;
			showFinalResult(resframe, TPListAtual);
			TPListAnterior = TPListAtual;
			waitKey(1);
		}
		else if(cont > 152) {
			frame.copyTo(resframe);
			//resize(resframe, resframe, Size(newcol, newrow));
			cout << "Validando objetos do proximo frame...\n";
			TPListAtual = create_next_object_list(TPListAnterior, resframe, TemplateList, TemplatePartsList,cont);
			check_inside(TPListAtual);
			//cout << "Ok\n";
			t = clock() - t;
			cout << "Found " << TPListAtual.size() << " objects in " << t << " clicks (" << ((float)t) / CLOCKS_PER_SEC << " seconds)." << endl << endl;
			//cout << "show results" << endl;
			showFinalResult(resframe, TPListAtual);
			TPListAnterior = TPListAtual;

			waitKey(1);
		}
	}
	
	return 0;
}


