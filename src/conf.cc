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
#include "conf.h"
#include "logger.h"

Conf conf;

Conf::Conf() {
    conf_file_name = "";
    nodes_file_name = "";
    db_host = "";
    db_max_conn = 0;
    db_user = "";
    db_passwd = "";
    bot_name = "";
    status_message = "";
    save_data_file = "";
    save_data_tmp_file = "";
    save_img_path = "";
    avatar_filename = "";
    log_file_name = "";
    log_time_format = "";
    log_file_size = 0;
    log_level = "";
}

bool Conf::parse_option(int argc, char **argv) {
    po::options_description desc("Allowed options");

    desc.add_options()
            ("help,h", "produce help message")
            ("file,f", po::value<string>(&conf_file_name)->required(), "config.ini");
    po::variables_map vm;
    try {
        po::store(po::parse_command_line(argc, argv, desc), vm);
    }
    catch (...) {
        cout << "undefined option！\n";
        return false;
    }
    notify(vm);
    return true;
}

bool Conf::parse_conf(string file_name) {
    po::options_description desc("Allowed options");
    desc.add_options()
            ("nodes_file_name", po::value<string>(&nodes_file_name)->required(), "")
            ("host", po::value<string>(&db_host)->required(), "")
            ("max_conn", po::value<int>(&db_max_conn)->required(), "")
            ("user", po::value<string>(&db_user)->required(), "")
            ("passwd", po::value<string>(&db_passwd)->required(), "")
            ("db", po::value<string>(&db_name)->required(), "")
            ("bot_name", po::value<string>(&bot_name)->required(), "")
            ("status_message", po::value<string>(&status_message)->required(), "")
            ("save_data_file", po::value<string>(&save_data_file)->required(), "")
            ("save_data_tmp_file", po::value<string>(&save_data_tmp_file)->required(), "")
            ("save_img_path", po::value<string>(&save_img_path)->required(), "")
            ("avatar_filename", po::value<string>(&avatar_filename)->required(), "")
            ("log_file_name", po::value<string>(&log_file_name)->required(), "")
            ("log_time_format", po::value<string>(&log_time_format)->required(), "")
            ("log_file_size", po::value<int>(&log_file_size)->required(), "")
            ("log_level", po::value<string>(&log_level)->required(), "");
    po::variables_map vm;
    try {
        ifstream settings_file(file_name);
        store(parse_config_file(settings_file, desc), vm);
    }
    catch (...) {
        GLOBAL_LOGGER(error) << "file conf error";
        return false;
    }
    notify(vm);
    return true;
}

const string &Conf::getConf_file_name() const {
    return conf_file_name;
}

const string &Conf::get_nodes_file_name() const {
    return nodes_file_name;
}

void Conf::setConf_file_name(const string &conf_file_name) {
    Conf::conf_file_name = conf_file_name;
}

const string &Conf::getDb_host() const {
    return db_host;
}

void Conf::setDb_host(const string &db_host) {
    Conf::db_host = db_host;
}

int Conf::getDb_max_conn() const {
    return db_max_conn;
}

void Conf::setDb_max_conn(int db_max_conn) {
    Conf::db_max_conn = db_max_conn;
}

const string &Conf::getDb_user() const {
    return db_user;
}

void Conf::setDb_user(const string &db_user) {
    Conf::db_user = db_user;
}

const string &Conf::getDb_passwd() const {
    return db_passwd;
}

void Conf::setDb_passwd(const string &db_passwd) {
    Conf::db_passwd = db_passwd;
}

const string &Conf::getDb_name() const {
    return db_name;
}

void Conf::setDb_name(const string &db_name) {
    Conf::db_name = db_name;
}

const string &Conf::getBot_name() const {
    return bot_name;
}

void Conf::setBot_name(const string &bot_name) {
    Conf::bot_name = bot_name;
}

const string &Conf::getStatus_message() const {
    return status_message;
}

void Conf::setStatus_message(const string &status_message) {
    Conf::status_message = status_message;
}

const string &Conf::getSave_data_file() const {
    return save_data_file;
}

void Conf::setSave_data_file(const string &save_data_file) {
    Conf::save_data_file = save_data_file;
}

const string &Conf::getSave_data_tmp_file() const {
    return save_data_tmp_file;
}

void Conf::setSave_data_tmp_file(const string &save_data_tmp_file) {
    Conf::save_data_tmp_file = save_data_tmp_file;
}

const string &Conf::getSave_img_path() const {
    return save_img_path;
}

void Conf::setSave_img_path(const string &save_img_path) {
    Conf::save_img_path = save_img_path;
}

const string &Conf::getLog_file_name() const {
    return log_file_name;
}

void Conf::setLog_file_name(const string &log_file_name) {
    Conf::log_file_name = log_file_name;
}

const string &Conf::getLog_time_format() const {
    return log_time_format;
}

void Conf::setLog_time_format(const string &log_time_format) {
    Conf::log_time_format = log_time_format;
}

int Conf::getLog_file_size() const {
    return log_file_size;
}

void Conf::setLog_file_size(int log_file_size) {
    Conf::log_file_size = log_file_size;
}

const string &Conf::getLog_level() const {
    return log_level;
}

void Conf::setLog_level(const string &log_level) {
    Conf::log_level = log_level;
}

const string &Conf::get_avatar_filename() const {
    return avatar_filename;
}

