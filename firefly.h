/*******************************************
** Firefly.h                              **
** Vers�o 2.0                             **
** Alterado em: 03/05/2015                **
** Alteracoes:                            **
** - Traduzido para o ingl�s              **
** - Vagalume agora � classe aninhada     **
** - M�todos que agem sobre apenas um     **
** vagalume s�o agora                     **
** fun��es membro da classe Vagalume      **
** - Retiradas funcionalidades            **
** desnecessarias                         **
*******************************************/

#ifndef FIREFLY_H
#define FIREFLY_H

#include <vector>

using namespace std;

namespace fei
{
	class Validation
	{
	public:
		virtual void operator() (std::vector<double> &) = 0;
	};

	class Evaluation
	{
	public:
		virtual double operator() (const std::vector<double> &) const = 0;
	};

	class FireflyAlgorithm
	{
	private:
		//! nested firefly
		class Firefly
		{
		private:
			//! makes sure that the variables values do not overstep the imposed lower and upper limits
			void certificateLimits();

			//! reference to the swarm the firefly is in
			FireflyAlgorithm *swarm;

			std::vector<double> variables;

			//! the brightness value calculated so far
			double brightness;


		public:
			//! default constructor
			Firefly(FireflyAlgorithm *swarm);

			//! get the brightness value calculated so far
			double getBrightness();
			const std::vector<double> &getVariables() const;

			void randomize();

			void calculateBrightness();

			//! beta0 belongs to [0, 1] and determinates how much the firefly will move, being 1 all the way to the other firefly (100%)
			void moveToward(Firefly &reference, double beta0);
			//! all the variables are randomly incremented or decremented up to alpha
			void moveRandomly(double alpha);

			void print() const;
		};

		//! euclidian distance between the two fireflies
		double distance(const Firefly &, const Firefly &);

		//! find the firefly with the best brightness value
		int findBest();

		//! vector containing the fireflies
		std::vector<Firefly> swarm;

		//! rules to wich the solution is submited
		// the function must recieve a solution and, if it isn't acceptable, return one that is
		Validation &validation;
		//! the function to be optimized
		Evaluation &evaluation;

		//! amount of variables of both the function and the solutions
		int variablesAmount;

		//! defines a interval that all the variables of the solution must be in
		double lowerLimit;
		double upperLimit;

		//! randomization coefficient
		double alpha;
		//! attraction coefficient
		double beta;
		//! light absorption coefficient, inverse of the fireflies perception of one anothers
		double gamma;
		//! randomization decay coefficient
		double delta;

	public:
		//! constructor
		FireflyAlgorithm(Evaluation &, int variablesAmount, double lowerLimit, double upperLimit, Validation &);

		//! set the randomization coefficient, being the random variation up to alpha, wich is a positive value
		void setAlpha(double);
		//! set the attraction coefficient, must belong to [0, 1]
		void setBeta(double);
		//! set the light absorption coefficient
		void setGamma(double);
		//! set the randomization decay coefficient, must belong to [0, 1], being the % of the randomization kept trough a iteration
		void setDelta(double);

		//! insert n fireflies carrying random solutions
		void insertFireflies(int firefliesAmount = 1);

		//! advance n iterations
		void iterate(int amount = 1);

		//! return the solution of the most bright firefly
		std::vector<double> extractSolution();

		void printSwarm();
	};
}

#endif
