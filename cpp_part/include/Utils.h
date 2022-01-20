#ifndef UTILS_H
#define UTILS_H
#include<string.h>
#include <fstream>
#include <sys/stat.h>
#include<vector>
#include<Element.h>

using namespace std;


class Utils {

private:
   const char * IMAGES_DB;
   const char * PATH_SEP;


public:
    Utils(){
       this->IMAGES_DB = "images_db";
       this->Utils::PATH_SEP = "/";
    }

    virtual ~Utils();

    int Min(int x , int y);
    vector<string> split(string s,string delimiter);
    double distanceIntersection(int * H1 , int * H2 ,int rowNumber, int colNumber ) ;
    double distanceEuclidienne(int* Vect1,int* Vect2,int length);

    Element * triSelection(Element * t , int taille) ;
    bool createFolderAndCopyTopTen(char ** imagesNames,char * resDirPath){
        char from[200];
        char to[200];
        if(mkdir(resDirPath,0777)==0){
            for(int i=0; i<10; i++) {
                strcpy(from,IMAGES_DB);
                strcat(from,PATH_SEP);
                strcat(from,imagesNames[i]);

                strcpy(to,resDirPath);
                strcat(to,PATH_SEP);
                strcat(to,imagesNames[i]);

                std::ifstream src(from, std::ios::binary);
                std::ofstream dest(to, std::ios::binary);
                dest << src.rdbuf();
            }
            return true;
        }
        return false;
    }
};

#endif // UTILS_H
