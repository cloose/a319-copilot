#ifndef _COM_INSTANCE_CREATION_FAILED_H_
#define _COM_INSTANCE_CREATION_FAILED_H_

#include <exception>
#include <string>
#include <windows.h>

class ComInstanceCreationFailed : public std::exception
{
public:
	explicit ComInstanceCreationFailed(HRESULT result);
	virtual ~ComInstanceCreationFailed() {}

	virtual const char* what() const throw();

private:
	std::string errorMessage;
};

#endif // _COM_INSTANCE_CREATION_FAILED_H_
