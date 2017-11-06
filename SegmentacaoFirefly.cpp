

#include "AtHomeConfig.h"
#include "Firefly_Optimization.h"
#include "multilimiarizacao.h"
#include "AtHomeVisualStrategy.h"



int main()
{
	// le imagens e extrai caracteristicas		
	string tipo = ".png";
	for (int i = 1; i <= 10; i++){
		string path = "";
		string path2 = "seg";
		path2 = path2 + std::to_string(i);
		path2.append(tipo);
		path.append(PATH5);
		path = path + std::to_string(i);
		path.append(tipo);

		cout << path.c_str() << endl;

		Mat img = imread(path.c_str(), 1);// novo

		//convert para tons de ciza
		Mat gry;
		cvtColor(img, gry, CV_RGB2GRAY);

		std::vector<unsigned int> bests;

		bests = beststhresholds(gry, 1, 50, 100);

		for (std::vector<unsigned int>::iterator it = bests.begin(); it != bests.end(); ++it)
			std::cout << ' ' << *it;

		cout << endl;
		Mat seg = MultiLim2(gry, bests, 0);
		//imshow("segmentation", seg);
		//xwaitKey();
		imwrite(path2, seg);
	}
	/*
	std::vector<Mat> SetOfSegmentedRegions = SplitSegmentedRegions(gry, bests, notbackwards);

	std::vector<Mat> candidateROIS = buildSetofROIs(SetOfSegmentedRegions);

	std::vector<Mat> images_originalROIS = get_images_from_original_rois(candidateROIS,img);

	cout << candidateROIS.size() << endl;

	namedWindow("1", 3);
	for (int i = 0; i < candidateROIS.size(); i++)
	{
		imshow("1", images_originalROIS[i]);
		waitKey(0); 
	}*/

	system("pause");

	return 0;
};



