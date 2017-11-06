

#ifndef ATHOMEVISUALSTRATEGY
#define ATHOMEVISUALSTRATEGY



Mat media(std::vector<Mat> images) {
	
	int **B = new int*[images[0].rows];
	int **G = new int*[images[0].rows];
	int **R = new int*[images[0].rows];
	
	for (int i = 0; i < images[0].rows; i++) {
		B[i] = new int[images[0].cols];
		G[i] = new int[images[0].cols];
		R[i] = new int[images[0].cols];
	}
	
	for (int i = 0; i < images[0].rows; i++) {
		for (int j = 0; j < images[0].cols; j++) {
			B[i][j] = 0;
			G[i][j] = 0;
			R[i][j] = 0;
		}
	}
	
	
	for (int a = 0; a < images.size(); a++) {
		cout << "a = " << a << endl;
		for (int i = 0; i < images[a].rows; i++) {
			for (int j = 0; j < images[a].cols; j++) {
				B[i][j] += images[a].at<Vec3b>(i, j)[0];
				G[i][j] += images[a].at<Vec3b>(i, j)[1];
				R[i][j] += images[a].at<Vec3b>(i, j)[2];
			}
		}
	}
	
	
	Mat m = images[0].clone();
	for (int i = 0; i < images[0].rows; i++) {
		for (int j = 0; j < images[0].cols; j++) {
			B[i][j] = B[i][j] / images.size();
			G[i][j] = G[i][j] / images.size();
			R[i][j] = R[i][j] / images.size();
			m.at<Vec3b>(i, j)[0] = B[i][j];
			m.at<Vec3b>(i, j)[1] = G[i][j];
			m.at<Vec3b>(i, j)[2] = R[i][j];
		}
	}

	for (int i = 0; i < images[0].rows; i++) {
		delete B[i];
		delete G[i];
		delete R[i];
	}
	delete B;
	delete G;
	delete R;
	return m;
}
std::vector<Mat> buildSetofROIs(std::vector<Mat> segmentedregions)
{

	int Nimages = segmentedregions.size();

	std::vector<Mat> Regioes; // (nLabels + 1, Mat(img.size(), CV_8UC3));

	for (int i = 0; i < Nimages; i++)
	{
		
		Mat im1 = segmentedregions[i];

		Mat imroi(im1.size(), CV_8UC1);
		int nLabels = connectedComponents(im1, imroi, 8);

		//cout << imroi.type() << endl;
		for (int r = 1; r < nLabels; r++)
		{
			
			int npixels = 0;
			Mat novo(im1.size(), CV_8UC1);
			for (int y = 0; y < imroi.rows; ++y) {
				for (int x = 0; x < imroi.cols; ++x) {
					int label = (int)imroi.at<int>(y, x);
					if (label == r) {
						novo.at <uchar>(y,x) = 255;
						npixels++;
					}
					else
						novo.at<uchar>(y,x) = 0;
				}
			}
			double pnpixels = (double)npixels / (novo.rows * novo.cols);
			if ( pnpixels >= 0.002) Regioes.push_back(novo);
		}
	}
    return Regioes;
}



std::vector<Mat> get_images_from_original_rois(std::vector<Mat> candidateROIs, Mat img)
{
  
	std::vector<Mat> originalRegioes;

	for (int ir = 0; ir < candidateROIs.size(); ir++)
	{
		Mat im0 = candidateROIs[ir];
		Mat novo = img.clone();

		for(int y=0; y < im0.rows; y++)
		  for (int x = 0; x < im0.cols; x++)
		  {
			  if (im0.at <uchar>(y, x))
			  {
				  novo.at<Vec3b>(Point(x, y))[0] = img.at<Vec3b>(Point(x, y))[0];
				  novo.at<Vec3b>(Point(x, y))[1] = img.at<Vec3b>(Point(x, y))[1];
				  novo.at<Vec3b>(Point(x, y))[2] = img.at<Vec3b>(Point(x, y))[2];
			  }
			  else
			  {
				  novo.at<Vec3b>(Point(x, y))[0] = 0;
				  novo.at<Vec3b>(Point(x, y))[1] = 0;
				  novo.at<Vec3b>(Point(x, y))[2] = 0;
			  }
		  }
		
		originalRegioes.push_back(novo);
	}

	return originalRegioes;
}

std::vector<Mat> getCropROIS(std::vector<Mat> ROIS, Mat origImg) {
	std::vector<Mat> crpROIS;
	for (int i = 0; i < ROIS.size(); i++) {
		//Mat frame_gray;
		//cvtColor(ROIS[i], frame_gray, COLOR_BGR2GRAY);
		Rect R = boundingRect(ROIS[i]);
		if (R.area() != ROIS[i].rows*ROIS[i].cols)
		{
			Mat rectangled(ROIS[i]);
			cv::rectangle(rectangled, R, Scalar(0, 255, 0));
			//imshow("Retangulo", rectangled);
			//waitKey();
			Mat crop = origImg(R);
			crpROIS.push_back(crop);
			//resize(crop, crop, Size(640, 640));
		}
	}
	return crpROIS;
}
#endif // MULTILIMIARIZACAO


