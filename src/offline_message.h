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
#ifndef __OFFLINE_MESSAGE_H__
#define __OFFLINE_MESSAGE_H__

#include <string>

using namespace std;

class OfflineMessage {
public:
    static const size_t MAX_POOL_SIZE = 1024;
    typedef int _Key;

public:
    OfflineMessage(string fr_pk, string to_id, string content, uint64_t id, uint64_t local_msg_id,
                   uint64_t create_time);

    OfflineMessage();

    ~OfflineMessage();

    const string fr_pk() { return fr_pk_; }

    void set_fr_pk(string fr_pk) { fr_pk_ = fr_pk; }

    const string to_pk() { return to_pk_; }

    void set_to_pk(string to_pk) { to_pk_ = to_pk; }

    const string content() { return content_; }

    void set_content(string content) { content_ = content; }

    uint64_t id() { return id_; }

    void set_id(uint64_t id) { id_ = id; }

    uint64_t local_msg_id() { return local_msg_id_; }

    void set_local_msg_id(uint64_t local_msg_id) { local_msg_id_ = local_msg_id; }

    uint64_t create_time() { return create_time_; }

    void set_create_time(uint64_t create_time) { create_time_ = create_time; }

    void clear();

    size_t bytes_size();

private:
    string fr_pk_;
    string to_pk_;
    string content_;
    uint64_t id_;
    uint64_t local_msg_id_;
    uint64_t create_time_;
};

#endif
