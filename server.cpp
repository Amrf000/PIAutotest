#include "server.h"
#include <QDebug>

Server::Server():Eexit(0)
{
    sharedMemory.setKey("qvnc");
    if(sharedMemory.isAttached())
    {
        sharedMemory.detach();//将该进程与共享内存段分离
    }
    //int size = sizeof(SyscData);
    if(!sharedMemory.create(sizeof(SyscData)) && sharedMemory.error()!=QSharedMemory::AlreadyExists)
    {
        qDebug()<<"can't create memory segment";
        qDebug()<<sharedMemory.error();
    }
    //size = sharedMemory.size();
    //if(sharedMemory.isAttached())
    //{
    //    sharedMemory.detach();
    //}
}
Server::~Server()
{
}
void Server::stop()
{
    Eexit = 1;
}
void Server::run()
{
	while(true)
	{
        downShareMemory();
        if(localData.CDesktopWinState && localData.CDesktopWin){
           if(localData.CDesktopWinState==SyscData::C_CREATEWND)
           {
               emit this->onCDesktopWinCreate(localData.CDesktopWin);
               qDebug()<<"CDesktopWin=crt"<<localData.CDesktopWin;
           }else if(localData.CDesktopWinState==SyscData::C_SHOWWND){
               qDebug()<<"CDesktopWin=show"<<localData.CDesktopWin;
               emit this->onCDesktopWinShow(localData.CDesktopWin);
           }else{
               qDebug()<<"CDesktopWin=des"<<localData.CDesktopWin;
           }
           localData.CDesktopWinState = 0;

        }
        if(localData.ViewerState && localData.Viewer){
           if(localData.ViewerState==SyscData::C_CREATEWND)
           {
               emit this->onViewerCreate(localData.Viewer);
               qDebug()<<"Viewer=crt"<<localData.Viewer;
           }else if(localData.ViewerState==SyscData::C_SHOWWND){
               qDebug()<<"Viewer=show"<<localData.Viewer;
               emit this->onViewerShow(localData.Viewer);
           }else{
               qDebug()<<"Viewer=des"<<localData.Viewer;
           }
           localData.ViewerState = 0;
        }
        if(localData.ConnectionState && localData.Connection){
           if(localData.ConnectionState==SyscData::C_CREATEWND)
           {
               emit this->onConnectionCreate(localData.Connection);
               qDebug()<<"Connection=crt"<<localData.Connection;
           }else{
               qDebug()<<"Connection=des"<<localData.Connection;
           }
           localData.ConnectionState = 0;
        }
        if(localData.AuthenticationState && localData.Authentication){
           if(localData.AuthenticationState==SyscData::C_CREATEWND)
           {
               emit this->onAuthenticationCreate(localData.Authentication);
               qDebug()<<"Authentication=crt"<<localData.Authentication;
           }else{
               qDebug()<<"Authentication=des"<<localData.Authentication;
           }
           localData.AuthenticationState = 0;
        }
        if(Eexit==1 || localData.Eexit==1)
        {
            localData.Eexit = 1;
            upShareMemory();
            msleep(100);
            break;
        }
        upShareMemory();
        msleep(20);
	}
}
SyscData* Server::getLocalDataRef()//目前不需要考虑消息堆积
{
     return &localData;
}

void Server::downShareMemory()
{
    /*if(!sharedMemory.isAttached())
    {
        if (!sharedMemory.attach())
        {
            printf("Unable to attach to shared memory segment. downerror=%d\n",sharedMemory.error());
            return;
        }else{
            //printf("attach down success\n");
        }
    }*/
    sharedMemory.lock();
    memcpy((char*)&localData,(char*)sharedMemory.constData(), sizeof(SyscData));
    sharedMemory.unlock();
    //if (!sharedMemory.detach())
    //{
    //    printf("Unable to deattach to shared memory segment. downerr=%d\n",sharedMemory.error());
    //    return;
    //}else{
        //printf("deattach down success\n");
    //}
}

void Server::upShareMemory()
{
    /*if(!sharedMemory.isAttached())
    {
        if (!sharedMemory.attach())
        {
            printf( "Unable to attach to shared memory segment. uperr=%d\n",sharedMemory.error());
            return;
        }else{
            //printf( "attach up success\n");
        }
    }*/
    sharedMemory.lock();
    char *to = (char*)sharedMemory.data();
    const char *from = (char*)&localData;
    memcpy(to, from, sizeof(SyscData));
    sharedMemory.unlock();
    //if (!sharedMemory.detach())
    //{
    //    printf( "Unable to deattach to shared memory segment. uperr=%d\n",sharedMemory.error());
    //    return;
    //}else{
        //printf( "deattach up success\n");
    //}
}
