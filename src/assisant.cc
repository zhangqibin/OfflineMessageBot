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
#include "assisant.h"
#include "logger.h"
#include "conf.h"
#include "util.h"
#include "offline_message.h"
#include "offline_message_model.h"
#include <boost/format.hpp>
#include <boost/algorithm/string.hpp>
#include "generate/message.pb.h"

using namespace std;

char *Assisant::avatar_data = nullptr;
uint32_t Assisant::avatar_data_size = 0;

Assisant::Assisant() {

}

void Assisant::tox_friend_connection_status_callback(Tox *tox, uint32_t friend_number, TOX_CONNECTION connection_status,
                                                     void *user_data) {
    string pk = Util::get_pk_by_id(tox, friend_number);
    GLOBAL_LOGGER(debug) << "connection_status: " << connection_status << " pk: " << pk;
    if (TOX_CONNECTION_NONE != connection_status) {
        // update bot avatar
        std::string file_name = conf.getSave_img_path() + conf.get_avatar_filename();
        if (nullptr == avatar_data || 0 == avatar_data_size) {
            avatar_data = get_data(file_name, avatar_data_size);
            GLOBAL_LOGGER(debug) << "tokme_bot_avatar size: " << avatar_data_size;
        }
        if (avatar_data) {
            uint8_t hash_data[TOX_HASH_LENGTH] = {};
            tox_hash(hash_data, (const uint8_t *) avatar_data, avatar_data_size);
            TOX_ERR_FILE_SEND error;
            tox_file_send(tox, friend_number, TOX_FILE_KIND_AVATAR, avatar_data_size, hash_data,
                          (const uint8_t *) file_name.c_str(), file_name.size(), &error);
            if (error != TOX_ERR_FILE_SEND_OK) {
                GLOBAL_LOGGER(error) << "tox_file_send error:" << error;
            }
        }
        message_offline_read_notice(tox, friend_number);
    }
    GLOBAL_LOGGER(info) << " total friend: " << tox_self_get_friend_list_size(tox);
}

void Assisant::friend_request_cb(Tox *tox, const uint8_t *public_key, const uint8_t *message, size_t length,
                                 void *user_data) {
    GLOBAL_LOGGER(debug) << "public_key:" << public_key;
    tox_friend_add_norequest(tox, public_key, NULL);
    update_savedata_file(tox);
}

void Assisant::friend_message_cb(Tox *tox, uint32_t friend_number, TOX_MESSAGE_TYPE type, const uint8_t *message,
                                 size_t length, void *user_data) {
    GLOBAL_LOGGER(debug) << "TOX_MESSAGE_TYPE:" << type;
    std::string msg(reinterpret_cast< char const * >(message), length);
}

void Assisant::file_recv_cb(Tox *tox, uint32_t friend_number, uint32_t file_number, uint32_t kind, uint64_t file_size,
                            const uint8_t *filename, size_t filename_length, void *user_data) {
    GLOBAL_LOGGER(debug) << "friend_number:" << friend_number << " file_number:" << file_number << " filename:"
                         << filename << " filename_length:" << filename_length << " kind:" << kind;
}

void Assisant::file_recv_chunk_cb(Tox *tox, uint32_t friend_number, uint32_t file_number, uint64_t position,
                                  const uint8_t *data, size_t length, void *user_data) {
    GLOBAL_LOGGER(debug) << "friend_number:" << friend_number << " file_number:" << file_number << " position:"
                         << position;
}

void Assisant::file_chunk_request_cb(Tox *tox, uint32_t friend_number, uint32_t file_number, uint64_t position,
                                     size_t length, void *user_data) {
    GLOBAL_LOGGER(debug) << "friend_number:" << friend_number << " file_number:" << file_number << " position:"
                         << position << " length:" << length;
    if (!length) {
        return;
    }
    std::string file_name = conf.getSave_img_path() + conf.get_avatar_filename();
    if (nullptr == avatar_data || 0 == avatar_data_size) {
        avatar_data = get_data(file_name, avatar_data_size);
        GLOBAL_LOGGER(debug) << "tokme_bot_avatar size: " << avatar_data_size;
    }
    // maybe (position + length) >= avatar_data_size ?
    if ((position + length) > avatar_data_size) {
        GLOBAL_LOGGER(error) << "position + length is more than avatar_data_size";
        return;
    }
    TOX_ERR_FILE_SEND_CHUNK err;
    uint8_t *byte_data = (uint8_t *) malloc(length);
    memset(byte_data, 0, length);
    memcpy(byte_data, avatar_data + position, length);
    bool ret = tox_file_send_chunk(tox, friend_number, file_number, position, byte_data, length, &err);
    if (err != TOX_ERR_FILE_SEND_CHUNK_OK && ret) {
        GLOBAL_LOGGER(error) << "tox_file_send_chunk error:" << err;
    }
    free(byte_data);
}

void Assisant::update_savedata_file(const Tox *tox) {
    size_t size = tox_get_savedata_size(tox);
    uint8_t *savedata = (uint8_t *) malloc(size);
    tox_get_savedata(tox, savedata);
    FILE *f = fopen(conf.getSave_data_tmp_file().c_str(), "wb");
    fwrite(savedata, size, 1, f);
    fclose(f);
    rename(conf.getSave_data_tmp_file().c_str(), conf.getSave_data_file().c_str());
    free(savedata);
}

char *Assisant::get_data(std::string file_name, uint32_t &file_size) {
    FILE *f = fopen(file_name.c_str(), "rb");
    uint32_t size = 0;
    char *buffer = nullptr;
    size_t result = 0;
    if (f == nullptr) {
        GLOBAL_LOGGER(error) << "File error:" << stderr;
        return buffer;
    }
    // obtain file size:
    fseek(f, 0, SEEK_END);
    size = ftell(f);
    rewind(f);

    // allocate memory to contain the whole file:
    buffer = (char *) malloc(sizeof(char) * size);
    if (buffer == nullptr) {
        GLOBAL_LOGGER(error) << "Memory error:" << stderr;
        return buffer;
    }

    // copy the file into the buffer:
    result = fread(buffer, 1, size, f);
    if (result != size) {
        GLOBAL_LOGGER(error) << "Reading error:" << stderr;
    }
    file_size = size;
    /* the whole file is now loaded in the memory buffer. */
    fclose(f);
    return buffer;
}

void Assisant::friend_name_cb(Tox *tox, uint32_t friend_number, const uint8_t *name, size_t length, void *user_data) {
    std::string pk = Util::get_pk_by_id(tox, friend_number);
    std::string tmp = (char *) name;
    // update local file
    update_savedata_file(tox);
    GLOBAL_LOGGER(debug) << "name " << tmp;
}

void Assisant::friend_status_message_cb(Tox *tox, uint32_t friend_number, const uint8_t *message, size_t length,
                                        void *user_data) {
    std::string pk = Util::get_pk_by_id(tox, friend_number);
    std::string status_message = (char *) message;
    // update local file
    update_savedata_file(tox);
    GLOBAL_LOGGER(debug) << "status message " << status_message;
}

uint8_t *Assisant::get_name(Tox *tox, uint32_t friend_number) {
    uint8_t *name = nullptr;
    TOX_ERR_FRIEND_QUERY error;
    size_t name_size = tox_friend_get_name_size(tox, friend_number, &error);
    if (TOX_ERR_FRIEND_QUERY_OK != error) {
        GLOBAL_LOGGER(error) << "tox_friend_get_name_size error:" << error;
        return name;
    }
    name = (uint8_t *) malloc(name_size + 1);
    memset(name, 0, name_size + 1);
    tox_friend_get_name(tox, friend_number, name, &error);
    if (TOX_ERR_FRIEND_QUERY_OK != error) {
        GLOBAL_LOGGER(error) << "tox_friend_get_name error:" << error;
        return name;
    }
    return name;
}


uint8_t *Assisant::get_status_message(Tox *tox, uint32_t friend_number) {
    uint8_t *status_message = nullptr;
    TOX_ERR_FRIEND_QUERY error;
    size_t status_message_size = tox_friend_get_status_message_size(tox, friend_number, &error);
    if (TOX_ERR_FRIEND_QUERY_OK != error) {
        GLOBAL_LOGGER(error) << "tox_friend_get_status_message_size error:" << error;
        return status_message;
    }
    status_message = (uint8_t *) malloc(status_message_size + 1);
    memset(status_message, 0, status_message_size + 1);
    tox_friend_get_status_message(tox, friend_number, status_message, &error);
    if (TOX_ERR_FRIEND_QUERY_OK != error) {
        GLOBAL_LOGGER(error) << "tox_friend_get_status_message error:" << error;
        return status_message;
    }
    return status_message;
}

void Assisant::friend_read_receipt_cb(Tox *tox, uint32_t friend_number, uint32_t message_id, void *user_data) {
    GLOBAL_LOGGER(debug) << "friend_number:" << friend_number << " message_id:" << message_id;
}

void Assisant::friend_send_message_offline(Tox *tox, uint32_t friend_number, uint16_t cmd, const uint8_t *message,
                                           size_t length) {
    TOX_ERR_FRIEND_SEND_MESSAGE errm;
    uint32_t message_id = tox_friend_send_message_offline(tox, friend_number, static_cast<TOX_MESSAGE_OFFLINE_CMD>(cmd),
                                                          message, length, &errm);
    if (TOX_ERR_FRIEND_SEND_MESSAGE_OK == errm) {
        GLOBAL_LOGGER(debug) << "ok" << " friend_nun:" << friend_number << " message_id:" << message_id << " cmd:"
                             << cmd;
    } else {
        GLOBAL_LOGGER(error) << "error:" << errm << " friend_nun:" << friend_number << " cmd:" << cmd;
    }
}

void Assisant::friend_message_offline_cb(Tox *tox, uint32_t friend_number, TOX_MESSAGE_OFFLINE_CMD cmd,
                                         const uint8_t *message,
                                         size_t length, void *user_data) {
    GLOBAL_LOGGER(debug) << " TOX_MESSAGE_OFFLINE_CMD:" << cmd;
    switch (cmd) {
        case TOX_MESSAGE_OFFLINE_QUERY_FRIEND_REQUEST: {
            query_friend(tox, friend_number, message, length);
            break;
        }
        case TOX_MESSAGE_OFFLINE_SEND_REQUEST: {
            message_offline_recv(tox, friend_number, message, length);
            break;
        }
        case TOX_MESSAGE_OFFLINE_PULL_REQUEST: {
            message_offline_pull(tox, friend_number);
            break;
        }
        case TOX_MESSAGE_OFFLINE_DEL_REQUEST: {
            message_offline_del(tox, friend_number, message, length);
            break;
        }
        default: {
            GLOBAL_LOGGER(error) << "invalid cmd " << cmd;
            break;
        }
    }
}

void Assisant::query_friend(Tox *tox, uint32_t friend_number, const uint8_t *message, size_t length) {
    im::tox::proto::QueryFriendReq req;
    bool ret = req.ParseFromArray(message, length);
    if (!ret) {
        GLOBAL_LOGGER(error) << "QueryFriendReq ParseFromArray error";
        return;
    }
    GLOBAL_LOGGER(debug) << "pk:" << req.pk();
    int friend_number_find = Util::get_id_by_pk(tox, req.pk());
    im::tox::proto::QueryFriendRes res;
    res.set_exist(friend_number_find == -1 ? 0 : 1);
    res.set_pk(req.pk());
    size_t size = res.ByteSizeLong();
    void *buffer = malloc(size);
    res.SerializeToArray(buffer, size);
    friend_send_message_offline(tox, friend_number, TOX_MESSAGE_OFFLINE_QUERY_FRIEND_RESPONSE,
                                static_cast<const uint8_t *>(buffer), size);
    free(buffer);
    buffer = nullptr;
}

void Assisant::message_offline_recv(Tox *tox, uint32_t friend_number, const uint8_t *message, size_t length) {
    im::tox::proto::OfflineMessageReq req;
    bool ret = req.ParseFromArray(message, length);
    if (!ret) {
        GLOBAL_LOGGER(error) << "OfflineMessageReq ParseFromArray error";
        return;
    }
    string fr_pk = Util::get_pk_by_id(tox, friend_number);
    GLOBAL_LOGGER(debug) << "fr_pk:" << fr_pk << " to_pk:" << req.to_pk();
    offline_message_model.add(fr_pk, req.to_pk(), req.crypto_message(), req.local_msg_id(), tox_unixtime());

    im::tox::proto::OfflineMessageReq res;
    res.set_local_msg_id(req.local_msg_id());
    size_t size = res.ByteSizeLong();
    void *buffer = malloc(size);
    res.SerializeToArray(buffer, size);
    friend_send_message_offline(tox, friend_number, TOX_MESSAGE_OFFLINE_SEND_RESPONSE,
                                static_cast<const uint8_t *>(buffer), size);
    free(buffer);
    buffer = nullptr;

}

void Assisant::message_offline_read_notice(Tox *tox, uint32_t friend_number) {
    string pk = Util::get_pk_by_id(tox, friend_number);
    GLOBAL_LOGGER(debug) << "pk:" << pk;
    uint64_t latest_id = offline_message_model.get_latest_id(pk);
    if (0 != latest_id) {
        im::tox::proto::OfflineMessageReadNotice notice;
        notice.set_latest_msg_id(latest_id);
        size_t size = notice.ByteSizeLong();
        void *buffer = malloc(size);
        notice.SerializeToArray(buffer, size);
        friend_send_message_offline(tox, friend_number, TOX_MESSAGE_OFFLINE_READ_NOTICE,
                                    static_cast<const uint8_t *>(buffer), size);
        free(buffer);
        buffer = nullptr;
    }
}

void Assisant::message_offline_pull(Tox *tox, uint32_t friend_number) {
    string to_pk = Util::get_pk_by_id(tox, friend_number);
    std::vector<OfflineMessage *> vec;
    offline_message_model.get_list(to_pk, vec);
    uint32_t total_count = offline_message_model.get_count(to_pk);
    uint32_t total_size = 0;
    uint32_t max_size = 1024;
    uint32_t count = 0;
    im::tox::proto::OfflineMessagePullRes res;
    im::tox::proto::OfflineMessage *msg = nullptr;
    GLOBAL_LOGGER(debug) << "pk:" << to_pk << " total_count:" << total_count;
    for (std::vector<OfflineMessage *>::iterator it = vec.begin(); it != vec.end(); ++it) {
        if (*it) {
            total_size += (*it)->bytes_size();
            if (total_size < max_size) {
                msg = res.add_msg();
                msg->set_local_msg_id((*it)->local_msg_id());
                msg->set_msg_id((*it)->id());
                msg->set_fr_pk((*it)->fr_pk());
                msg->set_to_pk((*it)->to_pk());
                msg->set_content((*it)->content());
                msg->set_create_time((*it)->create_time());
                count++;
            }
            delete (*it);
            (*it) = nullptr;
        }
    }
    res.set_left_count(total_count - count);
    size_t size = res.ByteSizeLong();
    void *buffer = malloc(size);
    if (buffer) {
        res.SerializeToArray(buffer, size);
        friend_send_message_offline(tox, friend_number, TOX_MESSAGE_OFFLINE_PULL_RESPONSE,
                                    static_cast<const uint8_t *>(buffer), size);
        free(buffer);
        buffer = nullptr;
    }
}

void Assisant::message_offline_del(Tox *tox, uint32_t friend_number, const uint8_t *message, size_t length) {
    im::tox::proto::OfflineMessageDelReq req;
    bool ret = req.ParseFromArray(message, length);
    if (!ret) {
        GLOBAL_LOGGER(error) << "OfflineMessageReq ParseFromArray error";
        return;
    }
    string to_pk = Util::get_pk_by_id(tox, friend_number);
    offline_message_model.del(to_pk, req.last_msg_id());
    GLOBAL_LOGGER(debug) << "to_pk:" << to_pk << " last_msg_id:" << req.last_msg_id();

}


Assisant::~Assisant() {
    free(avatar_data);
}
