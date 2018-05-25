#ifndef CONFIG_H
#define CONFIG_H

#include <QDir>

#ifdef Q_PROCESSOR_X86_64
#define VST_CHECKER_NAME "VSTChecker_x64.exe"
#else
#define VST_CHECKER_NAME "VSTChecker_x86.exe"
#endif



#endif // CONFIG_H
