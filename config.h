#ifndef CONFIG_H
#define CONFIG_H

#include <QDir>

#ifdef Q_PROCESSOR_X86_64
#define VST_CHECKER_NAME "VSTChecker_x64.exe"
#else
#define VST_CHECKER_NAME "VSTChecker_x86.exe"
#endif

const QString VST_INFO_FILE_DIR     = QDir::tempPath() + "/HandyKaraoke";
const QString VST_INFO_FILE_PATH    = VST_INFO_FILE_DIR + "/vst.info";

#endif // CONFIG_H
