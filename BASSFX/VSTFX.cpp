#include "VSTFX.h"

VSTFX::VSTFX(const QString &vstFile, DWORD stream, int priority) : FX(priority)
{
    this->vstFile = vstFile;
    this->stream = stream;
    this->type = FXType::VSTEffects;

    if (stream != 0)
    {
        #ifdef _WIN32
        fx = BASS_VST_ChannelSetDSP(stream, vstFile.toStdWString().c_str(),
                                         BASS_VST_KEEP_CHANS|BASS_UNICODE, priority);
        #elif __APPLE__
        fx = BASS_VST_ChannelSetDSP(stream, vstFile.vstPath.toStdString().c_str(),
                                         BASS_VST_KEEP_CHANS, priority);
        #endif

        defaultProgramIndex = program();
        defaultParams = params();

        BASS_VST_INFO info;
        if (BASS_VST_GetInfo(fx, &info))
        {
            _uids = info.uniqueID;
        }
    }

    this->_on = true;
}

VSTFX::~VSTFX()
{
    if (stream != 0)
    {
        BASS_VST_ChannelRemoveDSP(stream, fx);
    }
}

bool VSTFX::isVSTFile(const QString &vstPath, BASS_VST_INFO *info)
{
    HSTREAM stream = BASS_StreamCreate(44100, 2, 0, NULL, NULL);

    #ifdef _WIN32
    DWORD h = BASS_VST_ChannelSetDSP(stream, vstPath.toStdWString().c_str(),
                                     BASS_VST_KEEP_CHANS|BASS_UNICODE, 0);
    #else
    DWORD h = BASS_VST_ChannelSetDSP(stream, vstPath.toStdString().c_str(),
                                     BASS_VST_KEEP_CHANS, 0);
    #endif

    bool result = false;

    if (BASS_VST_GetInfo(h, info) && !info->isInstrument)
        result = true;
    else
        result = false;

    BASS_VST_ChannelRemoveDSP(stream, h);
    BASS_StreamFree(stream);

    return result;
}

BASS_VST_INFO VSTFX::VSTInfo()
{
    BASS_VST_INFO info;
    BASS_VST_GetInfo(fx, &info);

    return info;
}

uint VSTFX::uids()
{
    return _uids;
}

int VSTFX::program()
{
    if (stream == 0)
    {
        return programIndex;
    }
    else
    {
        return BASS_VST_GetProgram(fx);
    }
}

void VSTFX::setProgram(int programIndex)
{
    if (stream ==0)
    {
        this->programIndex = programIndex;
    }
    else
    {
        BASS_VST_SetProgram(fx, programIndex);
    }
}

QByteArray VSTFX::chunk()
{
    if (stream == 0)
    {
        return tempChunk;
    }
    else
    {
        DWORD length = 0;
        char *cnk = BASS_VST_GetChunk(fx, false, &length);
        return QByteArray(cnk, length);
    }
}

void VSTFX::setChunk(const QByteArray &cnk)
{
    if (stream == 0)
    {
        this->tempChunk = cnk;
    }
    else
    {
        if (cnk.length() > 0) {
            BASS_VST_SetChunk(fx, false, cnk.constData(), cnk.length());
        }
    }
}

QList<float> VSTFX::params()
{
    if (stream == 0)
    {
        return tempParams;
    }
    else
    {
        QList<float> params;
        int count = BASS_VST_GetParamCount(fx);
        for (int i=0; i<count; i++) {
            params.append(BASS_VST_GetParam(fx, i));
        }
        return params;
    }
}

void VSTFX::setParams(const QList<float> &params)
{
    if (stream == 0)
    {
        tempParams.clear();
        tempParams = params;
    }
    else
    {
        for (int i=0; i<params.count(); i++) {
            BASS_VST_SetParam(fx, i, params[i]);
        }
    }
}

void VSTFX::setStreamHandle(DWORD stream)
{
    tempChunk = chunk();
    tempParams = params();
    programIndex = program();
    BASS_VST_ChannelRemoveDSP(this->stream, fx);
    this->stream = stream;

    if (stream != 0)
    {
        #ifdef _WIN32
        fx = BASS_VST_ChannelSetDSP(stream, vstFile.toStdWString().c_str(),
                                         BASS_VST_KEEP_CHANS|BASS_UNICODE, priority);
        #elif __APPLE__
        fx = BASS_VST_ChannelSetDSP(stream, vstFile.vstPath.toStdString().c_str(),
                                         BASS_VST_KEEP_CHANS, priority);
        #endif

        this->setBypass(this->isBypass());
        this->setChunk(tempChunk);
        this->setProgram(programIndex);
        this->setParams(tempParams);
    }
}

void VSTFX::setBypass(bool b)
{
    _on = !b;

    if (stream == 0)
        return;

    BASS_VST_SetBypass(fx, b);
}

void VSTFX::reset()
{
    setProgram(defaultProgramIndex);
    setParams(defaultParams);
}
