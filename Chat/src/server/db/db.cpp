#include "db.hpp"
#include <muduo/base/Logging.h>

// 数据库配置信息
static string server = "192.168.255.130";
static string user = "root";
static string password = "123456";
static string dbname = "chat";


MySQL::MySQL() {
    _conn = mysql_init(nullptr);
}

MySQL::~MySQL() {
    if(_conn != nullptr) mysql_close(_conn);
}

bool MySQL::connect() {
    MYSQL *p = mysql_real_connect(_conn, server.c_str(), user.c_str(), 
                                    password.c_str(), dbname.c_str(), 3306, nullptr, 0);
    if(p != nullptr) {
        // C/C++代码默认的编码字符是ASCII，如果不设置，从MySQL上拉下来的中文显示'?'
        mysql_query(_conn, "set names gbk");
        LOG_INFO << "connect mysql success!";
    } else {
        LOG_INFO << "connect mysql fail!";
    }
    return p;
}

bool MySQL::update(string sql) {
    if(mysql_query(_conn, sql.c_str())) {
        LOG_INFO << __FILE__ << ":" << __LINE__ << ":" << sql << "更新失败！";
        return false;
    }
    return true;
}

MYSQL_RES *MySQL::query(string sql) {
    if(mysql_query(_conn, sql.c_str())) {
        LOG_INFO << __FILE__ << ":" << __LINE__ << ":" << sql << "查询失败！";
        return nullptr;
    }
    return mysql_use_result(_conn);
}

MYSQL* MySQL::getConnection() {
    return _conn;
}