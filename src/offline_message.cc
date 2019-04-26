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
#include "offline_message.h"
#include "generate/message.pb.h"

OfflineMessage::OfflineMessage(string fr_pk, string to_id, string content, uint64_t id, uint64_t local_msg_id,
                               uint64_t create_time) :
        fr_pk_(fr_pk), to_pk_(to_id), content_(content), id_(id), local_msg_id_(local_msg_id),
        create_time_(create_time) {
}

OfflineMessage::OfflineMessage() {
    id_ = 0;
    local_msg_id_ = 0;
    create_time_ = 0;
}

OfflineMessage::~OfflineMessage() {

}

void OfflineMessage::clear() {
    set_fr_pk("");
    set_to_pk("");
    set_content("");
    set_id(0);
    set_local_msg_id(0);
    set_create_time(0);
}

size_t OfflineMessage::bytes_size() {
    im::tox::proto::OfflineMessage msg;
    msg.set_local_msg_id(local_msg_id_);
    msg.set_msg_id(id_);
    msg.set_fr_pk(fr_pk_);
    msg.set_to_pk(to_pk_);
    msg.set_content(content_);
    msg.set_create_time(create_time_);
    return msg.ByteSizeLong();
}
