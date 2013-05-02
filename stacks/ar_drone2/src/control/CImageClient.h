#ifndef CIMAGECLIENT_H
#define CIMAGECLIENT_H

#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string.h>
#include <stdlib.h>
#include <iostream>
#include <fcntl.h>
#include <errno.h>
#include "../common/CRawImage.h"
#include "CDecoder.h"
#include "../common/CThread.h"
#include <semaphore.h>
#include "ros/ros.h"
#include "sensor_msgs/Image.h"

typedef enum
{
	CMD_NONE = 0,
	CMD_IMAGE,
	CMD_COMPRESS,
	CMD_START_TRACK,
	CMD_STOP_TRACK,
	CMD_QUIT,
	CMD_NUMBER
}
TCommandType;

/**
@author Tom Krajnik
*/
class CImageClient:CThread
{
public:
    CImageClient();
    ~CImageClient();

    int sendCommand(TCommandType type);
    int connectServer(char * ip,char* port);
    void run(CRawImage* image);
    int disconnectServer();
    CRawImage* image;
    void setPublisher(ros::Publisher *publisher);

private:
    int DoExecute();
    int socketNumber;
    CDecoder* codec;
    SDL_Thread * thread;
    bool stop,stopped;
    ros::Publisher *img_publisher;

};

#endif
