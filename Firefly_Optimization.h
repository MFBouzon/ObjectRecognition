

#include "incl.h"

using namespace std;
using namespace cv;

void psrGrayHist(Mat img, double H[], int L) 
{	
	int linhas  = img.rows;
	int colunas = img.cols;

	for (int i = 0; i<L; i++)  H[i] = 0;

	for (int y = 0; y<linhas; y++) {
		for (int x = 0; x<colunas; x++) {
			
			int pixelValue = (int)img.at<uchar>(y,x);  

			H[pixelValue] = H[pixelValue] + 1;
		}
	}

	for (int i = 0; i<L; i++) {
		H[i] = H[i] / (linhas*colunas);
	}

}


double newAlpha(double alpha, double delta) { // ok
											  //esta função reduz a aleatoriedade durante as iterações
	alpha = alpha*delta;
	return alpha;
}


double dist(std::vector<unsigned int> &elem1, std::vector<unsigned int> &elem2) { //ok
													  //esta funçao calcula a distancia entre dois vetores
	std::vector<double> aux;
	std::vector<double> aux2;

	double soma = 0;

	for (unsigned int i = 0; i< elem1.size(); i++) {
		int vai = elem1[i] - elem2[i];
		aux.push_back(vai);
	}

	for (unsigned int i = 0; i< elem1.size(); i++) {
		int vai = pow(aux[i], 2);
		aux2.push_back(vai);
	}

	for (unsigned int i = 0; i<elem1.size(); i++) {
		soma += aux2[i];
	}

	return (sqrt(soma));
}


void validaThresholds(std::vector<std::vector<unsigned int> > &fireflies) { //ok
														 //esta função ordena as linhas de cada firefly de forma crescente
														 //fireflies: matriz onde cada linha eh um firefly e as colunas sao os thresholds

	for (unsigned int i = 0; i < fireflies.size(); i++) {
		sort(fireflies[i].begin(), fireflies[i].end());
	}
}


std::vector<std::vector<double> > distancias(std::vector<std::vector<unsigned int> > &fireflies) { //ok
																	  //calcula todas as distancias entre os fireflies
	std::vector<std::vector <double> > all_dist;

	all_dist.resize(fireflies.size());

	for (unsigned int i = 0; i< fireflies.size(); i++) {
		all_dist[i].resize(fireflies.size());
	}

	for (unsigned int i = 0; i<all_dist.size(); i++) {
		for (unsigned int j = 0; j<all_dist.size(); j++) {
			all_dist[i][j] = 0.0;
		}
	}

	for (unsigned int i = 0; i<all_dist.size(); i++) {
		for (unsigned int j = 0; j<all_dist.size(); j++) {
			all_dist[i][j] = dist(fireflies[i], fireflies[j]);
		}
	}
	return all_dist;
}



void init_ffa(int n, int thresholds, int range, std::vector<std::vector<unsigned int> > &fireflies) { // ok
																				   //esta função cria matriz que guarda os fireflies e seus respectivos thresholds
																				   //n numero de ff
																				   //thresholds numero de colunas
																				   //lightn vetor double que guarda os brilhos dos fireflies que inicializa com 0

	// se esta linha estiver comentada, o FFA sera pseudoaleatorio
	//srand(time(NULL));

	//-----------------------------------------------------//

	//inicializa vector onde guarda o brilho dos fireflies para 0

	//    for(int i =0; i<n; i++){
	//        lightn.push_back(0);
	//    }
	//---------------------------------------------//
	int* v = new int[range];

	for (int i = 0; i<n; i++)
	{

		for (int t = 0; t<range; t++) { // preenche o vetor com numeros de 0  a range-1
			v[t] = t;
		}
		int aux = 0;
		int temp = 0;

		for (int t2 = 0; t2 <range; t2++) { // embaralha os numeros do vetor de forma aleatoria
			aux = std::rand() % range;
			temp = v[t2];
			v[t2] = v[aux];
			v[aux] = temp;
		}


		for (int j = 0; j<thresholds; j++) {
			fireflies[i][j] = v[j];
			//                    cout<<"v = "<< v[j]<<endl;
		}
	}
	validaThresholds(fireflies); // valida fireflies
								 //----------------------------------------------------------------------------
								 //imprime matriz e calcula distancia
								 //            for(int i=0; i<n; i++){

								 //            if(i!=n-1) cout<<"Dist: "<<i+1<<"+"<<i+2<<" = "<<dist(fireflies[i],fireflies[i+1])<<"    ";

								 //                for(int j=0; j<thresholds; j++){
								 //                    cout<< fireflies[i][j]<<"  ";
								 //                }
								 //                 cout<<endl;

								 //            }

}



void ff_move(double &beta, std::vector<std::vector<unsigned int> > &fireflies, std::vector<double> &lightn, double alpha, double gamma, int range, std::vector<std::vector<double> > a) 
{
	//esta função move os fireflies para o firefly com mais brilho
	
	//se esta linha estiver comentada, o algoritmo sera pseudoaleatorio 
	//srand(time(NULL));
	int* v = new int[fireflies[0].size()];
	double  beta0 = 1;
	int ni = fireflies.size(); //recebe o tamanho do vector

	for (int i = 0; i<ni; i++)
	{//1 for
		for (int j = 0; j<ni; j++)
		{//2 for
			if (lightn[j] > lightn[i])
			{//abrindo 1 if

				beta0 = 1.0;
				beta = beta0*exp((-gamma)*pow(a[i][j], 2));
				//                    cout<<"Beta = "<<beta<<endl;
				//                    system("PAUSE");

				for (unsigned int t = 0; t<fireflies[0].size(); t++)
				{//3 for
					v[t] = alpha*(std::rand() % 256);
					//cout<<"V = "<<v[t];
				}//fechando 3 if

				if (i != ni)// abrindo segundo if
				{
					for (unsigned int k = 0; k<fireflies[0].size(); k++)
					{//4 for

						fireflies[i][k] = ((1 - beta)*(fireflies[i][k])) + ((beta)*(fireflies[j][k])) + v[k];
						fireflies[i][k] = fireflies[i][k] / (1.0 + alpha);
						//                            for(unsigned int t=0; t<fireflies.size(); t++)
						//                            {//5 for
						//                                //cout<<"Entrou t = "<<t <<endl;

						//                                for(unsigned int t2=0; t2<fireflies[0].size(); t2++)
						//                                {//6 for
						//                                    //cout<<"Entrou t2 = "<<t2 <<endl;
						//                                    fireflies[t][t2] = fireflies[t][t2] / (1+alpha);
						//                                }//fechando 6 for
						//                            }//fechando 5 for
					}//fechando 4 for
				}//fechando segundo if
			}//fechando 1  if
		}//fim do 2 for
	}//fim do 1 for
	 //cout<<"fireflies size : "<<fireflies.size()<<endl;
	validaThresholds(fireflies);
}



void muda_fireflies(std::vector<std::vector <unsigned int> > &fireflies, std::vector<unsigned int> index) {

	std::vector<std::vector<unsigned int> > new_fireflies(fireflies.size(), std::vector<unsigned int>(fireflies[0].size(), 0));

	for (unsigned int i = 0; i< fireflies.size(); i++) {
		for (unsigned int j = 0; j< fireflies[0].size(); j++) {
			new_fireflies[i][j] = fireflies[i][j];
		}
	}

	for (unsigned int i = 0; i< fireflies.size(); i++) {
		for (unsigned int j = 0; j< fireflies[0].size(); j++) {
			fireflies[i][j] = new_fireflies[index[i]][j];
		}
	}

}

void tide_up(std::set<pair<double, unsigned int> > &zn, std::vector<unsigned int> &index, std::vector<double> &lightn) {
	//negativar, ordenar e devolver em lightn
	//devolver em index os indexes originais

	std::set<pair<double, unsigned int> >::iterator it;
	int i = 0;
	for (it = zn.begin(); it != zn.end(); it++) {
		lightn[i] = it->first * (-1);
		//cout<<"Tsallis : "<<lightn[i]<<endl;
		i++;
	}

	//    system("PAUSE");
	i = 0;
	for (it = zn.begin(); it != zn.end(); it++) {
		index[i] = it->second;
		i++;
	}
}


//função principal do firefly
std::vector<unsigned int> gaoFireflyThresholds(double* histograma, int hist_buff, int thresholds, int NFireFlies, int iter) 
{
	//esta função retorna um vetor de double que contém o brilho dos fireflies(lightn)
	//hist_buff =range
	int n = NFireFlies;
	int MaxGeneration = iter;
	double beta = 0;
	std::vector<std::vector<unsigned int> > fireflies; //matriz de fireflies com linhas = qtd fireflies(NFireFlies), colunas = thresholds(thresholds)
	fireflies.resize(n, std::vector<unsigned int>(thresholds, 0));
	std::vector<double> lightn(n, 0); //vector onde guarda o brilho de n fireflies
	std::vector<unsigned int> melhor(thresholds, 0);
	std::vector<unsigned int> index(n, 0);
	double alpha = 0.01;
	double gamma = 1.0;
	double delta = 0.97;

	init_ffa(n, thresholds, hist_buff, fireflies); // inicia matriz de fireflies que vao chutar onde será o corte no histograma
												   // hist_buff = range
												   //vector<double> zn(n, 0);
	std::set<pair<double, unsigned int> > zn;

	std::vector<std::vector<double>> all_dist(n, std::vector<double>(n, 0));

	for (int i = 0; i<MaxGeneration; i++) {
		zn.clear();


		for (int h = 0; h<n; h++) {
			pair<double, int > p((psrAvaliacaoTsallis(histograma,0.35, fireflies[h], thresholds)*(-1)), h);
			zn.insert(p);
		}

		tide_up(zn, index, lightn);
		//set<pair<double, int> >::iterator it = zn.begin();
		//        for(unsigned int a=0; a<zn.size(); a++){
		//            cout<<"Index -> "<<index[a]<<"  Lightn = "<<lightn[a]<<endl;
		//            it++;
		//        }

		//mudar matriz fireflies para que o melhor vector fique na linha fireflies[0]
		muda_fireflies(fireflies, index);

		//       //atualizar os fireflies
		//        for(int t=0; t<fireflies.size(); t++){
		//            for(int t2 =0; t2<fireflies[0].size(); t2++){
		//                fireflies[t][t2] =
		//            }
		//        }

		//moving fireflies to better locations

		for (int a = 0; a<n; a++) {
			for (int b = 0; b<n; b++) {
				all_dist[a][b] = dist(fireflies[a], fireflies[b]);
			}
		}

		ff_move(beta, fireflies, lightn, alpha, gamma, 256, all_dist);

		alpha = newAlpha(alpha, delta);

		for (int v = 0; v<thresholds; v++) {
			melhor[v] = fireflies[0][v];
		}

	}
	std::vector<unsigned int> bests;

	bests = melhor;

	return bests;
}




std::vector<unsigned int> beststhresholds(Mat img, int thresholds, int n_fires, int itera) {

	//      ofstream MyExcelFile;
	//      MyExcelFile.open("C:/Users/unifgoliveira/Documents/psteste.csv");

	double* H = new double[256]; // gray histogram array

	psrGrayHist(img, H, 256); // process the gray histogram of the image and throw it on array v

					 //        double temp =0;
					 //        for(int i =0; i<256; i++){
					 //            MyExcelFile << v[i]<<",";
					 //        }

					 //        MyExcelFile.close();

					 //        int thresholds = 2;
					 //        int n_fires = 50;
					 //        int itera = 100;

	std::vector<unsigned int> bests;

	bests = gaoFireflyThresholds(H, 256, thresholds, n_fires, itera); // put the bests thresholds on vector bests

	//cout<<"bests = "<<bests.size()<<endl;

	return bests;
}