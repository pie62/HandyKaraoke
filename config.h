#ifndef CONFIG_H
#define CONFIG_H

#include <QDir>

#ifdef Q_PROCESSOR_X86_64
#define VST_CHECKER_NAME "VSTChecker_x64.exe"
#else
#define VST_CHECKER_NAME "VSTChecker_x86.exe"
#endif

static const QString ALL_DATA_DIR_PATH = QDir::homePath() + "/.HandyKaraoke";

//--------------------------------------------------------------------------------

static const QString DATABASE_DIR_PATH = ALL_DATA_DIR_PATH + "/Data";

static const QString DATABASE_FILE_PATH = DATABASE_DIR_PATH + "/Database.db3";

//--------------------------------------------------------------------------------

static const QString CONFIG_DIR_PATH        = ALL_DATA_DIR_PATH + "/Config";

static const QString CONFIG_APP_FILE_PATH   = CONFIG_DIR_PATH + "/HandyKaraoke.conf";

static const QString CONFIG_SYNTH_FILE_PATH = CONFIG_DIR_PATH + "/SynthMixer.conf";

//--------------------------------------------------------------------------------

#endif // CONFIG_H
