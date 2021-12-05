#include "IndexationStrategies.h"

IndexationStrategies::IndexationStrategies(){}

IndexationStrategies::~IndexationStrategies(){}

int  * IndexationStrategies::createHistogram(const Mat * image)  {
  int N=256;
  int  * histogram = new int[N];
  for(int i=0;i<N;i++) histogram[i] = 0;

  for(int i=0; i<image->rows; i++)
    for(int j=0; j<image->cols; j++)
      histogram[(int)image->at<uchar>(i,j)]+=1;

  return histogram;
}



int * IndexationStrategies::createPrincipalDiagonalFromCorrelograme(const Mat * image)
{   int pixelNum=256;
    int rowNumber=image->rows;
    int colNumber=image->cols;
    //declaring Corrélogramme
    int ** correlogramme ;
    correlogramme = new int * [pixelNum];
    for (int i=0; i<pixelNum; ++i)
        correlogramme [i] = new int [pixelNum];

    // initializing Corrélogramme
    for(int r = 0; r < pixelNum; r++) {
        for(int c = 0; c < pixelNum; c++) {
            correlogramme[r][c] = 0;
        }
    }

    //Calculating Corrélogramme
    for(int r = 1; r < rowNumber-1 ; r++) {
        for(int c = 1; c < colNumber-1 ; c++) {

            int x1 = (int)image->at<uchar>(r,c);
            int y1 = (int)image->at<uchar>(r-1,c-1);

            int x2 = (int)image->at<uchar>(r,c);
            int y2 = (int)image->at<uchar>(r-1,c);

            int x3 = (int)image->at<uchar>(r,c);
            int y3 = (int)image->at<uchar>(r-1,c+1);

            int x4 = (int)image->at<uchar>(r,c);
            int y4 = (int)image->at<uchar>(r,c-1);

            int x5 = (int)image->at<uchar>(r,c);
            int y5 = (int)image->at<uchar>(r,c+1);

            int x6 = (int)image->at<uchar>(r,c);
            int y6 = (int)image->at<uchar>(r+1,c-1);

            int x7 = (int)image->at<uchar>(r,c);
            int y7 = (int)image->at<uchar>(r+1,c);

            int x8 = (int)image->at<uchar>(r,c);
            int y8 = (int)image->at<uchar>(r+1,c+1);

            correlogramme[x1][y1] ++;
            correlogramme[x2][y2] ++;
            correlogramme[x3][y3] ++;
            correlogramme[x4][y4] ++;
            correlogramme[x5][y5] ++;
            correlogramme[x6][y6] ++;
            correlogramme[x7][y7] ++;
            correlogramme[x8][y8] ++;
        }
    }

    //std::cout <<"Showing corrélogramme matrix"<<std::endl;
    //showMatrix(correlogramme,pixelNum,pixelNum);

    //populating Principal Vector
    int * principalDiagonal = new int [pixelNum];
    for (int i=0;i<pixelNum;i++)
        principalDiagonal[i]= correlogramme[i][i];

    return principalDiagonal;
}

