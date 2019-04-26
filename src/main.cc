#include <iostream>
#include "bot.h"
#include "conf.h"
#include "logger.h"
#include "db_conn.h"
#include "assisant.h"

int main(int argc, char *argv[]) {
    // init config file
    conf.parse_option(argc, argv);
    conf.parse_conf(conf.getConf_file_name());
    // severity_level: trace < debug < info < warning < error < fatal
    Logger logger(conf.getLog_file_name(),
                  conf.getLog_time_format(),
                  conf.getLog_file_size(),
                  conf.getLog_level());
    GLOBAL_LOGGER(info) << "conf file name: " << conf.getConf_file_name();
    // init db
    GLOBAL_LOGGER(info) << "host: " << conf.getDb_host() << " user: " << conf.getDb_user() << "passwd: "
                        << conf.getDb_passwd() << " max_conn: " << conf.getDb_max_conn();
    db_conn.init(conf.getDb_host(), conf.getDb_user(), conf.getDb_passwd(), conf.getDb_name(), conf.getDb_max_conn());
    GLOBAL_LOGGER(info) << "save_data_file: " << conf.getSave_data_file() << " save_data_tmp_file:"
                        << conf.getSave_data_tmp_file();
    Tox *tox = Bot::create_tox();
    GLOBAL_LOGGER(info) << "create_tox finish";
    Bot::bootstrap(tox);
    Bot::set_name(tox, conf.getBot_name());
    Bot::set_status_message(tox, conf.getStatus_message());
    GLOBAL_LOGGER(info) << " bot_name:" << conf.getBot_name() << " status_message: " << conf.getStatus_message();
    Bot::print_tox_id(tox);
    Bot::register_function(tox);
    Assisant::update_savedata_file(tox);

    while (1) {
        tox_iterate(tox, NULL);
        usleep(tox_iteration_interval(tox) * 100);
    }

    tox_kill(tox);
    return 0;
}
