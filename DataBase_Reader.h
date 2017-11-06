

#ifndef ATHOMEBRAHMATREINO
#define ATHOMEBRAHMATREINO


#include "AtHomeConfig.h"


using namespace std;
using namespace cv;



Mat treatimage(Mat src);


Mat get_hogdescriptor_visual_image(double GRADIENT_FEATURES[], Mat& origImg,
	std::vector<float>& descriptorValues,
	Size winSize,
	Size cellSize,
	int scaleFactor,
	double viz_factor);

void obtem_featuresCor(double FEATURES[], Mat img);
void obtem_featuresMat(double FEATURES[], Mat img);
void obtem_featuresSplitV02(double FEATURES_COR[], double FEATURES_GRAD[], int FEATURES_EDGE[], Mat img);
void obtem_featuresSplit(double FEATURES_COR[], double FEATURES_GRAD[], double FEATURES_GRAY[], Mat img);
void getHSVSplitHistogram1(Mat img, double HSVSplit);
void getColorBlockHistogram(Mat img1, double CBHIST[]);
void getEdgeBlockHistogram(Mat img1, int EBHIST[]);
void getHSVRegions(Mat &hsv, int row1, int col1, int idx, int idy, int &total_pixels, double HSV_Split);
void get_database_space_vector(string featuredata, int dimen, int feature[]);

std::vector <Mat> readTrueCrops(int ind);


void getHSVRegions(Mat &hsv, int row1, int col1, int idx, int idy, int &total_pixels, double HSV_Split[])
{

	int DIMENSAO_CORN = (int)(DIMENSAO_COR / (NREGIONS_X * NREGIONS_Y));

	for (int y = idy*row1; y < (idy + 1)*row1; y++)
		for (int x = idx*col1; x < (idx + 1)*col1; x++)
		{
			Vec3b intensity = hsv.at<Vec3b>(y, x);

			int Hn = floor((((int)intensity.val[0] / 179.0)*(Hbin - 1)) + 0.5);
			int Sn = floor((((int)intensity.val[1] / 255.0)*(Sbin - 1)) + 0.5);
			int Vn = floor((((int)intensity.val[2] / 255.0)*(Vbin - 1)) + 0.5);

			int ind = ((idy*NREGIONS_X) + idx)*DIMENSAO_CORN + (Sbin*Vbin) * Hn + Vbin * Sn + Vn;
			HSV_Split[ind] = HSV_Split[ind] + 1.0;
			total_pixels = total_pixels + 1;
		}

	for (int i = ((idy*NREGIONS_X) + idx)*DIMENSAO_CORN; i < ((idy*NREGIONS_X) + idx + 1)*DIMENSAO_CORN; i++) {
		HSV_Split[i] = HSV_Split[i] / (double)total_pixels;

	}
}




void getEdgeBlockHistogram(Mat img1, int EBHIST[])
{

	Mat img11;
	img1.copyTo(img11);

	Mat img1_gry;
	cvtColor(img11, img1_gry, CV_BGR2GRAY);

	int newrows = MATCH_SIZE;
	int newcols = MATCH_SIZE;
	cv::resize(img1_gry, img1_gry, cv::Size(newcols, newrows), 0, 0, CV_INTER_LINEAR);

	Canny(img1_gry, img1_gry, 5, 150, 3);

	int pixelindx = 0;
	for (int y = 0; y < newrows; y++)
		for (int x = 0; x < newcols; x++)
		{
			EBHIST[pixelindx] = (int)img1_gry.at<uchar>(y, x);
			pixelindx++;
		}
}



void getColorBlockHistogram(Mat img1, double CBHIST[])
{

	int bloc_cor = 1;

	// constante para dimensao de blocos

	// converter a imagem 1 pra multiplo de b, reescaloando-a
	int lin1 = img1.rows;
	int col1 = img1.cols;
	float Fl1 = bloc_cor * floor(lin1 / bloc_cor) / lin1;
	float Fc1 = bloc_cor * floor(col1 / bloc_cor) / col1;
	int newlin1 = Fl1 * lin1;
	int newcol1 = Fc1 * col1;

	// reescalonamento
	Mat img1r;
	cv::resize(img1, img1r, cv::Size(newcol1, newlin1), 0, 0, CV_INTER_LINEAR);

	// converte ambas as imagens para HSV
	Mat hsv1;
	cvtColor(img1r, hsv1, CV_BGR2Luv);

	// configura a discretizacao de cada canal
	//const int Cdisc = 5;
	//const int Cdisc = 5;
	//const int Cdisc = 5;

	// define a dimensao final do histograma extendido
	const int DIM = C1 * C2 * C3;
	const int DIM_EXP = DIMENSAO_COR;

	// garante que estejam zerados
	for (int i = 0; i < DIM_EXP; i++) CBHIST[i] = 0;


	// calcula histograma da imagem 1, um para cada bloco
	int y1 = 0; int y2 = newlin1 / bloc_cor;
	int HEXP = 0;
	for (int j = 0; j < bloc_cor; j++)
	{
		int x1 = 0; int x2 = newcol1 / bloc_cor;
		for (int i = 0; i < bloc_cor; i++)
		{
			Vec3b intensity;
			int total_pixels1 = 0;
			// calcula o histograna para 1 bloco
			for (int y = y1; y < y2; y++) {
				for (int x = x1; x < x2; x++)
				{
					intensity = hsv1.at<Vec3b>(y, x);
					int C1n = 0; // floor(((float)intensity.val[0] / 255.0) * (C1 - 1) + 0.5);
					int C2n = floor(((float)intensity.val[1] / 255.0) * (C2 - 1) + 0.5);
					int C3n = floor(((float)intensity.val[2] / 255.0) * (C3 - 1) + 0.5);

					int indx1 = C2 * C3 * C1n + C3 * C2n + C3n;
					CBHIST[HEXP + indx1] = CBHIST[HEXP + indx1] + 1.0;
					total_pixels1 = total_pixels1 + 1;
				}
			}

			for (int i = HEXP; i < DIM + HEXP; i++)  CBHIST[i] = CBHIST[i] / (float)total_pixels1;
			x1 = x2;
			x2 = x2 + newcol1 / bloc_cor;
			HEXP = HEXP + DIM;
		}
		y1 = y2;
		y2 = y2 + newlin1 / bloc_cor;
	}

}



void getHSVSplitHistogram1(Mat img, double HSV_Split[])
{

	int x, y;
	Mat src = img;
	Mat hsv;

	int total_pixels;

	/// Convert the image to HSV
	cvtColor(src, hsv, CV_BGR2HSV);

	int row1 = (int)(hsv.rows / NREGIONS_Y);
	int cols1 = (int)(hsv.cols) / NREGIONS_X;
	int DIMENSAO_CORN = (int)(DIMENSAO_COR / (NREGIONS_X * NREGIONS_Y));

	for (int i = 0; i < NREGIONS_Y; i++) {
		for (int j = 0; j < NREGIONS_X; j++) {
			total_pixels = 0;
			getHSVRegions(hsv, row1, cols1, j, i, total_pixels, HSV_Split);
		}
	}

}




// faz a mesma coisa, só que com MAT
void obtem_featuresSplit(double FEATURES_COR[], double FEATURES_GRAD[], double FEATURES_GRAY[], Mat img)
{
	int i, x, y;

	Mat src = img;
	Mat hsv;

	/////  PRIMEIRA CARACTERISTICA 
	// EXTRAI CARACTERISICAS DE COR - HISTOGRAMA HSV162
	double HSV_hist[DIMENSAO_COR];
	for (i = 0; i < DIMENSAO_COR; i++)  HSV_hist[i] = 0.0;
	getHSVSplitHistogram1(img, HSV_hist);


	// SEGUNDA CARACTERISTICA
	// get intensity values feature
	Mat gry(img.cols, img.rows, CV_8UC1); // img é uma imagem original 3 canais E gry é do mesmo tamanho de 1 canal
	cvtColor(img, gry, CV_RGB2GRAY); // converte gry para tons de cinza
	float HIST_GRAY[DIMENSAO_GRAY];  // histograma em tons de cinza
	for (i = 0; i < DIMENSAO_GRAY; i++)  HIST_GRAY[i] = 0.0;  // zera o histograma


															  // essa linha pode ser removida se desejar-se nao tratar a imagem
	gry = treatimage(gry);

	// esse bloco calcula o histograma em tons de cinza
	int pixelvalue;
	for (y = 0; y < gry.rows; y++)
		for (x = 0; x < gry.cols; x++)
		{
 		    pixelvalue = (int)gry.at<uchar>(y, x); // 
			HIST_GRAY[pixelvalue] = HIST_GRAY[pixelvalue] + 1.0;
			//cout << pixelvalue << " " << endl;
		}

	int total_pixels = gry.rows * gry.cols;
	for (i = 0; i < DIMENSAO_GRAY; i++) HIST_GRAY[i] = HIST_GRAY[i] / (float)total_pixels; // normaliza

																						   /////  TERCEIRA CARACTERISTICA 
																						   // EXTRAI CARACTERISICAS DE GRADIENTE - HOG
																						   /// reescalona a imagem para poder ser usado o HOG
	resize(src, src, Size(64, 128));

	Mat gry_original(src.cols, src.rows, CV_8UC1);
	Mat gry_treated(src.cols, src.rows, CV_8UC1);

	cvtColor(src, gry_original, CV_RGB2GRAY);

	gry_treated = treatimage(gry_original);



	// EXTRAI CARACTERÍSTICAS DE TONS DE CINZA

	HOGDescriptor d;
	// Size(64,128), //winSize
	// Size(16,16), //blocksize
	// Size(8,8), //blockStride,
	// Size(8,8), //cellSize,
	// 9, //nbins,
	// 0, //derivAper,
	// -1, //winSigma,
	// 0, //histogramNormType,
	// 0.2, //L2HysThresh,x
	// 0 //gammal correction,
	// //nlevels=64
	//);

	// void HOGDescriptor::compute(const Mat& img, vector<float>& descriptors,
	//                             Size winStride, Size padding,
	//                             const vector<Point>& locations) const

	std::vector<float> descriptorsValues;
	std::vector<Point> locations;


	d.compute(gry_treated, descriptorsValues, Size(0, 0), Size(0, 0), locations);

	double GRADIENT_FEATURES[DIMENSAO_GRAD];
	Mat visu = get_hogdescriptor_visual_image(GRADIENT_FEATURES, gry_treated, descriptorsValues, Size(winsizeX, winsizeY), Size(cellsize, cellsize), 3, 3);



	// preenche a cor
	for (i = 0; i < DIMENSAO_COR; i++)   FEATURES_COR[i] = HSV_hist[i];

	//preenche gradiente
	for (i = 0; i < DIMENSAO_GRAD; i++)  FEATURES_GRAD[i] = GRADIENT_FEATURES[i];

	// preenche tons de cinza
	for (i = 0; i < DIMENSAO_GRAY; i++)  FEATURES_GRAY[i] = HIST_GRAY[i];

}



std::vector <Mat> readTrueCrops(int ind)
{
	std::vector <Mat> ListTrueCrops;

	string main_true_path_crop = "C:\\Users\\Paulo\\Dropbox\\Imagedatabase\\AtHomeDatabase\\OBJECTS_DATABASE\\PARTS\\";

	for (int i = 0; i < NUM_TRUE_CROPS; i++)
	{
		string sigle_path = main_true_path_crop + NEW_OBJECTS_DIR_NAMES[ind] + "_c" + std::to_string(i+1) + ".png";
		//cout << sigle_path << endl;
		//system("pause");
		Mat img = imread(sigle_path);
		ListTrueCrops.push_back(img);
	}
	return ListTrueCrops;
}



// Nova versao de objetm-featureSplit, agora com EDGE no lugar de GRAY e histogramas em bloco das imagens
void obtem_featuresSplitV02(double FEATURES_COR[], double FEATURES_GRAD[], int FEATURES_EDGE[], Mat img)
{
	int i, x, y;

	Mat src = img;
	Mat hsv;

	/////  PRIMEIRA CARACTERISTICA 
	// EXTRAI CARACTERISICAS DE COR - HISTOGRAMA HSV162
	double CBHIST[DIMENSAO_COR];
	for (i = 0; i < DIMENSAO_COR; i++)  CBHIST[i] = 0.0;
	getColorBlockHistogram(img, CBHIST);

	int EBHIST[DIMENSAO_EDGE];  // histograma em tons de cinza
	getEdgeBlockHistogram(img, EBHIST);

	/*
	// SEGUNDA CARACTERISTICA
	// get intensity values feature
	Mat gry(img.cols, img.rows, CV_8UC1); // img é uma imagem original 3 canais E gry é do mesmo tamanho de 1 canal
	cvtColor(img, gry, CV_RGB2GRAY); // converte gry para tons de cinza
	float HIST_GRAY[DIMENSAO_GRAY];  // histograma em tons de cinza
	for (i = 0; i < DIMENSAO_GRAY; i++)  HIST_GRAY[i] = 0.0;  // zera o histograma


															  // essa linha pode ser removida se desejar-se nao tratar a imagem
	gry = treatimage(gry);

	// esse bloco calcula o histograma em tons de cinza
	int pixelvalue;
	for (y = 0; y < gry.rows; y++)
		for (x = 0; x < gry.cols; x++)
		{
			pixelvalue = (int)gry.at<uchar>(y, x); // 
			HIST_GRAY[pixelvalue] = HIST_GRAY[pixelvalue] + 1.0;
			//cout << pixelvalue << " " << endl;
		}

	int total_pixels = gry.rows * gry.cols;
	for (i = 0; i < DIMENSAO_GRAY; i++) HIST_GRAY[i] = HIST_GRAY[i] / (float)total_pixels; // normaliza

																						   /////  TERCEIRA CARACTERISTICA 
																						   // EXTRAI CARACTERISICAS DE GRADIENTE - HOG
	
	*/
	
	/// reescalona a imagem para poder ser usado o HOG
	
	resize(src, src, Size(64, 128));

	Mat gry_original(src.cols, src.rows, CV_8UC1);
	Mat gry_treated(src.cols, src.rows, CV_8UC1);

	cvtColor(src, gry_original, CV_RGB2GRAY);

	gry_treated = treatimage(gry_original);


	// EXTRAI CARACTERÍSTICAS DE GRADIENT

	HOGDescriptor d;
	// Size(64,128), //winSize
	// Size(16,16), //blocksize
	// Size(8,8), //blockStride,
	// Size(8,8), //cellSize,
	// 9, //nbins,
	// 0, //derivAper,
	// -1, //winSigma,
	// 0, //histogramNormType,
	// 0.2, //L2HysThresh,x
	// 0 //gammal correction,
	// //nlevels=64
	//);

	// void HOGDescriptor::compute(const Mat& img, vector<float>& descriptors,
	//                             Size winStride, Size padding,
	//                             const vector<Point>& locations) const

	std::vector<float> descriptorsValues;
	std::vector<Point> locations;


	d.compute(gry_treated, descriptorsValues, Size(0, 0), Size(0, 0), locations);

	double GBHIST[DIMENSAO_GRAD];
	Mat visu = get_hogdescriptor_visual_image(GBHIST, gry_treated, descriptorsValues, Size(winsizeX, winsizeY), Size(cellsize, cellsize), 3, 3);



	// preenche a cor
	for (i = 0; i < DIMENSAO_COR; i++)   FEATURES_COR[i] = CBHIST[i];

	//preenche gradiente
	for (i = 0; i < DIMENSAO_GRAD; i++)  FEATURES_GRAD[i] = GBHIST[i];

	// preenche tons de cinza
    for (i = 0; i < DIMENSAO_GRAY; i++)  FEATURES_EDGE[i] = EBHIST[i];

}




// faz a mesma coisa, só que com MAT
void obtem_featuresCor(double FEATURES[], Mat src)
{
	int i, x, y;

	Mat img;
	src.copyTo(img);

	Mat luv;

	/// Convert the image to grayscale
	cv::cvtColor(img, luv, CV_BGR2Luv);

	/////  PRIMEIRA CARACTERISTICA 
	float LUV_hist[DIMENSAO_COR];

	for (i = 0; i < DIMENSAO_COR; i++)  LUV_hist[i] = 0.0;

	Vec3b intensity;
	int total_pixels = 0;
	
	for (int y = 0; y < img.rows; y++) {
	  for (int x = 0; x < img.cols; x++)
	  {
		 intensity = img.at<Vec3b>(y, x);
		 int Rn = floor(((float)intensity.val[0] / 255.0) * (C1 - 1) + 0.5);
		 int Gn = floor(((float)intensity.val[1] / 255.0) * (C2 - 1) + 0.5);
		 int Bn = floor(((float)intensity.val[2] / 255.0) * (C3 - 1) + 0.5);

		 int indx1 = C2 * C3 * Rn + C3 * Gn + Bn;
		 LUV_hist[indx1] = LUV_hist[indx1] + 1.0;
		 total_pixels = total_pixels + 1;
	  }
	}

	for (i = 0; i < DIMENSAO_COR; i++)  FEATURES[i] = LUV_hist[i] / (float)total_pixels;
}



// faz a mesma coisa, só que com MAT
void obtem_featuresMat(double FEATURES[], Mat img)
{
	int i, x, y;

	Mat src = img;
	Mat hsv;

	//cout << "src = " << src.cols << "col = " << src.rows << endl;

	//system("pause");
	
	/// Convert the image to grayscale
	cvtColor(img, hsv, CV_RGB2HSV);


	/////  PRIMEIRA CARACTERISTICA 
	// EXTRAI CARACTERISICAS DE COR - HISTOGRAMA HSV162
	float HSV_hist[DIMENSAO_COR];

	for (i = 0; i < DIMENSAO_COR; i++)  HSV_hist[i] = 0.0;

	Vec3b intensity;
	for (y = 0; y < hsv.rows; y++)
		for (x = 0; x < hsv.cols; x++)
		{
			intensity = hsv.at<Vec3b>(y, x);
			int Hn = ((float)intensity.val[0] / 179.0)*(float)Hbin;
			int Sn = ((float)intensity.val[1] / 255.0)*(float)Sbin;
			int Vn = ((float)intensity.val[2] / 255.0)*(float)Vbin;

			int indx = 9 * Hn + 3 * Sn + Vn;
			HSV_hist[indx] = HSV_hist[indx] + 1.0;
		}

	int total_pixels = hsv.rows * hsv.cols;
	for (i = 0; i < DIMENSAO_COR; i++)  HSV_hist[i] = HSV_hist[i] / (float)total_pixels;
	

	// SEGUNDA CARACTERISTICA
	// get intensity values feature
	Mat gry(img.cols, img.rows, CV_8UC1); // img é uma imagem original 3 canais E gry é do mesmo tamanho de 1 canal
	cvtColor(img, gry, CV_RGB2GRAY); // converte gry para tons de cinza
	float HIST_GRAY[DIMENSAO_GRAY];  // histograma em tons de cinza
	for (i = 0; i < DIMENSAO_GRAY; i++)  HIST_GRAY[i] = 0.0;  // zera o histograma


	// essa linha pode ser removida se desejar-se nao tratar a imagem
	gry = treatimage(gry);

	// esse bloco calcula o histograma em tons de cinza
	int pixelvalue;
	for (y = 0; y < gry.rows; y++)
		for (x = 0; x < gry.cols; x++)
		{
			pixelvalue = (int)gry.at<uchar>(y, x); // 
			HIST_GRAY[pixelvalue] = HIST_GRAY[pixelvalue] + 1.0;
		}

	total_pixels = gry.rows * gry.cols;
	for (i = 0; i < DIMENSAO_GRAY; i++) HIST_GRAY[i] = HIST_GRAY[i] / (float)total_pixels; // normaliza



	/////  TERCEIRA CARACTERISTICA 
	// EXTRAI CARACTERISICAS DE GRADIENTE - HOG
	/// reescalona a imagem para poder ser usado o HOG
	resize(src, src, Size(64, 128));

	Mat gry_original(src.cols, src.rows, CV_8UC1);
	Mat gry_treated(src.cols, src.rows, CV_8UC1);

	cvtColor(src, gry_original, CV_RGB2GRAY);
	//cvtColor(src, gry_treated, CV_RGB2GRAY);

	//namedWindow("original", 4);
	//imshow("original", gry_original);
	
	gry_treated = treatimage(gry_original);

	//namedWindow("treated", 4);
	//imshow("treated", gry_treated);

	//waitKey(0);

	// EXTRAI CARACTERÍSTICAS DE TONS DE CINZA
	/// Convert the image to grayscale

	HOGDescriptor d;
	// Size(64,128), //winSize
	// Size(16,16), //blocksize
	// Size(8,8), //blockStride,
	// Size(8,8), //cellSize,
	// 9, //nbins,
	// 0, //derivAper,
	// -1, //winSigma,
	// 0, //histogramNormType,
	// 0.2, //L2HysThresh,x
	// 0 //gammal correction,
	// //nlevels=64
	//);

	// void HOGDescriptor::compute(const Mat& img, vector<float>& descriptors,
	//                             Size winStride, Size padding,
	//                             const vector<Point>& locations) const

	std::vector<float> descriptorsValues;
	std::vector<Point> locations;

	//cout << gry.channels() << endl;

	d.compute(gry_treated, descriptorsValues, Size(0, 0), Size(0, 0), locations);

	//cout << "HOG descriptor size is " << d.getDescriptorSize() << endl;
	//cout << "img dimensions: " << img.cols << " width x " << img.rows << " height" << endl;
	//cout << "Found " << descriptorsValues.size() << " descriptor values" << endl;
	//cout << "Nr of locations specified : " << locations.size() << endl << endl;


	//Mat visu = get_hogdescriptor_visu(src, descriptorsValues, const Size & size);
	//int cellsize = 8;   // cellsize original = 16;
	//int winsizeX = 64;   // winsizeX original = 64;
	//int winsizeY = 128;  // winsizeY original = 128;


	double GRADIENT_FEATURES[DIMENSAO_GRAD];
	Mat visu = get_hogdescriptor_visual_image(GRADIENT_FEATURES, gry_treated, descriptorsValues, Size(winsizeX,winsizeY), Size(cellsize, cellsize), 3, 3);


	// preenche a cor
	for (i = 0; i < DIMENSAO_COR; i++)   FEATURES[i] = HSV_hist[i];
	
	//preenche gradiente
	//for (i = 0; i < DIMENSAO_GRAD; i++)  FEATURES[i] = GRADIENT_FEATURES[i];

	// preenche tons de cinza
	//for (i = 0; i < DIMENSAO_GRAY; i++)   FEATURES[i] = HIST_GRAY[i];
	
	// preenche COR + GRADIENTE
	//for (i = 0; i < DIMENSAO_GRAD; i++)  FEATURES[i+DIMENSAO_COR] = GRADIENT_FEATURES[i];
	//
	// preenche COR + GRAY
	//for (i = 0; i < DIMENSAO_GRAY; i++)  FEATURES[i + DIMENSAO_COR] = HIST_GRAY[i];

	// preenche COR + GRAY + GRADIENTE
	//for (i = 0; i < DIMENSAO_GRAD; i++)  FEATURES[i + DIMENSAO_COR + DIMENSAO_GRAY] = GRADIENT_FEATURES[i];

	//namedWindow("hog", 2);
	//imshow("hog",visu);
	//waitKey(0);

}


/*
// extrai caracteristicas de cor com IplImage
void obtem_featuresIplImage(double FEATURES[], IplImage* img)
{

	int i, x, y;

	// IMAGENS PARA MANIPULAÇÃO DE DIVERSAS CARACTERÍSTICAS
	IplImage* dst = cvCreateImage(cvSize(img->width, img->height), 8, 3);
	IplImage* h   = cvCreateImage(cvSize(img->width, img->height), 8, 1);
	IplImage* s   = cvCreateImage(cvSize(img->width, img->height), 8, 1);
	IplImage* v   = cvCreateImage(cvSize(img->width, img->height), 8, 1);
	IplImage* gry = cvCreateImage(cvSize(img->width, img->height), 8, 1);


	/////  PRIMEIRA CARACTERISTICA 
	// EXTRAI CARACTERISICAS DE COR - HISTOGRAMA HSV162
	const int BINS = Hbin * Sbin * Vbin;
	float HSV_hist[BINS];

	for (i = 0; i < BINS; i++)  HSV_hist[i] = 0.0;

   // extrai caracteristicas de cor
	cvCvtColor(img, dst, CV_RGB2HSV);
	//cvNamedWindow("original", 2);
	//cvShowImage("original", img);

	//cvNamedWindow("hsv", 2);
	//cvShowImage("hsv", dst);

	int Lin = img->height;
	int Col = img->width;
	for (y = 0; y < Lin; y++)
	{
		for (x = 0; x < Col; x++)
		{
			int H = ((uchar*)(dst->imageData + dst->widthStep*y))[x * 3];
			int S = ((uchar*)(dst->imageData + dst->widthStep*y))[x * 3 + 1];
			int V = ((uchar*)(dst->imageData + dst->widthStep*y))[x * 3 + 2];

			int Hn = ((float)H / 179.0)*(float)Hbin;
			int Sn = ((float)S / 255.0)*(float)Sbin;
			int Vn = ((float)V / 255.0)*(float)Vbin;

			int indx = 9 * Hn + 3 * Sn + Vn;
			HSV_hist[indx] = HSV_hist[indx] + 1.0;
		}
    }

	int total_pixels = img->width * img->height;
	for (i = 0; i < BINS; i++)  HSV_hist[i] = HSV_hist[i] / (float)total_pixels;

	for (i = 0; i < BINS; i++) FEATURES[i] = HSV_hist[i];


}

*/




/*void database_reader(std::vector<sample_type> samplesss)
{
   
    


}*/


//HOGDescriptor visual_imagealizer
// adapted for arbitrary size of feature sets and training images
// Size(64,1128), //winSize
// Size(16,16), //blocksize
// Size(8,8), //blockStride,
// Size(8,8), //cellSize,
// 3, //scaleFactor
// 3, //vis_factor
// 9, //nbins,
// 0, //derivAper,
// -1, //winSigma,
// 0, //histogramNormType,
// 0.2, //L2HysThresh,
// 0 //gammal correction,
// //nlevels=64



Mat get_hogdescriptor_visual_image(double GRADIENT_FEATURES[], Mat& origImg,
	std::vector<float>& descriptorValues,
	Size winSize,
	Size cellSize,
	int scaleFactor,
	double viz_factor)
{
	Mat visual_image;
	resize(origImg, visual_image, Size(origImg.cols*scaleFactor, origImg.rows*scaleFactor));

	int gradientBinSize = 9;
	// dividing 180° into 9 bins, how large (in rad) is one bin?
	float radRangeForOneBin = 3.14 / (float)gradientBinSize;

	// prepare data structure: 9 orientation / gradient strenghts for each cell
	int cells_in_x_dir = winSize.width / cellSize.width;
	int cells_in_y_dir = winSize.height / cellSize.height;
	int totalnrofcells = cells_in_x_dir * cells_in_y_dir;

	float*** gradientStrengths = new float**[cells_in_y_dir];
	int** cellUpdateCounter = new int*[cells_in_y_dir];
	
	for (int y = 0; y<cells_in_y_dir; y++)
	{
		gradientStrengths[y] = new float*[cells_in_x_dir];
		cellUpdateCounter[y] = new int[cells_in_x_dir];
		for (int x = 0; x<cells_in_x_dir; x++)
		{
			gradientStrengths[y][x] = new float[gradientBinSize];
			cellUpdateCounter[y][x] = 0;

			for (int bin = 0; bin<gradientBinSize; bin++)
				gradientStrengths[y][x][bin] = 0.0;
		}
	}

	// nr of blocks = nr of cells - 1
	// since there is a new block on each cell (overlapping blocks!) but the last one
	int blocks_in_x_dir = cells_in_x_dir - 1;
	int blocks_in_y_dir = cells_in_y_dir - 1;

	// compute gradient strengths per cell
	int descriptorDataIdx = 0;
	int cellx = 0;
	int celly = 0;

	//cout << "blocks_in_x_dir " << blocks_in_x_dir << endl;
	//cout << "blocks_in_y_dir " << blocks_in_y_dir << endl;

	for (int blockx = 0; blockx<blocks_in_x_dir; blockx++)
	{
		for (int blocky = 0; blocky<blocks_in_y_dir; blocky++)
		{
			// 4 cells per block ...
			for (int cellNr = 0; cellNr<4; cellNr++)
			{
				// compute corresponding cell nr
				int cellx = blockx;
				int celly = blocky;
				if (cellNr == 1) celly++;
				if (cellNr == 2) cellx++;
				if (cellNr == 3)
				{
					cellx++;
					celly++;
				}

				for (int bin = 0; bin<gradientBinSize; bin++)
				{
					float gradientStrength = descriptorValues[descriptorDataIdx];
					descriptorDataIdx++;

					gradientStrengths[celly][cellx][bin] += gradientStrength;

				} // for (all bins)


				  // note: overlapping blocks lead to multiple updates of this sum!
				  // we therefore keep track how often a cell was updated,
				  // to compute average gradient strengths
				cellUpdateCounter[celly][cellx]++;

			} // for (all cells)


		} // for (all block x pos)
	} // for (all block y pos)


	//for(int py=0; py < 8; py++)
    //for(int px=0; px < )

	  // compute average gradient strengths
	//int kx,
	//int ky;
	int k = 0;
	for (int celly = 0; celly<cells_in_y_dir; celly++)
	{
		for (int cellx = 0; cellx<cells_in_x_dir; cellx++)
		{
			float NrUpdatesForThisCell = (float)cellUpdateCounter[celly][cellx];

			// compute average gradient strenghts for each gradient bin direction
			for (int bin = 0; bin<gradientBinSize; bin++)
			{
				gradientStrengths[celly][cellx][bin] /= NrUpdatesForThisCell;
				GRADIENT_FEATURES[k++] = gradientStrengths[celly][cellx][bin];
			}
			//kx = cellx;
		}
		//ky = celly;
	}


	//cout << "celly = " << cells_in_y_dir << " cellx = " << cells_in_x_dir << " bin = "<< gradientBinSize << endl;
	
    // VISUALIZAÇÃO
	// draw cells
	for (int celly = 0; celly<cells_in_y_dir; celly++)
	{
		for (int cellx = 0; cellx<cells_in_x_dir; cellx++)
		{
			int drawX = cellx * cellSize.width;
			int drawY = celly * cellSize.height;

			int mx = drawX + cellSize.width / 2;
			int my = drawY + cellSize.height / 2;

			cv::rectangle(visual_image,
				Point(drawX*scaleFactor, drawY*scaleFactor),
				Point((drawX + cellSize.width)*scaleFactor,
				(drawY + cellSize.height)*scaleFactor),
				CV_RGB(100, 100, 100),
				1);

			// draw in each cell all 9 gradient strengths
			for (int bin = 0; bin<gradientBinSize; bin++)
			//for (int bin = 0; bin<1; bin++)
			{
				float currentGradStrength = gradientStrengths[celly][cellx][bin];

				// no line to draw?
				if (currentGradStrength == 0)
					continue;

				float currRad = bin * radRangeForOneBin + radRangeForOneBin / 2;

				float dirVecX = cos(currRad);
				float dirVecY = sin(currRad);
				float maxVecLen = cellSize.width / 2;
				float scale = viz_factor; // just a visual_imagealization scale,
										  // to see the lines better

										  // compute line coordinates
				float x1 = mx - dirVecX * currentGradStrength * maxVecLen * scale;
				float y1 = my - dirVecY * currentGradStrength * maxVecLen * scale;
				float x2 = mx + dirVecX * currentGradStrength * maxVecLen * scale;
				float y2 = my + dirVecY * currentGradStrength * maxVecLen * scale;

				// draw gradient visual_imagealization
				line(visual_image,
					Point(x1*scaleFactor, y1*scaleFactor),
					Point(x2*scaleFactor, y2*scaleFactor),
					CV_RGB(0, 0, 255),
					1);

			} // for (all bins)

		} // for (cellx)
	} // for (celly)
	

	  // don't forget to free memory allocated by helper data structures!
	for (int y = 0; y<cells_in_y_dir; y++)
	{
		for (int x = 0; x<cells_in_x_dir; x++)
		{
			delete[] gradientStrengths[y][x];
		}
		delete[] gradientStrengths[y];
		delete[] cellUpdateCounter[y];
	}
	delete[] gradientStrengths;
	delete[] cellUpdateCounter;

	return visual_image;

}


/*

Mat get_hogdescriptor_visu(const Mat& color_origImg, std::vector<float>& descriptorValues, const Size & size)
{
	const int DIMX = size.width;
	const int DIMY = size.height;
	float zoomFac = 3;
	Mat visu;
	resize(color_origImg, visu, Size((int)(color_origImg.cols*zoomFac), (int)(color_origImg.rows*zoomFac)));

	int cellSize = 8;
	int gradientBinSize = 9;
	float radRangeForOneBin = (float)(CV_PI / (float)gradientBinSize); // dividing 180 into 9 bins, how large (in rad) is one bin?

																	   // prepare data structure: 9 orientation / gradient strenghts for each cell
	int cells_in_x_dir = DIMX / cellSize;
	int cells_in_y_dir = DIMY / cellSize;
	float*** gradientStrengths = new float**[cells_in_y_dir];
	int** cellUpdateCounter = new int*[cells_in_y_dir];
	for (int y = 0; y<cells_in_y_dir; y++)
	{
		gradientStrengths[y] = new float*[cells_in_x_dir];
		cellUpdateCounter[y] = new int[cells_in_x_dir];
		for (int x = 0; x<cells_in_x_dir; x++)
		{
			gradientStrengths[y][x] = new float[gradientBinSize];
			cellUpdateCounter[y][x] = 0;

			for (int bin = 0; bin<gradientBinSize; bin++)
				gradientStrengths[y][x][bin] = 0.0;
		}
	}

	// nr of blocks = nr of cells - 1
	// since there is a new block on each cell (overlapping blocks!) but the last one
	int blocks_in_x_dir = cells_in_x_dir - 1;
	int blocks_in_y_dir = cells_in_y_dir - 1;

	// compute gradient strengths per cell
	int descriptorDataIdx = 0;
	int cellx = 0;
	int celly = 0;

	for (int blockx = 0; blockx<blocks_in_x_dir; blockx++)
	{
		for (int blocky = 0; blocky<blocks_in_y_dir; blocky++)
		{
			// 4 cells per block ...
			for (int cellNr = 0; cellNr<4; cellNr++)
			{
				// compute corresponding cell nr
				cellx = blockx;
				celly = blocky;
				if (cellNr == 1) celly++;
				if (cellNr == 2) cellx++;
				if (cellNr == 3)
				{
					cellx++;
					celly++;
				}

				for (int bin = 0; bin<gradientBinSize; bin++)
				{
					float gradientStrength = descriptorValues[descriptorDataIdx];
					descriptorDataIdx++;

					gradientStrengths[celly][cellx][bin] += gradientStrength;

				} // for (all bins)


				  // note: overlapping blocks lead to multiple updates of this sum!
				  // we therefore keep track how often a cell was updated,
				  // to compute average gradient strengths
				cellUpdateCounter[celly][cellx]++;

			} // for (all cells)


		} // for (all block x pos)
	} // for (all block y pos)


	  // compute average gradient strengths
	for (celly = 0; celly<cells_in_y_dir; celly++)
	{
		for (cellx = 0; cellx<cells_in_x_dir; cellx++)
		{

			float NrUpdatesForThisCell = (float)cellUpdateCounter[celly][cellx];

			// compute average gradient strenghts for each gradient bin direction
			for (int bin = 0; bin<gradientBinSize; bin++)
			{
				gradientStrengths[celly][cellx][bin] /= NrUpdatesForThisCell;
			}
		}
	}

	// draw cells
	for (celly = 0; celly<cells_in_y_dir; celly++)
	{
		for (cellx = 0; cellx<cells_in_x_dir; cellx++)
		{
			int drawX = cellx * cellSize;
			int drawY = celly * cellSize;

			int mx = drawX + cellSize / 2;
			int my = drawY + cellSize / 2;

			cv::rectangle(visu, Point((int)(drawX*zoomFac), (int)(drawY*zoomFac)), Point((int)((drawX + cellSize)*zoomFac), (int)((drawY + cellSize)*zoomFac)), Scalar(100, 100, 100), 1);

			// draw in each cell all 9 gradient strengths
			for (int bin = 0; bin<gradientBinSize; bin++)
			{
				float currentGradStrength = gradientStrengths[celly][cellx][bin];

				// no line to draw?
				if (currentGradStrength == 0)
					continue;

				float currRad = bin * radRangeForOneBin + radRangeForOneBin / 2;

				float dirVecX = cos(currRad);
				float dirVecY = sin(currRad);
				float maxVecLen = (float)(cellSize / 2.f);
				float scale = 2.5; // just a visualization scale, to see the lines better

								   // compute line coordinates
				float x1 = mx - dirVecX * currentGradStrength * maxVecLen * scale;
				float y1 = my - dirVecY * currentGradStrength * maxVecLen * scale;
				float x2 = mx + dirVecX * currentGradStrength * maxVecLen * scale;
				float y2 = my + dirVecY * currentGradStrength * maxVecLen * scale;

				// draw gradient visualization
				line(visu, Point((int)(x1*zoomFac), (int)(y1*zoomFac)), Point((int)(x2*zoomFac), (int)(y2*zoomFac)), Scalar(0, 255, 0), 1);

			} // for (all bins)

		} // for (cellx)
	} // for (celly)


	  // don't forget to free memory allocated by helper data structures!
	for (int y = 0; y<cells_in_y_dir; y++)
	{
		for (int x = 0; x<cells_in_x_dir; x++)
		{
			delete[] gradientStrengths[y][x];
		}
		delete[] gradientStrengths[y];
		delete[] cellUpdateCounter[y];
	}
	delete[] gradientStrengths;
	delete[] cellUpdateCounter;

	return visu;

} // get_hogdescriptor_visu

*/


void get_database_space_vector(string featuredata, int dimen, int feature[])
{

	std::ifstream in(featuredata, std::ios_base::binary);
	for (int i = 0; i < dimen; i++)
	{
		int DATAi;
		in.read((char *)&DATAi, sizeof(int));
		feature[i] = DATAi;
	}
	in.close();
}


Mat treatimage(Mat src)
{
	Mat dst;

	GaussianBlur(src,src,Size(3,3),0,0); 

	equalizeHist(src, dst);
	//dst = src;

	return dst;

}



#endif // ATHOMEBRAHMATREINO


//GaussianBlur(InputArray src, OutputArray dst, Size ksize, double sigmaX, double sigmaY = 0, int borderType = BORDER_DEFAULT)