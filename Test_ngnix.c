//Порт, который мы слушаем
#define PORT_NO 2222

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netinet/in.h>

int main(int argc, char *argv[])
{
        //Буфер, куда мы будем считывать данные из сокета
        long buffersize=50;
        int sockfd, newsockfd;
        socklen_t clilen;
        // Переменная, в которой будет храниться адрес нашего буфера
        char *buffer;
        struct sockaddr_in serv_addr, cli_addr;
        FILE * resultfile;
        // выделяем память
        buffer = malloc (buffersize+1);
        //открываем файл для записи наших сообщений        
        resultfile = fopen("/tmp/nginx_vs_apache.log","a");
        bzero((char *) &serv_addr, sizeof(serv_addr));
        bzero(buffer,buffersize+1);
        serv_addr.sin_family = AF_INET;
        serv_addr.sin_addr.s_addr = INADDR_ANY;
        serv_addr.sin_port = htons(PORT_NO);
        //создаем структуру (сокет), тут SOCK_STREAM это tcp/ip сокет.
        sockfd = socket(AF_INET, SOCK_STREAM, 0);
        if (sockfd < 0) error("ERROR opening socket");
        //определяем структуру нашего сокета, будем слушать порт 2222 на всех ip адресах
        if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) error("ERROR on binding");
        // говорим нашей ОС, чтобы она принимала входящие коннекты для нашего сокета, максимум 50
        listen(sockfd,50);
        while (1) {
                //в замкнутом цикле обрабатываем входящие подключения и читаем из них
                newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
                if (newsockfd < 0) error("ERROR on accept");
                read(newsockfd,buffer,buffersize);
                fprintf(resultfile, buffer);
                fflush (resultfile);
        }
        close(sockfd);
        return 0;
}