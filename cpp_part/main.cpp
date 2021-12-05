#include <stdio.h>

#include<TcpSocketServer.h>
#include<Utils.h>
#include<SearchEngine.h>
#include<string.h>

using namespace std;





int main()
{
   TcpSocketServer * server = new TcpSocketServer(8888);
    server->init();
    server->startDaemon();

     /*Utils ut;
     char * tab[10];
     char imName[100];
     tab[0] = "1.jpg";
     tab[1] = "2.jpg";
     tab[2] = "3.jpg";
     tab[3] = "4.jpg";
     tab[4] = "5.jpg";
     tab[5] = "6.jpg";
     tab[6] = "7.jpg";
     tab[7] = "8.jpg";
     tab[8] = "9.jpg";
     tab[9] = "10.jpg";
     ut.createFolderAndCopyTopTen(tab,"/home/oussama/RES_DIR");*/

    // SearchEngine en;
     //en.doIndexationProcess();

    return 0;
}
