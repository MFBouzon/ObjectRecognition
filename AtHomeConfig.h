
#ifndef JUDITECONFIG
#define JUDITECONFIG


#include <opencv2/core/utility.hpp>
#include "opencv2/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/core/core_c.h"
#include <opencv2/opencv.hpp>
#include <opencv2/features2d.hpp>
#include <vector>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>


using namespace std;
using namespace cv;

const int NUMOBJS = 15;

const int NUM_TRUE_CROPS = 5;


// Novos Objetos
char *PATHBV = "C:\\Users\\Paulo\\Dropbox\\Imagedatabase\\AtHomeDatabase\\NOVA_BASE_DE_DADOS\\AGUA_BIOLEVE_ROSA\\AGUA_BIOLEVE_ROSA";

char *PATHOL = "C:\\Users\\Paulo\\Dropbox\\Imagedatabase\\AtHomeDatabase\\NOVA_BASE_DE_DADOS\\AGUA_OLAF\\AGUA_OLAF";

char *PATHCP = "C:\\Users\\Paulo\\Dropbox\\Imagedatabase\\AtHomeDatabase\\NOVA_BASE_DE_DADOS\\CAIXA_PERFUME\\CAIXA_PERFUME";

char *PATHCV = "C:\\Users\\Paulo\\Dropbox\\Imagedatabase\\AtHomeDatabase\\NOVA_BASE_DE_DADOS\\CERVEJA\\CERVEJA";

char *PATHDS = "C:\\Users\\Paulo\\Dropbox\\Imagedatabase\\AtHomeDatabase\\NOVA_BASE_DE_DADOS\\DESODORANTE\\DESODORANTE";

char *PATHDF = "C:\\Users\\Paulo\\Dropbox\\Imagedatabase\\AtHomeDatabase\\NOVA_BASE_DE_DADOS\\DORFLEX\\DORFLEX";

char *PATHFC = "C:\\Users\\Paulo\\Dropbox\\Imagedatabase\\AtHomeDatabase\\NOVA_BASE_DE_DADOS\\FABER_CASTELL\\FABER_CASTELL";

char *PATHFU = "C:\\Users\\Paulo\\Dropbox\\Imagedatabase\\AtHomeDatabase\\NOVA_BASE_DE_DADOS\\FANTA_UVA\\FANTA";

char *PATHGV = "C:\\Users\\Paulo\\Dropbox\\Imagedatabase\\AtHomeDatabase\\NOVA_BASE_DE_DADOS\\GUARA_VITON\\GUARA_VITON";

char *PATHLC = "C:\\Users\\Paulo\\Dropbox\\Imagedatabase\\AtHomeDatabase\\NOVA_BASE_DE_DADOS\\LEITE_CAIXA\\LEITE_CAIXA";

char *PATHMF = "C:\\Users\\Paulo\\Dropbox\\Imagedatabase\\AtHomeDatabase\\NOVA_BASE_DE_DADOS\\MINI_FANTA\\MINI_FANTA";

char *PATHPL = "C:\\Users\\Paulo\\Dropbox\\Imagedatabase\\AtHomeDatabase\\NOVA_BASE_DE_DADOS\\POLENGUINHO\\POLENGUINHO";

char *PATHSA = "C:\\Users\\Paulo\\Dropbox\\Imagedatabase\\AtHomeDatabase\\NOVA_BASE_DE_DADOS\\SUCO_AMARELO\\SUCO_AMARELO";

char *PATHTD = "C:\\Users\\Paulo\\Dropbox\\Imagedatabase\\AtHomeDatabase\\NOVA_BASE_DE_DADOS\\TODDYNHO\\TODDYNHO"; 

char *PATHGP = "C:\\Users\\Paulo\\Dropbox\\Imagedatabase\\AtHomeDatabase\\NOVA_BASE_DE_DADOS\\GARRAFINHA_PEQ\\GARRAFINHA_PEQ";



// caminho dos dados POSITIVOS
char *PATH1Br = "C:\\Users\\Paulo\\Dropbox\\Imagedatabase\\AtHomeDatabase\\Brahma\\ImagensTreino\\Positivo\\brahma";

char *PATH1Ck = "C:\\Users\\Paulo\\Dropbox\\Imagedatabase\\AtHomeDatabase\\CK\\ImagensTreino\\Positivo\\CK";

char *PATH1Fc = "C:\\Users\\Paulo\\Dropbox\\Imagedatabase\\AtHomeDatabase\\fabercastel\\ImagensTreino\\Positivo\\fabercastel";

char *PATH1Mx = "C:\\Users\\Paulo\\Dropbox\\Imagedatabase\\AtHomeDatabase\\MAX\\ImagensTreino\\Positivo\\MAX";

char *PATH1Fl = "C:\\Users\\Paulo\\Dropbox\\Imagedatabase\\AtHomeDatabase\\fantalaranja\\ImagensTreino\\Positivo\\fantalaranja";

char *PATH1Gv = "C:\\Users\\Paulo\\Dropbox\\Imagedatabase\\AtHomeDatabase\\guaravito\\ImagensTreino\\Positivo\\guaravito";

char *PATH1Bl = "C:\\Users\\Paulo\\Dropbox\\Imagedatabase\\AtHomeDatabase\\bioleve\\ImagensTreino\\Positivo\\bioleve";

char *PATH1Df = "C:\\Users\\Paulo\\Dropbox\\Imagedatabase\\AtHomeDatabase\\dorflex\\ImagensTreino\\Positivo\\dorflex";

char *PATH1Fu = "C:\\Users\\Paulo\\Dropbox\\Imagedatabase\\AtHomeDatabase\\fantauva\\ImagensTreino\\Positivo\\fantauva";

char *PATH1Td = "C:\\Users\\Paulo\\Dropbox\\Imagedatabase\\AtHomeDatabase\\toddy\\ImagensTreino\\Positivo\\toddy";

// cinco novos
char *PATH1Dd = "C:\\Users\\Paulo\\Dropbox\\Imagedatabase\\AtHomeDatabase\\toddy\\ImagensTreino\\Positivo\\Desodorante";

char *PATH1Of = "C:\\Users\\Paulo\\Dropbox\\Imagedatabase\\AtHomeDatabase\\toddy\\ImagensTreino\\Positivo\\Olaf";

char *PATH1Sc = "C:\\Users\\Paulo\\Dropbox\\Imagedatabase\\AtHomeDatabase\\toddy\\ImagensTreino\\Positivo\\Suco";

char *PATH1Lt = "C:\\Users\\Paulo\\Dropbox\\Imagedatabase\\AtHomeDatabase\\toddy\\ImagensTreino\\Positivo\\Leite";

char *PATH1Pg = "C:\\Users\\Paulo\\Dropbox\\Imagedatabase\\AtHomeDatabase\\toddy\\ImagensTreino\\Positivo\\Polenguinho";


// caminho dos dados NEGATIVOS
char *PATH2Br = "C:\\Users\\Paulo\\Dropbox\\Imagedatabase\\AtHomeDatabase\\Brahma\\ImagensTreino\\Negativo\\brahmaneg";

char *PATH2Ck = "C:\\Users\\Paulo\\Dropbox\\Imagedatabase\\AtHomeDatabase\\CK\\ImagensTreino\\Negativo\\CKneg";

char *PATH2Fc = "C:\\Users\\Paulo\\Dropbox\\Imagedatabase\\AtHomeDatabase\\fabercastel\\ImagensTreino\\Negativo\\fabercastelneg";

char *PATH2Mx = "C:\\Users\\Paulo\\Dropbox\\Imagedatabase\\AtHomeDatabase\\MAX\\ImagensTreino\\Negativo\\MAXneg";

char *PATH2Fl = "C:\\Users\\Paulo\\Dropbox\\Imagedatabase\\AtHomeDatabase\\fantalaranja\\ImagensTreino\\Negativo\\fantalaranjaneg";

char *PATH2Gv = "C:\\Users\\Paulo\\Dropbox\\Imagedatabase\\AtHomeDatabase\\guaravito\\ImagensTreino\\Negativo\\guaravitoneg";

char *PATH2Bl = "C:\\Users\\Paulo\\Dropbox\\Imagedatabase\\AtHomeDatabase\\bioleve\\ImagensTreino\\Negativo\\bioleveneg";

char *PATH2Df = "C:\\Users\\Paulo\\Dropbox\\Imagedatabase\\AtHomeDatabase\\dorflex\\ImagensTreino\\Negativo\\dorflexneg";

char *PATH2Fu = "C:\\Users\\Paulo\\Dropbox\\Imagedatabase\\AtHomeDatabase\\fantauva\\ImagensTreino\\Negativo\\fantauvaneg";

char *PATH2Td = "C:\\Users\\Paulo\\Dropbox\\Imagedatabase\\AtHomeDatabase\\toddy\\ImagensTreino\\Negativo\\toddyneg";



// caminho da imagem teste TRUE POSITIVA
char *PATH3Br = "C:\\Users\\Paulo\\Dropbox\\Imagedatabase\\AtHomeDatabase\\Brahma\\ImagensTeste\\TruePositivo\\brahma-prateleira-001nt.png";

char *PATH3Ck = "C:\\Users\\Paulo\\Dropbox\\Imagedatabase\\AtHomeDatabase\\CK\\ImagensTeste\\TruePositivo\\CK2nt.png";

char *PATH3Fc = "C:\\Users\\Paulo\\Dropbox\\Imagedatabase\\AtHomeDatabase\\fabercastel\\ImagensTeste\\TruePositivo\\fabercastel4nt.png";

char *PATH3Mx = "C:\\Users\\Paulo\\Dropbox\\Imagedatabase\\AtHomeDatabase\\MAX\\ImagensTeste\\TruePositivo\\MAX1nt.png";

char *PATH3Fl = "C:\\Users\\Paulo\\Dropbox\\Imagedatabase\\AtHomeDatabase\\fantalaranja\\ImagensTeste\\TruePositivo\\fantalaranja1nt.png";

char *PATH3Gv = "C:\\Users\\Paulo\\Dropbox\\Imagedatabase\\AtHomeDatabase\\guaravito\\ImagensTeste\\TruePositivo\\guaravito3nt.png";

char *PATH3Bl = "C:\\Users\\Paulo\\Dropbox\\Imagedatabase\\AtHomeDatabase\\bioleve\\ImagensTeste\\TruePositivo\\bioleve4nt.png";

char *PATH3Df = "C:\\Users\\Paulo\\Dropbox\\Imagedatabase\\AtHomeDatabase\\dorflex\\ImagensTeste\\TruePositivo\\dorflex1nt.png";

char *PATH3Fu = "C:\\Users\\Paulo\\Dropbox\\Imagedatabase\\AtHomeDatabase\\fantauva\\ImagensTeste\\TruePositivo\\fantauva3nt.png";

char *PATH3Td = "C:\\Users\\Paulo\\Dropbox\\Imagedatabase\\AtHomeDatabase\\toddy\\ImagensTeste\\TruePositivo\\toddy3nt.png";



// caminho da imagem teste TRUE NEGATIVA
char *PATH4Br = "C:\\Users\\Paulo\\Dropbox\\Imagedatabase\\AtHomeDatabase\\Brahma\\ImagensTeste\\TrueNegativo\\brahmaneg-182800.png";

char *PATH4Ck = "C:\\Users\\Paulo\\Dropbox\\Imagedatabase\\AtHomeDatabase\\CK\\ImagensTeste\\TrueNegativo\\CKneg2nt.png";

char *PATH4Fc = "C:\\Users\\Paulo\\Dropbox\\Imagedatabase\\AtHomeDatabase\\fabercastel\\ImagensTeste\\TrueNegativo\\fabercastelneg16.png";

char *PATH4Mx = "C:\\Users\\Paulo\\Dropbox\\Imagedatabase\\AtHomeDatabase\\MAX\\ImagensTeste\\TrueNegativo\\MAXneg1nt.png";

char *PATH4Fl = "C:\\Users\\Paulo\\Dropbox\\Imagedatabase\\AtHomeDatabase\\fantalaranja\\ImagensTeste\\TrueNegativo\\fantalaranjaneg1nt.png";

char *PATH4Gv = "C:\\Users\\Paulo\\Dropbox\\Imagedatabase\\AtHomeDatabase\\guaravito\\ImagensTeste\\TrueNegativo\\guaravitoneg1nt.png";


// caminho genericos
char *PATH5 = "C:\\Users\\muril\\Pictures\\celula";

char *PATH6 = "C:\\Users\\Paulo\\Dropbox\\Imagedatabase\\AtHomeDatabase\\Prateleira\\prateleiracrop4.png";

char *PATH7 = "C:\\Users\\Paulo\\Dropbox\\Imagedatabase\\AtHomeDatabase\\MAX\\ImagensTreino\\Positivo\\MAX3.png";

char *PATH8 = "C:\\Users\\Paulo\\Dropbox\\Imagedatabase\\AtHomeDatabase\\Prateleira\\cropImg13.png";

//char *PATH9 = "C:\\Users\\Paulo\\Dropbox\\Imagedatabase\\AtHomeDatabase\\Prateleira\\prateleiracrop.png";

char *PATH9 = "C:\\Users\\Paulo\\Dropbox\\Imagedatabase\\AtHomeDatabase\\Prateleira\\prateleiracrop2.png";

//char *PATH9 = "C:\\Users\\Paulo\\Dropbox\\Imagedatabase\\AtHomeDatabase\\Prateleira\\prateleiracrop3.png";

//char *PATH9 = "C:\\Users\\Paulo\\Dropbox\\Imagedatabase\\AtHomeDatabase\\Prateleira\\mesa_com_toddy.png";

char *PATH10 = "C:\\Users\\Paulo\\Dropbox\\Imagedatabase\\AtHomeDatabase\\Prateleira\\prateleira.png";

char *PATH23 = "C:\\Users\\Paulo\\Dropbox\\Imagedatabase\\AtHomeDatabase\\Prateleira\\prateleiracrop5.png";

char *PATH12 = "C:\\Users\\Paulo\\Dropbox\\Imagedatabase\\AtHomeDatabase\\Prateleira\\brahma12.png";

char *PATH13 = "C:\\Users\\Paulo\\Dropbox\\Imagedatabase\\AtHomeDatabase\\Prateleira\\brahma13.png";

char *PATH14 = "C:\\Users\\Paulo\\Dropbox\\Imagedatabase\\AtHomeDatabase\\Prateleira\\prateleiracropdorflex1.png";

char *PATH15 = "C:\\Users\\Paulo\\Dropbox\\Imagedatabase\\AtHomeDatabase\\Prateleira\\prateleiracropfantauva1.png";

char *PATH16 = "C:\\Users\\Paulo\\Dropbox\\Imagedatabase\\AtHomeDatabase\\Prateleira\\prateleiracropfantalaranja1.png";

char *PATH17 = "C:\\Users\\Paulo\\Dropbox\\Imagedatabase\\AtHomeDatabase\\Prateleira\\prateleiracropfabercastel1.png";

char *PATH18 = "C:\\Users\\Paulo\\Dropbox\\Imagedatabase\\AtHomeDatabase\\Prateleira\\prateleiracropck1.png";

char *PATH19 = "C:\\Users\\Paulo\\Dropbox\\Imagedatabase\\AtHomeDatabase\\Prateleira\\prateleiracropguaraviton1.png";

char *PATH20 = "C:\\Users\\Paulo\\Dropbox\\Imagedatabase\\AtHomeDatabase\\Prateleira\\prateleiracropbioleve1.png";

char *PATH21 = "C:\\Users\\Paulo\\Dropbox\\Imagedatabase\\AtHomeDatabase\\Prateleira\\prateleiracropmax1.png";

char *PATH22 = "C:\\Users\\Paulo\\Dropbox\\Imagedatabase\\AtHomeDatabase\\Prateleira\\prateleiracropfalso1.png";

char *PATHVID = "C:\\Users\\Paulo\\Dropbox\\Imagedatabase\\AtHomeDatabase\\video_prateleira\\prateleira1.mp4";

char *PATH6MAX = "C:\\Users\\Paulo\\Dropbox\\Imagedatabase\\AtHomeDatabase\\Prateleira\\cropImg19.png";

char *CROPMAX = "C:\\Users\\Paulo\\Dropbox\\Implementacoes\\JuditProject\\Teste008\\cropmax.png";

char *NEWCROPMAX = "C:\\Users\\Paulo\\Dropbox\\Implementacoes\\JuditProject\\Teste008\\newcropmax.png";

char *CROPMAXMANUAL = "C:\\Users\\Paulo\\Dropbox\\Implementacoes\\JuditProject\\Teste008\\cropImg_manual.png";

char *VIDEO = "C:\\Users\\Paulo\\Dropbox\\PROJETOS\\VALE-ITV\\VALE-ITAQUI-MA\\VIDEO3\\frame";


// CAMINHOS DA BASE NOVA FORMATADA
	
const string OBJECTS[]{
	"Brahma",//0
	"CK",//1
	"FaberCastel", //2
	"MAX",  //3
	"FantaLaranja", //4
	"Guaraviton",  //5
	"Bioleve",  //6
	"Dorflex",  //7
	"FantaUva", //8
	"Toddy",  //9
	"Desodorante", //10
	"Olaf",  //11
	"Suco", //12
	"Leite", //13
	"Polenguinho", //14
	"Desconhecido"
};


char *NEW_DATABASE = "C:\\Users\\Paulo\\Dropbox\\Imagedatabase\\AtHomeDatabase\\NOVA_BASE_DE_DADOS\\";

const string OBJECTS_DIR_NAMES[]{
	"CERVEJA\\CERVEJA",
	"CAIXA_PERFUME\\CAIXA_PERFUME",
	"FABER_CASTELL\\FABER_CASTELL",
	"GARRAFINHA_PEQ\\GARRAFINHA_PEQ",
	"MINI_FANTA\\MINI_FANTA",
	"GUARA_VITON\\GUARA_VITON",
	"AGUA_BIOLEVE_ROSA\\AGUA_BIOLEVE_ROSA",
	"DORFLEX\\DORFLEX",
	"FANTA_UVA\\FANTA",
	"TODDYNHO\\TODDYNHO",
	"DESODORANTE\\DESODORANTE",
	"AGUA_OLAF\\AGUA_OLAF",
	"SUCO_AMARELO\\SUCO_AMARELO",
	"LEITE_CAIXA\\LEITE_CAIXA",
	"POLENGUINHO\\POLENGUINHO",
};


const string MAIN_PATH_DATABASE = "C:\\Users\\Paulo\\Dropbox\\Imagedatabase\\AtHomeDatabase\\OBJECTS_DATABASE\\";

const string NEW_OBJECTS_DIR_NAMES[]{
	"brahma",
	"ck",
	"fabercastell",
	"max",
	"minifantalaranja",
	"guaraviton",
	"bioleve",
	"dorflex",
	"fantauva",
	"toddynho",
	"doverosa",
	"olaf",
	"sucoamarelo",
	"italac",
	"polenguinho",
};


const unsigned long NFRAMES = 500;

const unsigned long NIMAGENS = 100;
const unsigned long NIMAGENSBIOLEVE = 176;


// caracterisitcas de cor
const int Hbin =   5;//10
const int Sbin =   5;
const int Vbin =   5;


// novas caracterisitcas de cor
const int C1 = 5;
const int C2 = 5;
const int C3 = 5;

const int bcor = 1;

// para o mathing template
#define maxRAT 9
//float RAT[maxRAT] = {0.2, 0.23, 0.25, 0.28, 0.3, 0.33, 0.35, 0.38, 0.4, 0.43, 0.45, 0.48, 0.5 };
double RAT[maxRAT] = { 0.2, 0.225, 0.25, 0.275, 0.3, 0.35, 0.38, 0.4, 0.5 };


const int NREGIONS_X = 3;
const int NREGIONS_Y = 3;

const int DIMENSAO_COR = bcor * bcor * C1 * C2 * C3;

//const int DIMENSAO_COR = 9 * Hbin * Sbin * Vbin;
//const int DIMENSAO_COR = 3 * Hbin * Sbin * Vbin;
//const int DIMENSAO_COR = Hbin;

// caracteristicas de tons de cinza
const int DIMENSAO_GRAY = 256;


const double gamma = 0.00025;
const double C = 5.0;
//const double gamma = 0.03125;
//const double C = 5.0;


const int cellsize =  16;   // cellsize original = 16;
const int winsizeX =  64;   // winsizeX original = 64;
const int winsizeY = 128;  // winsizeY original = 128;

//tamanho das features de gradiente
const int DIMENSAO_GRAD = (64 / cellsize)*(64 / cellsize) * 2 * 9;

//tamanho das features de EDGE INFO para mathcing
const int MATCH_SIZE = 30;
const int DIMENSAO_EDGE = MATCH_SIZE * MATCH_SIZE;


//const int DIMENSAO_FINAL = DIMENSAO_GRAY + DIMENSAO_GRAD;
//const int DIMENSAO_FINAL = DIMENSAO_GRAY + DIMENSAO_GRAD;
const int DIMENSAO_FINAL = DIMENSAO_COR + DIMENSAO_GRAD + DIMENSAO_EDGE;
//const int DIMENSAO_FINAL = DIMENSAO_COR + DIMENSAO_GRAD;
//const int DIMENSAO_FINAL = DIMENSAO_COR;
//const int DIMENSAO_FINAL = DIMENSAO_GRAD;

const int NNOISE = 0.0005;

const int delX = 30;
const int delY = 30;

typedef struct imageandindx
{
	Mat img_crop;
	Rect R;
	string obj;
	int indx;
	int assert;
	double sim;
	double simcor;
	double simgrad;
	double simgray;
	double val;
};



typedef struct candidateregion
{
	Mat img_candidate;
	Rect Loc;
	string obj;
	double Val;
};


typedef struct Rectval
{
	Rect R;
	float val;
	int indx;
};


typedef struct SceneLoc
{
	Rect C;
	string object;
	int indx;
	int NumWindows;
	double Val;
	double ValGrad;
	double ValColor;

	double maxVal;
	double maxVal1;
	double maxVal2;
	double maxVal3;
	double maxVal4;
	double maxVal5;

};



typedef struct FoundObject
{
	Rect Crop;
	int maxVal;
	int firstValid;
	int valid;
	int obj;
	int indTmpl;
	int r;
	Mat img;
	int fase;
	double T;
};


typedef struct matchs
{
	int indx;
	double matchGrad;
	double matchHsv;
	double matchEdge;
	double matchVal;
	Rect matchbox;
};

typedef struct segmentos
{
	int indx_obj;
	Rect R;
	Mat bwseg;
	Mat gyseg;
	int area;
};

#endif // !JUDITECONFIG
