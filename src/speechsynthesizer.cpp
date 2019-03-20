#include "speechsynthesizer.h"

#include <XPLMUtilities.h>
#include "utils/ComInstanceCreationFailed.h"

SpeechSynthesizer::SpeechSynthesizer()
{
	HRESULT result = voice.CreateInstance(CLSID_SpVoice);
	if (FAILED(result)) {
		XPLMDebugString("Failed to create speech recognizer instance\n");
		throw ComInstanceCreationFailed(result);
	}
}

SpeechSynthesizer::~SpeechSynthesizer()
{
}

void SpeechSynthesizer::speak(const std::wstring & text)
{
	voice->Speak(text.c_str(), 0, NULL);
}
