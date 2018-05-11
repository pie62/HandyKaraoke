/*
  ==============================================================================

    VSTChecker.h
    Created: 11 May 2018 10:09:52am
    Author:  Noob

  ==============================================================================
*/
#ifndef VSTCHECKER_H
#define VSTCHECKER_H

#include <string>

typedef struct __declspec(dllexport)
{
	int			uid;
	std::string	name;
	std::string	descriptiveName;
	std::string manufacturerName;
	std::string pluginFormatName;
	std::string category;
	std::string version;
	std::string fileOrIdentifier;
	int 		numInputChannels;
	int 		numOutputChannels;
	bool 		isInstrument;
	bool 		hasSharedContainer;

} VSTDescription;

class __declspec(dllexport) VSTChecker
{
public:
	static bool isVST2File(const char *filepath, VSTDescription *description);
	static bool isVST2File(const wchar_t *filepath, VSTDescription *description);
	static bool isVST2File(const std::string &filepath, VSTDescription *description);
};

#endif // VSTCHECKER_H