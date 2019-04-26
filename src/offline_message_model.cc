/****************************************************************************
MIT License

Copyright (c) 2019 TOK

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
****************************************************************************/
#include "offline_message_model.h"
#include "offline_message.h"

OfflineMessageModel offline_message_model;

OfflineMessageModel::OfflineMessageModel() {

}

OfflineMessageModel::~OfflineMessageModel() {

}

int OfflineMessageModel::add(const std::string &fr_pk,
                             const std::string &to_pk,
                             const std::string &content,
                             uint64_t local_msg_id,
                             uint64_t create_time
) {
    int ret = 0;
    sql::Connection *connection = db_conn.allocate();
    sql::PreparedStatement *pstmt;
    try {
        pstmt = connection->prepareStatement(
                "INSERT INTO t_offline_message(local_msg_id,fr_pk,to_pk,content,create_time) VALUES(?,?,?,?,?)");
        pstmt->setInt64(1, local_msg_id);
        pstmt->setString(2, fr_pk);
        pstmt->setString(3, to_pk);
        pstmt->setString(4, content);
        pstmt->setInt64(5, create_time);
        ret = pstmt->executeUpdate();
        pstmt->close();
        delete pstmt;
        db_conn.release(connection);
    }
    catch (sql::SQLException &e) {
        DBConn::show_sql_exception(e);
        delete pstmt;
        db_conn.release(connection);
    }
    return ret;
}

int OfflineMessageModel::del(const std::string &pk, uint64_t msg_id) {
    int ret = 0;
    sql::Connection *connection = db_conn.allocate();
    sql::PreparedStatement *pstmt;
    try {
        pstmt = connection->prepareStatement("DELETE FROM t_offline_message WHERE id <= ? and to_pk = ?");
        pstmt->setInt64(1, msg_id);
        pstmt->setString(2, pk);
        ret = pstmt->executeUpdate();
        pstmt->close();
        delete pstmt;
        db_conn.release(connection);
    }
    catch (sql::SQLException &e) {
        DBConn::show_sql_exception(e);
        delete pstmt;
        db_conn.release(connection);
    }
    return ret;
}

uint64_t OfflineMessageModel::get_latest_id(const std::string &pk) {
    sql::Connection *connection = db_conn.allocate();
    sql::PreparedStatement *pstmt;
    sql::ResultSet *res;
    uint64_t latest_id = 0;
    try {
        pstmt = connection->prepareStatement(
                "SELECT id FROM t_offline_message where to_pk = ? ORDER BY id DESC LIMIT 1");
        pstmt->setString(1, pk);
        res = pstmt->executeQuery();
        if (res->next()) {
            latest_id = res->getInt64("id");
        }
        pstmt->close();
        delete pstmt;
        delete res;
        db_conn.release(connection);
    }
    catch (sql::SQLException &e) {
        DBConn::show_sql_exception(e);
        delete pstmt;
        delete res;
        db_conn.release(connection);
    }
    return latest_id;
}

uint32_t OfflineMessageModel::get_count(const std::string &pk) {
    sql::Connection *connection = db_conn.allocate();
    sql::PreparedStatement *pstmt;
    sql::ResultSet *res;
    uint32_t count = 0;
    try {
        pstmt = connection->prepareStatement("SELECT count(id) as count FROM t_offline_message where to_pk = ?");
        pstmt->setString(1, pk);
        res = pstmt->executeQuery();
        if (res->next()) {
            count = res->getInt64("count");
        }
        pstmt->close();
        delete pstmt;
        delete res;
        db_conn.release(connection);
    }
    catch (sql::SQLException &e) {
        DBConn::show_sql_exception(e);
        delete pstmt;
        delete res;
        db_conn.release(connection);
    }
    return count;
}

void OfflineMessageModel::get_list(const std::string &pk, std::vector<OfflineMessage *> &vec, uint32_t limit) {
    sql::Connection *connection = db_conn.allocate();
    sql::PreparedStatement *pstmt;
    sql::ResultSet *res;
    uint32_t count = 0;
    try {
        pstmt = connection->prepareStatement(
                "SELECT id,local_msg_id,fr_pk,to_pk,content,create_time FROM t_offline_message where to_pk = ? ORDER BY ID ASC LIMIT ?");
        pstmt->setString(1, pk);
        pstmt->setInt64(2, limit);
        res = pstmt->executeQuery();
        while (res->next()) {
            OfflineMessage *msg = new OfflineMessage();
            if (msg) {
                msg->set_id(res->getInt64("id"));
                msg->set_local_msg_id(res->getInt64("local_msg_id"));
                msg->set_fr_pk(res->getString("fr_pk"));
                msg->set_to_pk(res->getString("to_pk"));
                msg->set_content(res->getString("content"));
                msg->set_create_time(res->getInt64("create_time"));
                vec.push_back(msg);
            }
        }
        pstmt->close();
        delete pstmt;
        delete res;
        db_conn.release(connection);
    }
    catch (sql::SQLException &e) {
        DBConn::show_sql_exception(e);
        delete pstmt;
        delete res;
        db_conn.release(connection);
    }

} 


