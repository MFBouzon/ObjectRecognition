#ifndef HISTOGRAMA_H
#define HISTOGRAMA_H

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <vector>

#define NIVEIS 256

namespace fei{
	class Histogram : public std::vector<unsigned int> 
	{
	public:
		Histogram(const cv::Mat &);
	};
}

#endif // ! HISTOGRAMA_H