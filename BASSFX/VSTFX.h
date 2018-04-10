#ifndef VSTFX_H
#define VSTFX_H

#include "FX.h"
#include <bass_vst.h>

class VSTFX : public FX
{
public:
    VSTFX(const QString &vstFile, DWORD stream = 0, int priority = 1);
   ~VSTFX();

    BASS_VST_INFO VSTInfo();
    uint uids();
    int program();
    void setProgram(int programIndex);

    QList<float> params();
    void setParams(const QList<float> &params);
    void setStreamHandle(DWORD stream);
    void setBypass(bool b);
    void reset();

private:
    QString vstFile;

    int programIndex = 0, defaultProgramIndex = 0;
    QList<float> tempParams, defaultParams;
};

#endif // VSTFX_H
