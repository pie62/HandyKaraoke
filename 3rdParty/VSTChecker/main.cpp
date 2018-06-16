#include <bass.h>
#include <bass_vst.h>

#include <iostream>

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        std::cout << "Invalid parameter, argc is " << argc << std::endl;
        return 3;
    }

    BASS_Init(-1, 0, 0, NULL, NULL);

    HSTREAM stream = BASS_StreamCreate(44100, 2, 0, NULL, NULL);

    #ifdef _WIN32
    DWORD vst = BASS_VST_ChannelSetDSP(stream, argv[1], BASS_VST_KEEP_CHANS, 0);
    #else
    DWORD vst = BASS_VST_ChannelSetDSP(stream, argv[1], BASS_VST_KEEP_CHANS, 0);
    #endif


    if (vst == 0)
    {
        std::cout << argv[1] << " is not VST file" << std::endl;
        BASS_StreamFree(stream);
        BASS_Free();
        return 2;
    }


    int returnCode;
    BASS_VST_INFO info;

    if (BASS_VST_GetInfo(vst, &info))
    {
        returnCode = 0;

        std::cout << info.uniqueID << std::endl;
        std::cout << info.effectName << std::endl;
        std::cout << info.vendorName << std::endl;
        std::cout << argv[1] << std::endl;
    }
    else
    {
        std::cout << "Can't get info from " << argv[1] << std::endl;
        returnCode = 1;
    }

    BASS_VST_ChannelRemoveDSP(stream, vst);
    BASS_StreamFree(stream);
    BASS_Free();

    return returnCode;
}
