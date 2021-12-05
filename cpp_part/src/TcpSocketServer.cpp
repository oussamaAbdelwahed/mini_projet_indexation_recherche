#include "TcpSocketServer.h"

using namespace std;



TcpSocketServer::TcpSocketServer(int p)
{

    this->PORT = p;
}

TcpSocketServer::~TcpSocketServer() {}


//INIT TCP SOCKET SERVER PARAMS
void TcpSocketServer::init()
{
    this->en.doIndexationProcess();
    cout << "FINISHED INDEXATION PROCESS !"<<endl;

    for (int i = 0; i < this->max_clients; i++){
        this->client_socket[i] = 0;
    }

    if( (this->master_socket = socket(AF_INET, SOCK_STREAM, 0)) == 0){
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    //set master socket to allow multiple connections ,
    //this is just a good habit, it will work without this
    if( setsockopt(this->master_socket, SOL_SOCKET, SO_REUSEADDR, (char *)&this->opt,sizeof(this->opt)) < 0 ){
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }

    //type of socket created
    this->address.sin_family = AF_INET;
    this->address.sin_addr.s_addr = INADDR_ANY;
    this->address.sin_port = htons(this->PORT);

    //bind the socket to localhost port 8888
    if (bind(this->master_socket, (struct sockaddr *)&this->address, sizeof(this->address)) <0 ){
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    printf("Listening on port %d \n", this->PORT);

    //try to specify maximum of MAX_PENDING_REQUESTS pending connections for the master socket
    if (listen(this->master_socket, MAX_PENDING_REQUESTS) < 0){
        perror("listen");
        exit(EXIT_FAILURE);
    }
    //accept the incoming connection
    addrlen = sizeof(this->address);
    puts("Waiting for connections ...");
}


//START TCP LISTENING SOCKET AND ACCEPT INCOMING CONNECTIONS
void TcpSocketServer::startDaemon(){
    char ** resultSet ;
    char * desciptorType;
    char * fullyQualifiedImagePath;
    char separator[3]=";;";
    char POINT_SEP[3]=".";
    char * tmp;
    char data[1026];

    while(TRUE){
        //clear the socket set
        FD_ZERO(&this->readfds);

        //add master socket to set
        FD_SET(this->master_socket, &this->readfds);
        this->max_sd = this->master_socket;

        //add child sockets to set
        for (int i = 0 ; i < this->max_clients ; i++)
        {
            //socket descriptor
            this->sd = this->client_socket[i];

            //if valid socket descriptor then add to read list
            if(this->sd > 0)
                FD_SET( this->sd, &this->readfds);

            //highest file descriptor number, need it for the select function
            if(this->sd > this->max_sd)
                this->max_sd = this->sd;
        }

        //wait for an activity on one of the sockets , timeout is NULL ,
        //so wait indefinitely
        this->activity = select(this->max_sd + 1, &this->readfds, NULL, NULL, NULL);

        if ((this->activity < 0) && (errno!=EINTR))
        {
            printf("select error");
        }

        //If something happened on the master socket ,
        //then its an incoming connection
        if (FD_ISSET(this->master_socket, &this->readfds))
        {
            if ((this->new_socket = accept(this->master_socket,(struct sockaddr *)&this->address, (socklen_t*)&this->addrlen))<0)
            {
                perror("accept");
                exit(EXIT_FAILURE);
            }

            //inform user of socket number - used in send and receive commands
            printf("New connection , socket fd is %d , ip is : %s , port : %d\n", this->new_socket, inet_ntoa(this->address.sin_addr), ntohs(this->address.sin_port));

            //add new socket to array of sockets
            for (int i = 0; i <this->max_clients; i++){
                //if position is empty
                if(this->client_socket[i] == 0 )
                {
                    this->client_socket[i] = this->new_socket;
                    printf("Adding to list of sockets as %d\n", i);

                    break;
                }
            }
        }

        //else its some IO operation on some other socket
        for (int i = 0; i < this->max_clients; i++){
            this->sd = this->client_socket[i];

            if (FD_ISSET( this->sd, &this->readfds))
            {
                //Check if it was for closing , and also read the
                //incoming message
                if ((this->valread = read( this->sd, this->buffer, 1024)) == 0)
                {
                    //Somebody disconnected , get his details and print
                    getpeername(this->sd, (struct sockaddr *)&this->address, (socklen_t*)&this->addrlen);
                    printf("Host disconnected , ip %s , port %d \n",inet_ntoa(this->address.sin_addr), ntohs(this->address.sin_port));
                }
                //Echo back the message that came in
                else
                {
                    //set the string terminating NULL byte on the end
                    //of the data read
                    this->buffer[this->valread] = '\0';
                    printf("Received data from client socket  = %s \n",this->buffer);

                    strcpy(data,this->buffer);

                    fullyQualifiedImagePath = strtok(data,separator);
                    desciptorType =  strtok(NULL,separator);
                    //std::cout<<fullyQualifiedImagePath<<" "<<desciptorType<<std::endl;
                    char ** resultingTenImagesNames = this->en.searchTopTenByTheGivenDescriptor(strcmp(desciptorType,"0") == 0 ? 0 : 1,fullyQualifiedImagePath);
                    tmp  = strtok(fullyQualifiedImagePath,".");

                    //this->en.ut.createFolderAndCopyTopTen(resultingTenImagesNames,tmp);

                    //try inside the SearchENgine.searchTopTenByTheGivenDescriptor to concatenate score with ':' separtor after each resulting image path
                    strcpy(this->buffer,"");
                    for(int l=0;l<10;l++){
                       strcat(this->buffer,resultingTenImagesNames[l]);
                       strcat(this->buffer,";");
                    }

                   // strcat(this->buffer,"DUMMY");
                    cout << buffer <<endl;
                    send(this->sd, this->buffer, strlen(this->buffer), 0);
                }
                //Close the socket and mark as 0 in list for reuse
                close(this->sd);
                this->client_socket[i] = 0;
            }
        }
    }
}


