#ifndef INDEXATIONSTRATEGIES_H
#define INDEXATIONSTRATEGIES_H

#include<math.h>
#include<opencv2/opencv.hpp>

using namespace cv;

class IndexationStrategies
{
    public:
        IndexationStrategies();
        virtual ~IndexationStrategies();
        static int * createHistogram(const Mat * image);
        static int * createPrincipalDiagonalFromCorrelograme(const Mat * image);


    private:

};

#endif // INDEXATIONSTRATEGIES_H
