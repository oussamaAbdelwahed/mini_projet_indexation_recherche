#include "SearchEngine.h"
#include<string.h>

using namespace std;
using namespace cv;

SearchEngine::SearchEngine(){}

SearchEngine::~SearchEngine(){}


//OFFLINE WORK
bool SearchEngine::doIndexationProcess() {
   char  imgName[100];
   char  tmp[100];

   Mat imgMatrix;
   int * hisImg;
   int * principalDiag;
   for(int i=0;i<=1000;i++) {
      strcpy(imgName,this->IMAGES_DB_PATH);
      strcat(imgName,this->PATH_SEP);

      sprintf(tmp,"%d",i+1);

      strcat(imgName,tmp);
      strcat(imgName,".jpg");
      imgMatrix=imread(imgName,CV_LOAD_IMAGE_GRAYSCALE);
      if(!imgMatrix.empty()){
          hisImg=this->indexationStrategies.createHistogram(&imgMatrix);
          principalDiag=this->indexationStrategies.createPrincipalDiagonalFromCorrelograme(&imgMatrix);

          for(int j =0 ; j<256;j++) {
            this->colorDescriptorIndexesDB[i][j]=hisImg[j];
            this->textureDescriptorIndexesDB[i][j]=principalDiag[j];
          }
      }else{
          cout << "image with name "<< i+1 <<".jpg cannot be read in indexation process or doesnt exist !!!"<<endl;
      }
   }
   return true;
}

//Online Work :
char ** SearchEngine::searchTopTenByTheGivenDescriptor(int descriptor,char * requestImgPath) {
    cout <<"DESCRIPTOR =  "<< descriptor <<endl;
    const Mat reqImgMat = imread(requestImgPath,CV_LOAD_IMAGE_GRAYSCALE);

    char ** topTen = new char * [10];
    Element results[1000];
    Element * sortedResults;

    int * index;
    if (descriptor==0) {
        index=this->is.createHistogram(&reqImgMat);
        for (int i=0;i<1000;i++) {
            results[i].distance=this->ut.distanceIntersection(index,this->colorDescriptorIndexesDB[i],360,480);
            results[i].index=i+1;
        }
        sortedResults= this->ut.triSelection(results,1000);
   }else{
        index=this->is.createPrincipalDiagonalFromCorrelograme(&reqImgMat);
        for (int i=0;i<1000;i++) {
            results[i].distance=this->ut.distanceEuclidienne(index,this->textureDescriptorIndexesDB[i],256);
            results[i].index=i+1;
        }
        sortedResults = this->ut.triSelection(results,1000);
    }
    char temporary[50];
    for(int i=0;i<10;i++) {
        cout << sortedResults[i].distance<< endl;
        topTen[i] = new char[10];
        sprintf(topTen[i],"%d",sortedResults[i].index);
        strcat(topTen[i],".jpg");

        strcat(topTen[i],"$");
        sprintf(temporary,"%0.7f",sortedResults[i].distance);
        strcat(topTen[i],temporary);

    }
    return topTen;
}
