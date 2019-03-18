#ifndef _COM_HELPER_H
#define _COM_HELPER_H

#include <comdef.h>

template <class T>
class ComPtr
{
public:
    constexpr ComPtr() : m_comInterface(nullptr) {};
    ~ComPtr() {
        if (m_comInterface) 
            m_comInterface->Release();
    }

    HRESULT CreateInstance(REFCLSID classId) {
        return ::CoCreateInstance(classId, NULL, CLSCTX_ALL, __uuidof(T), (void**)&m_comInterface);
    }

    operator T*() const {
        return m_comInterface;
    }

    T** operator&() {
        return &m_comInterface;
    }
    
	T* operator->() const {
		return m_comInterface;
	}

	T* detach() {
		T* copy = m_comInterface;
		m_comInterface = nullptr;
		return copy;
	}

private:
    T* m_comInterface;
};

#endif // _COM_HELPER_