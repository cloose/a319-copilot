#include "dataref.h"

template <>
void DataRef<int>::checkDataType()
{
    if (XPLMGetDataRefTypes(m_dataRef) != xplmType_Int) {
        throw DataRefLookupException(m_identifier + " declared to be int, but isn't.");
    }
}

template <>
void DataRef<float>::checkDataType()
{
    if (XPLMGetDataRefTypes(m_dataRef) != xplmType_Float) {
        throw DataRefLookupException(m_identifier + " declared to be float, but isn't.");
    }
}

template <>
void DataRef<std::vector<int>>::checkDataType()
{
    if (XPLMGetDataRefTypes(m_dataRef) != xplmType_IntArray) {
        throw DataRefLookupException(m_identifier + " declared to be int array, but isn't.");
    }
}

template <>
void DataRef<std::vector<float>>::checkDataType()
{
    if (XPLMGetDataRefTypes(m_dataRef) != xplmType_FloatArray) {
        throw DataRefLookupException(m_identifier + " declared to be float array, but isn't.");
    }
}

template <>
void DataRef<std::string>::checkDataType()
{
    if (XPLMGetDataRefTypes(m_dataRef) != xplmType_Data) {
        throw DataRefLookupException(m_identifier + " declared to be byte array, but isn't.");
    }
}

template <>
DataRef<int>::operator int() const
{
    return XPLMGetDatai(m_dataRef);
}

template <>
DataRef<float>::operator float() const
{
    return XPLMGetDataf(m_dataRef);
}

template <>
DataRef<std::vector<int>>::operator std::vector<int>() const
{
    std::vector<int> result(XPLMGetDatavi(m_dataRef, nullptr, 0, 0));
    XPLMGetDatavi(m_dataRef, &result[0], 0, result.size());
    return result;
}

template <>
DataRef<std::vector<float>>::operator std::vector<float>() const
{
    std::vector<float> result(XPLMGetDatavf(m_dataRef, nullptr, 0, 0));
    XPLMGetDatavf(m_dataRef, &result[0], 0, result.size());
    return result;
}

template <>
DataRef<std::string>::operator std::string() const
{
    std::string result;
    result.resize(XPLMGetDatab(m_dataRef, nullptr, 0, 0));
    XPLMGetDatab(m_dataRef, &result[0], 0, result.size());
    return result;
}

template <>
const DataRef<int>& DataRef<int>::operator=(const int& value)
{
    if (m_readWriteType != ReadWriteType::ReadOnly) {
        XPLMSetDatai(m_dataRef, value);
    }
    return *this;
}

template <>
const DataRef<float>& DataRef<float>::operator=(const float& value)
{
    if (m_readWriteType != ReadWriteType::ReadOnly) {
        XPLMSetDataf(m_dataRef, value);
    }
    return *this;
}

template <>
const DataRef<std::vector<int>>& DataRef<std::vector<int>>::operator=(const std::vector<int>& value)
{
    if (m_readWriteType != ReadWriteType::ReadOnly) {
        XPLMSetDatavi(m_dataRef, const_cast<int*>(&value[0]), 0, value.size());
    }
    return *this;
}

template <>
const DataRef<std::vector<float>>& DataRef<std::vector<float>>::operator=(const std::vector<float>& value)
{
    if (m_readWriteType != ReadWriteType::ReadOnly) {
        XPLMSetDatavf(m_dataRef, const_cast<float*>(&value[0]), 0, value.size());
    }
    return *this;
}

template <>
const DataRef<std::string>& DataRef<std::string>::operator=(const std::string& value)
{
    if (m_readWriteType != ReadWriteType::ReadOnly) {
        XPLMSetDatab(m_dataRef, const_cast<char*>(value.c_str()) , 0, value.size() + 1);
    }
    return *this;
}

template<>
dataref_trait<std::vector<int>>::BasicType DataRef<std::vector<int>>::operator[](int index) const
{
    const std::vector<int>& vi(*this);
    return vi[index];
}

template<>
dataref_trait<std::vector<float>>::BasicType DataRef<std::vector<float>>::operator[](int index) const
{
    const std::vector<float>& vf(*this);
    return vf[index];
}

template<>
dataref_trait<std::string>::BasicType DataRef<std::string>::operator[](int index) const
{
    std::string result;
    result.resize(XPLMGetDatab(m_dataRef, nullptr, 0, 0));
    XPLMGetDatab(m_dataRef, &result[0], 0, result.size());
    return result[index];
}

template<>
void DataRef<std::vector<int>>::setValue(int index, int value)
{
    int* values = new int[1] {value};
    XPLMSetDatavi(m_dataRef, values, index, 1);
}

template<>
void DataRef<std::vector<float>>::setValue(int index, float value)
{
    float* values = new float[1] {value};
    XPLMSetDatavf(m_dataRef, values, index, 1);
}

template<>
void DataRef<std::string>::setValue(int index, char value)
{
    char* values = new char[1] {value};
    XPLMSetDatab(m_dataRef, values, index, 1);
}
