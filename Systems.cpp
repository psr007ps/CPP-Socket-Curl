#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <string.h> 
#include <string>
#include <sys/socket.h> 
#include <netinet/in.h> 
#include <netdb.h> 
#include <time.h>
#include <float.h>
#include <limits.h>


#define BUFFER_SIZE 1024

void error(const char *msg) { perror(msg); exit(0); }

void swap(double *p,double *q) {
   double t;
   
   t=*p; 
   *p=*q; 
   *q=t;
}

void sort(double a[],double n) { 
   int i,j;

   for(i = 0;i < n-1;i++) {
      for(j = 0;j < n-i-1;j++) {
         if(a[j] > a[j+1])
            swap(&a[j],&a[j+1]);
      }
   }
}

char * substring(char *string, char *from, char *to){
    
    char *first = strstr(string, from);
    if (first == NULL) {
        first = &string[0];
    } else {
        first += strlen(from);
    }
    char *last = strstr(first, to);
    if (last == NULL) {
        last = &string[strlen(string)];
    }
    char *sub = (char*)calloc(strlen(string) + 1, sizeof(char));
    strncpy(sub, first, last - first);
    return sub;
}

char * getHTTPRequest(char *host, char*path){
    // "GET /links HTTP/1.1\r\nHost: my-worker.pranshu.workers.dev\r\nConnection: close\r\n\r\n"
    char *request = (char*)calloc(100, sizeof(char));
    strcpy(request, "GET /");
    strcat(request, path);
    strcat(request, " HTTP/1.1\r\nHost: ");
    strcat(request, host);
    strcat(request, "\r\nConnection: close\r\n\r\n");
    return request;
}


int getRequest(char* url, char flag){

    int portno = 80;
    
    //char url[] = "https://my-worker.pranshu.workers.dev/links";
    // char* url = argv[1];
    //removing protocol
    char from[] = "://";
    char to[]   = " ";
    char *temp = substring(url, from, to);
    //extracting host
    char from1[] = " ";
    char to1[] = "/";
    char *host = substring(temp, from1, to1);
    //printf("host = %s\n", host);
    //extracting path
    char from2[] = "/";
    char to2[] = " ";
    char *path = substring(temp, from2, to2);
    //printf("path = %s\n", path);
    //creating HTTP request
    char *message_fmt = getHTTPRequest(host, path);
    //printf("\nrequest = %s\n", message_fmt);


    //char *message_fmt = "GET /links HTTP/1.1\r\nHost: my-worker.pranshu.workers.dev\r\nConnection: close\r\n\r\n";

    struct hostent *server;
    struct sockaddr_in serv_addr;
    int sockfd, bytes, sent, total;
    double received;
    char message[1024],response[4096];

    /* create the socket */
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) error("ERROR opening socket");

    /* lookup the ip address */
    server = gethostbyname(host);
    if (server == NULL) error("ERROR, no such host");

    /* fill in the structure */
    memset(&serv_addr,0,sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(portno);
    memcpy(&serv_addr.sin_addr.s_addr,server->h_addr,server->h_length);

    /* connect the socket */
    if (connect(sockfd,(struct sockaddr *)&serv_addr,sizeof(serv_addr)) < 0)
        error("ERROR connecting");


    /* send the request */
    total = strlen(message);
    sent = 0;
    
    bytes = write(sockfd,message_fmt,strlen(message_fmt));

    char buffer[BUFFER_SIZE];
    while(read(sockfd, buffer, BUFFER_SIZE - 1) != 0){
		if (flag == 'u'){
            printf("\n\n%s\n\n", buffer);
        }    
        bytes = read(sockfd, buffer, BUFFER_SIZE - 1); 
        if (bytes < 0){
            //printf("here");
            return -1;   
        }

        received += bytes;   
        
		bzero(buffer, BUFFER_SIZE);
	}

    /* close the socket */
    close(sockfd);
    return received;
}
int main(int argc,char *argv[])
{
    if (argc < 2){
        printf("\nhelp: \t\t\t ./a.out -h\n");
        return 0;
    }
    
    //printf("argv[%u] = %s\n", i, argv[i]);
    if (argv[1][0] == '-') {
       if (argv[1][1] == 'h') 
       {
           printf("\nhelp: \t\t\t ./a.out -h\n");
           printf("profile: \t\t ./a.out -p <complete url> <number of requests>\n");
           printf("GET Request: \t\t ./a.out -u <complete url>\n");
           printf("Example GET Request: \t ./a.out -u https://my-worker.pranshu.workers.dev/links\n\n");
       }
       else if (argv[1][1] == 'p')
       {
           if (argc < 4 || argc > 4){
                return 0;
            }
            char* url = argv[2];
            char* noOfRequests = argv[3];
            clock_t start, end;
            double timePerReq;
            double maxTime, minTime;
            double sum = 0, avg = 0;
            int minBytes = INT_MAX;
            int maxBytes = -1;
            int failed = 0, succeeded = atoi(noOfRequests);
            int numReq = atoi(noOfRequests);
            double arrTime[numReq];
            double med;
            for (int i = 0; i < atoi(noOfRequests); i++){
                start = clock();
                double received = getRequest(url, 'p');
                if (received == -1){
                    failed++;
                    succeeded--;
                    received = 0;
                }
                //printf("Response:\n%d\n",received);
                end = clock();
                timePerReq = ((double) (end - start)) / CLOCKS_PER_SEC;
                arrTime[i] = timePerReq;
                
                if (received > maxBytes){
                    maxBytes = received;
                }
                //printf("%f", received);
                if (received < minBytes){
                    minBytes = received;
                }
                sum += timePerReq;
                //printf("%f seconds", timePerReq);
           }
           if (minBytes == INT_MAX && maxBytes == -1){
               minBytes = 0;
               maxBytes = 0;
           }
            double num = atof(noOfRequests);

            sort(arrTime, numReq);
            minTime = arrTime[0];
            maxTime = arrTime[numReq - 1];
            med = arrTime[(numReq+1) / 2 - 1];
            //printf("median = %f", median);
            avg = sum / num;
            
            printf("\nNumber of requests = %d\n", numReq);
            printf("Succeeded = %d\n", succeeded);
            printf("Failed = %d\n", failed);
            printf("Slowest Time = %f seconds\n", maxTime);
            printf("Fastest Time = %f seconds\n", minTime);
            printf("Mean Time = %f seconds and Median Time = %f seconds\n", avg, med);
            printf("Max Bytes received = %d bytes\n", maxBytes);
            printf("Min Bytes received = %d bytes\n\n", minBytes);
            
       }
       else if (argv[1][1] == 'u')
       {
           if (argc > 3){
                return 0;
            }
           char* url = argv[2];
           getRequest(url, 'u');
       }
       else{
           
           printf("\nhelp: \t\t\t ./a.out -h\n\n");
           
       }
    } 
    else{
        printf("\nhelp: \t\t\t ./a.out -h\n\n");
    }
    return 0;
           
}