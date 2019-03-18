#include "cominstancecreationfailed.h"

#include <comdef.h>

static std::string GetComError(HRESULT result) {
	_com_error error(result);
	LPCTSTR errorMessage = error.ErrorMessage();

	char buffer[1024];
	_snprintf(buffer, sizeof(buffer), "ERROR: %ws\n", errorMessage);
	return buffer;
}

ComInstanceCreationFailed::ComInstanceCreationFailed(HRESULT result)
	: errorMessage(GetComError(result))
{
}

const char * ComInstanceCreationFailed::what() const throw()
{
	return errorMessage.c_str();
}
