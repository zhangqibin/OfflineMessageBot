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
/*
 * implement tox register callback and tokeme base function
 */
#ifndef __ASSISANT_H
#define __ASSISANT_H

#include <string>
#include <tox/tox.h>

class Assisant {

public:
    Assisant();

    static void update_savedata_file(const Tox *tox);

    static void
    tox_friend_connection_status_callback(Tox *tox, uint32_t friend_number, TOX_CONNECTION connection_status,
                                          void *user_data);

    static void friend_request_cb(Tox *tox, const uint8_t *public_key, const uint8_t *message, size_t length,
                                  void *user_data);

    static uint8_t *get_name(Tox *tox, uint32_t friend_number);

    static uint8_t *get_status_message(Tox *tox, uint32_t friend_number);

    static void friend_message_cb(Tox *tox, uint32_t friend_number, TOX_MESSAGE_TYPE type, const uint8_t *message,
                                  size_t length, void *user_data);

    static void
    friend_status_message_cb(Tox *tox, uint32_t friend_number, const uint8_t *message, size_t length, void *user_data);

    static void friend_name_cb(Tox *tox, uint32_t friend_number, const uint8_t *name, size_t length, void *user_data);

    static void file_recv_cb(Tox *tox, uint32_t friend_number, uint32_t file_number, uint32_t kind, uint64_t file_size,
                             const uint8_t *filename, size_t filename_length, void *user_data);

    static void file_recv_chunk_cb(Tox *tox, uint32_t friend_number, uint32_t file_number, uint64_t position,
                                   const uint8_t *data, size_t length, void *user_data);

    static void file_chunk_request_cb(Tox *tox, uint32_t friend_number, uint32_t file_number, uint64_t position,
                                      size_t length, void *user_data);

    static char *get_data(std::string file_name, uint32_t &file_size);

    static void
    friend_message_offline_cb(Tox *tox, uint32_t friend_number, TOX_MESSAGE_OFFLINE_CMD cmd, const uint8_t *message,
                              size_t length, void *user_data);

    static void friend_read_receipt_cb(Tox *tox, uint32_t friend_number, uint32_t message_id, void *user_data);

    static void
    friend_send_message_offline(Tox *tox, uint32_t friend_number, uint16_t cmd, const uint8_t *message, size_t length);

    static void query_friend(Tox *tox, uint32_t friend_number, const uint8_t *message, size_t length);

    static void message_offline_recv(Tox *tox, uint32_t friend_number, const uint8_t *message, size_t length);

    static void message_offline_read_notice(Tox *tox, uint32_t friend_number);

    static void message_offline_pull(Tox *tox, uint32_t friend_number);

    static void message_offline_del(Tox *tox, uint32_t friend_number, const uint8_t *message, size_t length);

    virtual ~Assisant();

private:
    static char *avatar_data;
    static uint32_t avatar_data_size;
};

#endif //__ASSISANT_H
