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
#ifndef __OFFLINE_MESSAGE_MODEL_H
#define __OFFLINE_MESSAGE_MODEL_H

#include "db_conn.h"
#include "logger.h"
#include <vector>

class OfflineMessage;

class OfflineMessageModel {
public:
    OfflineMessageModel();

    int add(const std::string &fr_pk,
            const std::string &to_pk,
            const std::string &content,
            uint64_t local_msg_id,
            uint64_t create_time
    );

    int del(const std::string &pk, uint64_t msg_id);

    uint64_t get_latest_id(const std::string &pk);

    uint32_t get_count(const std::string &pk);

    void get_list(const std::string &pk, std::vector<OfflineMessage *> &vec, uint32_t limit = 10);

    virtual ~OfflineMessageModel();
};

extern OfflineMessageModel offline_message_model;
#endif //__OFFLINE_MESSAGE_MODEL_H
