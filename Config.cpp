#include "Config.h"

#include <QApplication>

QString Config::DATABASE_DIR_PATH       = ALL_DATA_DIR_PATH + "/Data";
QString Config::DATABASE_FILE_PATH      = Config::DATABASE_DIR_PATH + "/Database.db3";

QString Config::CONFIG_DIR_PATH         = ALL_DATA_DIR_PATH + "/Config";
QString Config::CONFIG_APP_FILE_PATH    = Config::CONFIG_DIR_PATH + "/HandyKaraoke.conf";
QString Config::CONFIG_SYNTH_FILE_PATH  = Config::CONFIG_DIR_PATH + "/SynthMixer.conf";

void Config::initConfigDataPath()
{
    QDir dir(qApp->applicationDirPath() + "/Data");
    if (dir.exists()) {
        DATABASE_DIR_PATH = qApp->applicationDirPath() + "/Data";
        DATABASE_FILE_PATH = DATABASE_DIR_PATH + "/Database.db3";
    }

    dir.setPath(qApp->applicationDirPath() + "/Config");
    if (dir.exists()) {
        CONFIG_DIR_PATH = qApp->applicationDirPath() + "/Config";
        CONFIG_APP_FILE_PATH = CONFIG_DIR_PATH + "/HandyKaraoke.conf";
        CONFIG_SYNTH_FILE_PATH = CONFIG_DIR_PATH + "/SynthMixer.conf";
    }
}
