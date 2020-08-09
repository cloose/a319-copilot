// Copyright (c) 2017, Philipp Ringler philipp@x-plane.com
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
// 1. Redistributions of source code must retain the above copyright notice, this
//    list of conditions and the following disclaimer.
// 2. Redistributions in binary form must reproduce the above copyright notice,
//    this list of conditions and the following disclaimer in the documentation
//    and/or other materials provided with the distribution.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
// ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
// WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
// DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
// ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
// (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
// LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
// ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
// SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//
// The views and conclusions contained in the software and documentation are those
// of the authors and should not be interpreted as representing official policies,
// either expressed or implied, of the FreeBSD Project.

#ifndef _DATAREF_H_
#define _DATAREF_H_

#include <stdexcept>
#include <string>
#include <vector>
#include <XPLM/XPLMDataAccess.h>

class DataRefLookupException : public std::runtime_error
{
public:
    explicit DataRefLookupException(const std::string& message) : std::runtime_error(message) {}
};

enum class ReadWriteType {
	ReadOnly = 1,
    WriteOnly = 2,
    ReadWrite = WriteOnly | ReadOnly
};

template <typename T>
struct dataref_trait {
    typedef T BasicType;
};

template <>
struct dataref_trait<std::vector<float>> {
    typedef float BasicType;
    std::vector<float> m_cache;
};

template <>
struct dataref_trait<std::vector<int>> {
    typedef int BasicType;
    std::vector<int> m_cache;
};

template <>
struct dataref_trait<std::string> {
    typedef char BasicType;
};

template <typename SimType>
class DataRef : private dataref_trait<SimType>
{
public:
    explicit DataRef(const std::string& identifier, ReadWriteType readWriteType = ReadWriteType::ReadOnly);
    virtual ~DataRef();

    operator SimType() const;
    const DataRef& operator=(const SimType& rhs);

    typename dataref_trait<SimType>::BasicType operator[](int index) const;
    void setValue(int index, typename dataref_trait<SimType>::BasicType value) { operator =(value); }

    DataRef(const DataRef&) = delete;

private:
    void lookUp();
    void checkDataType();
    void checkWriteability();

    std::string m_identifier;
    ReadWriteType m_readWriteType;
    XPLMDataRef m_dataRef;
};

template <typename SimType>
DataRef<SimType>::DataRef(const std::string& identifier, ReadWriteType readWriteType)
    : m_identifier(identifier)
    , m_readWriteType(readWriteType)
    , m_dataRef(nullptr)
{
    lookUp();
    checkDataType();
    checkWriteability();
}

template <typename SimType>
DataRef<SimType>::~DataRef()
{
}

template <typename SimType>
void DataRef<SimType>::lookUp()
{
    m_dataRef = XPLMFindDataRef(m_identifier.c_str());
    if (!m_dataRef) {
        throw DataRefLookupException(m_identifier + " not found in X-Plane's available datarefs.");
    }
}

template <typename SimType>
void DataRef<SimType>::checkDataType()
{
    throw DataRefLookupException("incompatible type");
}

template <typename SimType>
void DataRef<SimType>::checkWriteability()
{
    if (m_readWriteType == ReadWriteType::WriteOnly || m_readWriteType == ReadWriteType::ReadWrite) {
        if (!XPLMCanWriteDataRef(m_dataRef)) {
            throw DataRefLookupException("Declared to be writeable, but X-Plane says it is read-only.");
        }
    }
}

template <> void DataRef<int>::checkDataType();
template <> void DataRef<float>::checkDataType();
template <> void DataRef<std::vector<int>>::checkDataType();
template <> void DataRef<std::vector<float>>::checkDataType();
template <> void DataRef<std::string>::checkDataType();

template <> DataRef<int>::operator int() const;
template <> DataRef<float>::operator float() const;
template <> DataRef<std::vector<int>>::operator std::vector<int>() const;
template <> DataRef<std::vector<float>>::operator std::vector<float>() const;
template <> DataRef<std::string>::operator std::string() const;

template <> const DataRef<int>& DataRef<int>::operator=(const int&);
template <> const DataRef<float>& DataRef<float>::operator=(const float&);
template <> const DataRef<std::vector<int>>& DataRef<std::vector<int>>::operator=(const std::vector<int>&);
template <> const DataRef<std::vector<float>>& DataRef<std::vector<float>>::operator=(const std::vector<float>&);
template <> const DataRef<std::string>& DataRef<std::string>::operator=(const std::string&);

template <typename SimType>
typename dataref_trait<SimType>::BasicType DataRef<SimType>::operator[](int) const
{
    typedef typename dataref_trait<SimType>::BasicType T;
    return T(*this);
}

template<> dataref_trait<std::vector<float>>::BasicType DataRef<std::vector<float> >::operator[](int index) const;
template<> dataref_trait<std::vector<int>>::BasicType DataRef<std::vector<int> >::operator[](int index) const;
template<> dataref_trait<std::string>::BasicType DataRef<std::string>::operator[](int index) const;

template<> void DataRef<std::vector<int>>::setValue(int index, int value);
template<> void DataRef<std::vector<float>>::setValue(int index, float value);
template<> void DataRef<std::string>::setValue(int index, char value);

#endif // _DATAREF_H_
