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
