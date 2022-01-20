#include "SearchEngine.h"
#include<string.h>
#include<fstream>
#include<sstream>

using namespace std;
using namespace cv;

SearchEngine::SearchEngine(){}

SearchEngine::~SearchEngine(){}

bool SearchEngine::readPrecalculatedIndexesFromFiles(){
 //open the 2 csv files and read them , then copy their content to the in mermory indexes matrices
  std::ifstream histogram_indexes_file("histogram_indexes.csv");
  std::ifstream correlogram_indexes_file("correlogram_indexes.csv");
  int i=0;
  int j=0;
  string line1="";
  string line2="";
  vector<string> v1;
  vector<string> v2;
  while(getline(histogram_indexes_file,line1) && getline(correlogram_indexes_file,line2) && i<1000){
   //explode the line by the ';' separator and loop through it
    v1= this->ut.split(line1,";");
    v2= this->ut.split(line2,";");
    j=0;
    for(string& s1: v1){
       this->colorDescriptorIndexesDB[i][j++] = stoi(s1);
    }
    j=0;
    for(string& s2: v2){
       this->textureDescriptorIndexesDB[i][j++] = stoi(s2);
    }
   line1=line2="";
   i++;
  }
  histogram_indexes_file.close();
  correlogram_indexes_file.close();

  return true;
}


//OFFLINE WORK
bool SearchEngine::doIndexationProcess() {
   char  imgName[100];
   char  tmp[100];

   //here open 2 files histogram_indexes.csv & correlogram_indexes.csv
   std::ofstream histogram_indexes_file("histogram_indexes.csv");
   std::ofstream correlogram_indexes_file("correlogram_indexes.csv");

   Mat imgMatrix;
   int * hisImg;
   int * principalDiag;
   for(int i=0;i<1000;i++) {
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
            //append to the same file line (histogram index)
            histogram_indexes_file << hisImg[j];
            //append to the same file line (correlogram index)
            correlogram_indexes_file << principalDiag[j];
            if(j<255){
               histogram_indexes_file << ";";
               correlogram_indexes_file << ";";
            }

            this->colorDescriptorIndexesDB[i][j]=hisImg[j];
            this->textureDescriptorIndexesDB[i][j]=principalDiag[j];
          }
          if(i<999){
            histogram_indexes_file << "\n";
            correlogram_indexes_file << "\n";
          }
      }else{
          cout << "image with name "<< i+1 <<".jpg cannot be read in indexation process or doesnt exist !!!"<<endl;
      }
   }
   histogram_indexes_file.close();
   correlogram_indexes_file.close();

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
