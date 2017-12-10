Important!!: You are looking at the documents and some files from the Windows Version of BASS_VST other then the dynamic library file. (.dylib)
We haven't had time to port the docs yet but you should be able to figure it out from these....8)

If you'd like to help port these docs for OS X users please send a message to BaseHead, LLC  steve@baseheadinc.com



BASS_VST
===============================================================================

BASS_VST allows the usage of VST plugins in BASS.  BASS_VST was written to work
with Silverjuke(R).  Any other usage is for your own risk - but you're welcome
and I'm sure it will work :-)




Files that you should have found in this Package
===============================================================================

README.TXT      This file
bass_vst.lib    The BASS_VST import library to access 
libBass_vst.dylib  OS X plugin
bass_vst.h      BASS_VST C/C++ header file and documentation

Redistribution of this package is allowed if all files stay intact.


Usage in a glance
===============================================================================

Example:

    #include <bass.h>
    #include <bass_vst.h>

    // create a normal BASS stream
    DWORD ch = BASS_StreamCreateFile(false, "c:\\sth.mp3", 0, 0, 0);

    // assign a VST plugin DLL to the channel -- the returned hndle can be used
    // with all BASS_VST_*() functions
    DWORD dsp = BASS_VST_ChannelSetDSP(ch, "c:\\equalizer.dll", 0, 0);

    // that's all
    BASS_ChannelPlay(ch, false);

    // ... wait for finish ...

    // delete the stream - this will also delete the VST effect
    BASS_StreamFree(ch);



That's all for using VST effects, which will probably the most common usage.
However, you can also use VST instruments (VSTi plugins) with BASS_VST:

    // create a VST instrument stream -- the returned handle can be used with
    // all BASS_Channel*() and all BASS_VST_*() functions
    DWORD ch = BASS_VST_ChannelCreate(44100, 2, "c:\\instr.dll", 0);

    // the returned handle can be used like any other BASS channel handle -
    // eg. you can add a VST effect and start the stream
    BASS_VST_ChannelSetDSP(ch, "c:\\equalizer.dll", 0, 0);
    BASS_ChannelPlay(ch);

    // ... wait for finish ... trigger notes using BASS_VST_ProcessEvent() ...

    // when done, delete the VST instrument stream
    BASS_VST_ChannelFree(ch);




Further information
===============================================================================

For all functions and parameter descriptions, please refer to bass_vst.h -
another documentation is not available at the moment (and probably will never
be). Also note the version history and the disclaimer in this file.

BASS_VST was orginally created by
(C) Bjoern Petersen Software Design'n'Development
VST PlugIn Interface Technology by Steinberg Media Technologies GmbH
Contact for Windows Questions: drsilver@silverjuke.net - http://www.silverjuke.net

The OS X version was ported by BaseHead, LLC
For all BASS_VST OS X related questions email support@baseheadinc.com
http://baseheadinc.com

