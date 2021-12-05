#include "Utils.h"

#include<math.h>
#include<SearchEngine.h>


Utils::~Utils()
{
    //dtor
}



int Utils::Min(int x , int y) {
    if (x<y)
        return x ;
    else return y;
}

double Utils::distanceIntersection(int * H1 , int * H2 ,int rowNumber, int colNumber ) {
    double somme = 0 ;
    for (int i=0;i<256;i++)
        somme = somme + Min(H1[i],H2[i]);

    return (1-(somme/(rowNumber*colNumber)));
}



double Utils::distanceEuclidienne(int* Vect1,int* Vect2,int length)
{
    double result=0;
    for (int i=0;i<length;i++)
        result += pow((Vect1[i]-Vect2[i]),2);
    return sqrt(result);
}

Element * Utils::triSelection(Element * t , int taille) {
    int i,mini,j;
    Element x;

    for (i=0;i< taille-1;i++) {
        mini=i;
        for(j=i+1;j<taille;j++)
            if(t[j].distance<t[mini].distance)
                mini = j;
        x=t[i];
        t[i]=t[mini];
        t[mini]=x;
    }
    return &t[0];
}
