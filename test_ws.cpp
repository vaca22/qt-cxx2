//
// Created by ligh1 on 5/16/2023.
//

#include "test_ws.h"
#include <iostream>

#include "libwebsockets.h"
#include "pthread.h"

static lws *wsi = NULL;
static bool established = false;
static bool isBreak = false;
static bool stop = false;


int UnmarshalURL(const char *_url, std::string &_protocol, std::string &_host, int &_port, std::string &path)
{
    std::string url(_url);
    int pslash = url.find("//", 0);
    if (pslash >= 0)
    {
        _protocol = url.substr(0, pslash - 1);
        url = url.substr(pslash + 2);
    }

    pslash = url.find(':');
    if (pslash < 0)
    {
        //没有端口号
        _port = -1;
        pslash = url.find('/', 0);
        if (pslash < 0)
        {
            //没有path
            _host = url;
        }
        else
        {
            //有path
            _host = url.substr(0, pslash);
            path = url.substr(pslash);
        }
    }
    else
    {
        //有端口号
        _host = url.substr(0, pslash);
        url = url.substr(pslash + 1);
        pslash = url.find('/', 0);
        if (pslash < 0)
        {
            //没有path
            _port = atoi(url.c_str());
        }
        else
        {
            //有path
            _port = atoi(url.substr(0, pslash).c_str());
            path = url.substr(pslash);
        }
    }
    return 0;
}

//记录接收10次服务器返回
static int recvSum = 0;
lws *_wsi_local=NULL;




// lws消息回调函数
int ws_callback(lws *_wsi, enum lws_callback_reasons _reasons, void *_user, void *_in, size_t _len)
{
    printf("CALLBACK REASON: %d\n", _reasons);

    //发送或者接受buffer，建议使用栈区的局部变量，lws会自己释放相关内存
    //如果使用堆区自定义内存空间，可能会导致内存泄漏或者指针越界
    char buffer[2560];
    memset(buffer, 0, 2560);

    switch (_reasons)
    {
        case LWS_CALLBACK_CLIENT_ESTABLISHED:
            //连接成功时，会触发此reason
            printf("established\n");
            //调用一次lws_callback_on_writeable，会触发一次callback的LWS_CALLBACK_CLIENT_WRITEABLE，之后可进行一次发送数据操作
            lws_callback_on_writable(_wsi);
            break;
        case LWS_CALLBACK_CLIENT_CLOSED:
            // 客户端主动断开、服务端断开都会触发此reason
            isBreak = true; // ws关闭，发出消息，退出消息循环
            printf("ws closed\n");
            break;
        case LWS_CALLBACK_CLIENT_CONNECTION_ERROR:
            //连接失败、异常
            printf("connect error\n");
            break;
        case LWS_CALLBACK_CLIENT_RECEIVE:
            //获取到服务端的数据
            memcpy(buffer, _in, _len);
            printf("recv: %s\n", buffer);


            break;
        case LWS_CALLBACK_CLIENT_WRITEABLE:
            //调用lws_callback_on_writeable，会触发一次此reason
            if (stop)
                break;
            recvSum++;
            if (recvSum >= 10)
            {
                stop = true;
                printf("will close\n");
                //使用lws_close_reason来准备断开连接的断开信息
                lws_close_reason(_wsi, LWS_CLOSE_STATUS_GOINGAWAY, NULL, 0);
                //当callback中return非0 时，则会主动断开websocket
                return -1;
            }
            _wsi_local=_wsi;
            break;

    }
    return 0;
}

void *test_ws_main(void *arg) {



    char *url = "ws://vaca.vip:3003/ws?phone=13207759669";
    printf("des URL:%s\n", url);
    std::string protocol; //ws/wss协议
    std::string host;     //主机IP
    int port;             //端口
    std::string path;     //path
    //解析URL的参数
    UnmarshalURL(url, protocol, host, port, path);

    bool ssl = protocol == "wss" ? true : false; //确认是否进行SSL加密

    //lws初始化阶段
    struct lws_context_creation_info info; //websocket 配置参数
    struct lws_context *context;           //websocket 连接上下文
    struct lws_client_connect_info ci;     //websocket 连接信息

    //建议初始化全部置为0
    memset(&info, 0, sizeof(info));
    memset(&ci, 0, sizeof(ci));

    struct lws_protocols lwsprotocol[2];
    //ws处理协议初始化时，建议将所有内存空间置0
    memset(&lwsprotocol[0], 0, sizeof(lws_protocols));
    memset(&lwsprotocol[1], 0, sizeof(lws_protocols));
    lwsprotocol[0].name = "ws-client";
    lwsprotocol[0].callback = ws_callback; //设置回调函数
    lwsprotocol[0].user = NULL;
    lwsprotocol[0].tx_packet_size = 5120;
    lwsprotocol[0].rx_buffer_size = 5120;
    lwsprotocol[1].name = NULL;
    lwsprotocol[1].callback = NULL;

    info.protocols = lwsprotocol;       //设置处理协议
    info.port = CONTEXT_PORT_NO_LISTEN; //作为ws客户端，无需绑定端口
    //ws和wss的初始化配置不同
    info.options = ssl ? LWS_SERVER_OPTION_DO_SSL_GLOBAL_INIT : 0; //如果是wss，需要做全局SSL初始化

    context = lws_create_context(&info); //创建连接上下文
    if (context == NULL)
    {
        printf("create context error\n");
        return nullptr;
    }

    //初始化连接信息
    ci.context = context;      //设置上下文
    ci.address = host.c_str(); //设置目标主机IP
    ci.port = port;            //设置目标主机服务端口
    ci.path = path.c_str();    //设置目标主机服务PATH
    ci.host = ci.address;      //设置目标主机IP
    ci.origin = ci.address;    //设置目标主机IP
    ci.pwsi = &wsi;            //设置wsi句柄
    ci.userdata = NULL;        //userdata 指针会传递给callback的user参数，一般用作自定义变量传入
    ci.protocol = lwsprotocol[0].name;

    //ws/wss需要不同的配置
    ci.ssl_connection = ssl ? (LCCSCF_USE_SSL | LCCSCF_ALLOW_SELFSIGNED | LCCSCF_SKIP_SERVER_CERT_HOSTNAME_CHECK | LCCSCF_ALLOW_INSECURE) : 0;

    lws_client_connect_via_info(&ci); //使连接信息生效

    //进入消息循环
    while (!isBreak)
    {
        lws_service(context, 500);
    }
    printf("ws disconnect\n");
    return 0;
}


void start_test_ws() {
    //create a pthread with stack size 2M
    pthread_t tid;
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_attr_setstacksize(&attr, 2 * 1024 * 1024);
    pthread_create(&tid, &attr, test_ws_main, NULL);
}