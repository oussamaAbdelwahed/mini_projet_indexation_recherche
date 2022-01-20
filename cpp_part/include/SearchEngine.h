#ifndef SEARCHENGINE_H
#define SEARCHENGINE_H
#include<IndexationStrategies.h>
#include<Utils.h>


class SearchEngine
{
public:
    SearchEngine();
    virtual ~SearchEngine();
    Utils ut;

    //OFFLINE WORK
    bool doIndexationProcess();

    bool readPrecalculatedIndexesFromFiles();

    //int * indexUsingHistogram(char * imageAbsolutePath);

    //int * indexUsingCorrelogram(char * imageAbsolutePath);


    //ONLINE WORK
    char ** searchTopTenByTheGivenDescriptor(int descriptor,char * requestImgPath);

private:

    char * IMAGES_DB_PATH = "images_db";
    char * PATH_SEP = "/";
    int colorDescriptorIndexesDB[1000][256];
    IndexationStrategies is;

    int textureDescriptorIndexesDB[1000][256];
    IndexationStrategies indexationStrategies;
};

#endif // SEARCHENGINE_H
