#ifndef _SPEECH_SYNTHESIZER_H_
#define _SPEECH_SYNTHESIZER_H_

#include <string>
#include <windows.h>
#include <sapi.h>
#include "utils/comhelper.h"

class SpeechSynthesizer
{
public:
	SpeechSynthesizer();
	~SpeechSynthesizer();

	void speak(const std::wstring& text);

private:
	ComPtr<ISpVoice> voice;
};

#endif // _SPEECH_SYNTHESIZER_H_