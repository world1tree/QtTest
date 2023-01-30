//
// Created by zaiheshi on 1/29/23.
//

// You may need to build the project (run Qt uic code generator) to get "ui_MainWindow.h" resolved

#include "arpa/inet.h"
#include "Headers/mainwindow.h"
#include "Forms/ui_mainwindow.h"
#include <QMessageBox>
#include "sys/socket.h"

MainWindow::MainWindow(QWidget *parent) : QWidget(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
}

MainWindow::~MainWindow() {
    delete ui;
}

#pragma pack (1) //对齐方式,1字节对齐
//一些和网络通讯相关的结构放在这里
typedef struct _COMM_PKG_HEADER
{
    unsigned short pkgLen;    //报文总长度【包头+包体】--2字节，2字节可以表示的最大数字为6万多，我们定义_PKG_MAX_LENGTH 30000，所以用pkgLen足够保存下
    unsigned short msgCode;   //消息类型代码--2字节，用于区别每个不同的命令【不同的消息】
    int            crc32;     //CRC32效验--4字节，为了防止收发数据中出现收到内容和发送内容不一致的情况，引入这个字段做一个基本的校验用
}COMM_PKG_HEADER, *LPCOMM_PKG_HEADER;
typedef struct _STRUCT_REGISTER
{
    int           iType;          //类型
    char          username[56];   //用户名
    char          password[40];   //密码

}STRUCT_REGISTER, *LPSTRUCT_REGISTER;
typedef struct _STRUCT_LOGIN
{
    char          username[56];   //用户名
    char          password[40];   //密码

}STRUCT_LOGIN, *LPSTRUCT_LOGIN;
#pragma pack() //取消指定对齐，恢复缺省对齐

int sClient;   // 全局socket

void MainWindow::on_pushButton_clicked() {
    // 创建socket
    sClient = socket(AF_INET, SOCK_STREAM, 0);
    if (sClient == -1)
        return;
    // 连接服务器
    struct sockaddr_in serv_addr; // 服务器的地址结构体
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(9999);
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    if (::connect(sClient, (sockaddr*)&serv_addr, sizeof(sockaddr_in)) == -1)
        return;
    QMessageBox::about(this, "connect", "连接服务器成功");
//    int iSendRecvTimeOut = 5000;
//    if (setsockopt(sClient, SOL_SOCKET, SO_SNDTIMEO, (const char*)&iSendRecvTimeOut, sizeof(int)) == -1)
//        return;
//    if (setsockopt(sClient, SOL_SOCKET, SO_RCVTIMEO, (const char*)&iSendRecvTimeOut, sizeof(int)) == -1)
//        return;
}

//发送数据，值得讲解
int SendData(int sSocket, char *p_sendbuf, int ibuflen)
{
    int usend = ibuflen; //要发送的数目
    int uwrote = 0;      //已发送的数目
    int tmp_sret;

    while (uwrote < usend)
    {
        tmp_sret = send(sSocket, p_sendbuf + uwrote, usend - uwrote, 0);
        if ((tmp_sret == -1) || (tmp_sret == 0))
        {
            //有错误发生了
            return -1;
        }
        uwrote += tmp_sret;
    }//end while
    return uwrote;
}

int g_iLenPkgHeader = sizeof(COMM_PKG_HEADER);
void MainWindow::on_pushButton_2_clicked() {
    char *p_sendbuf = (char *)new char[g_iLenPkgHeader + sizeof(STRUCT_REGISTER)];

    LPCOMM_PKG_HEADER         pinfohead;
    pinfohead = (LPCOMM_PKG_HEADER)p_sendbuf;      // 同样是强制类型装换
    pinfohead->msgCode = 1;
    pinfohead->msgCode = htons(pinfohead->msgCode);
    pinfohead->crc32 = htonl(123); //测试，所以随便来 一个
    pinfohead->pkgLen = htons(g_iLenPkgHeader + sizeof(STRUCT_REGISTER));

    LPSTRUCT_REGISTER pstruc_sendstruc = (LPSTRUCT_REGISTER)(p_sendbuf + g_iLenPkgHeader);
    pstruc_sendstruc->iType = htonl(100);
    strcpy(pstruc_sendstruc->username, "1234");

    if (SendData(sClient, p_sendbuf, g_iLenPkgHeader + sizeof(STRUCT_REGISTER)) == -1)
    {
        QMessageBox::about(this, "send", "sendregister failed.");
        //测试程序也就不关闭socket，直接返回去算了
        delete[] p_sendbuf; //内存总还是要释放的
        return;
    }

    delete[] p_sendbuf; //释放上边的内存

    //再发个登录命令
    p_sendbuf = (char *)new char[g_iLenPkgHeader + sizeof(STRUCT_LOGIN)];

    //LPCOMM_PKG_HEADER         pinfohead;
    pinfohead = (LPCOMM_PKG_HEADER)p_sendbuf;
    pinfohead->msgCode = 2;  //变化一下
    pinfohead->msgCode = htons(pinfohead->msgCode);
    pinfohead->crc32 = htonl(345); //测试，所以随便来 一个
    pinfohead->pkgLen = htons(g_iLenPkgHeader + sizeof(STRUCT_LOGIN));

    LPSTRUCT_LOGIN pstruc_sendstruc2 = (LPSTRUCT_LOGIN)(p_sendbuf + g_iLenPkgHeader);
    strcpy(pstruc_sendstruc2->username, "5678");

    if (SendData(sClient, p_sendbuf, g_iLenPkgHeader + sizeof(STRUCT_LOGIN)) == -1)
    {
        //测试程序也就不关闭socket，直接返回去算了
        QMessageBox::about(this, "send", "sendlogin failed.");
        delete[] p_sendbuf; //内存总还是要释放的
        return;
    }
    delete[] p_sendbuf;
    //closesocket(sClient);
    QMessageBox::about(this, "send", "send two pack successfully.");

}
