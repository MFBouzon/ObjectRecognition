#ifndef ENTROPIA_H
#define ENTROPIA_H

#include <vector>

#include "firefly.h"
#include "histogram.h"

using namespace std;

namespace fei{
	class Entropy
	{
	public:
		virtual double operator() (const std::vector<unsigned int> &) const = 0;
		virtual double additivity(const std::vector<double> &) const = 0;
	};

	class Shannon : public Entropy
	{
	public:
		double operator() (const std::vector<unsigned int> &) const;
		double additivity(const std::vector<double> &) const;
	};

	class Tsallis : public Entropy
	{
	public:
		Tsallis(double);
		void setQ(double);
		double operator() (const std::vector<unsigned int> &) const;
		double additivity(const std::vector<double> &) const;
	private:
		double q;
	};

	class EntropicImageEvaluation : public Evaluation
	{
	public:
		EntropicImageEvaluation(const Histogram &, const Entropy &);
		double operator() (const std::vector<double> &) const;
	private:
		const Histogram &histogram;
		const Entropy &entropy;
	};
}

#endif // ! ENTROPIA_H