#include <muduo/net/TcpServer.h>
#include <muduo/net/EventLoop.h>
#include <iostream>
#include <functional>
#include <string>
using namespace std;
using namespace muduo;
using namespace muduo::net;
using namespace placeholders;

class ChatServer {
public:
    ChatServer(EventLoop* loop,
            const InetAddress& listenAddr,
            const string& nameArg)
        : _server(loop, listenAddr, nameArg), _loop(loop) 
        {
            _server.setConnectionCallback(std::bind(&ChatServer::onConnetion, this, _1));
            _server.setMessageCallback(std::bind(&ChatServer::onMessage, this, _1, _2, _3));
            // 设置线程数量
            _server.setThreadNum(4);
        }

    void start() {_server.start();}
private:
    void onConnetion(const TcpConnectionPtr& conn) {
        if(conn->connected()) {
            cout << conn->peerAddress().toIpPort() << " -> "
                 << conn->localAddress().toIpPort() << "success" << endl;
        }
        else {
            cout << conn->peerAddress().toIpPort() << " -> "
                 << conn->localAddress().toIpPort() << "fail" << endl;
            conn->shutdown();
        }
    }
    void onMessage(const TcpConnectionPtr& conn,
                            Buffer* buffer,
                            Timestamp time) {
        string buf = buffer->retrieveAllAsString();
        cout << "recv data:" << buf << "time:" << time.toString() << endl;
        conn->send(buf);
    }
    TcpServer _server;
    EventLoop *_loop;
};


int main() {
    EventLoop loop;
    InetAddress addr("192.168.255.130", 6000);
    ChatServer server(&loop, addr,"ChatServer");
    server.start();
    loop.loop();
    return 0;
}