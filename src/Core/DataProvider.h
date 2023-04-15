#pragma once

#include <string>
#include <vector>
#include <cstdint>
#include <unordered_map>
#include <ctools/Logger.h>

class DataProvider
{
private:
	// key = data name, offset in buffer for have op on datas
	std::unordered_map<std::string, uint32_t> m_Offsets;
	// datas buffer
	std::vector<uint8_t> m_Datas;

public:
	// add size to uniform block, return startOffset
	bool RegisterByteSize(const std::string& vKey, uint32_t vSizeInBytes, uint32_t* vStartOffset = 0);

	// add a variable
	template<typename T> void RegisterVar(const std::string& vKey, T vValue); // add var to uniform block
	template<typename T> void RegisterVar(const std::string& vKey, T* vValue, uint32_t vSizeInBytes); // add var to uniform block

	// Get / set + op on variables
	template<typename T> bool GetVar(const std::string& vKey, T& vValue); // Get
	template<typename T> bool SetVar(const std::string& vKey, T vValue); // set
	template<typename T> bool SetVar(const std::string& vKey, T* vValue, uint32_t vSizeInBytes); // set
	template<typename T> bool SetAddVar(const std::string&, T vValue); // add and set like +=

private:
	bool OffsetExist(const std::string& vKey);
	uint32_t GetGoodAlignement(uint32_t vSize);
	void AddOffsetForKey(const std::string& vKey, uint32_t vOffset);
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////
//// PUBLIC TEMPLATES //////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////

template<typename T>
void DataProvider::RegisterVar(const std::string& vKey, T* vValue, uint32_t vSizeInBytes)
{
	uint32_t startOffset;
	if (RegisterByteSize(vKey, vSizeInBytes, &startOffset))
	{
		// on copy de "startOffset" à "startOffset + vSizeInBytes"
		memcpy(datas.data() + startOffset, vValue, vSizeInBytes);
	}
}

template<typename T>
void DataProvider::RegisterVar(const std::string& vKey, T vValue)
{
	RegisterVar(vKey, &vValue, sizeof(vValue));
}

template<typename T>
bool DataProvider::GetVar(const std::string& vKey, T& vValue)
{
	if (OffsetExist(vKey))
	{
		uint32_t offset = offsets[vKey];
		uint32_t size = sizeof(vValue);
		memcpy(&vValue, datas.data() + offset, size);
		return true;
	}
	LogVarDebug("key %s not exist in DataProvider. GetVar fail.", vKey.c_str());
	return false;
}

template<typename T>
bool DataProvider::SetVar(const std::string& vKey, T* vValue, uint32_t vSizeInBytes)
{
	if (OffsetExist(vKey) && vSizeInBytes > 0)
	{
		uint32_t newSize = vSizeInBytes;
		uint32_t offset = offsets[vKey];
		memcpy(datas.data() + offset, vValue, newSize);
		isDirty = true;
		return true;
	}
	LogVarDebug("key %s not exist in DataProvider. SetVar fail.", vKey.c_str());
	return false;
}

template<typename T>
bool DataProvider::SetVar(const std::string& vKey, T vValue)
{
	return SetVar(vKey, &vValue, sizeof(vValue));
}

template<typename T>
bool DataProvider::SetAddVar(const std::string& vKey, T vValue)
{
	T v;
	if (GetVar(vKey, v))
	{
		v += vValue;
		return SetVar(vKey, v);
	}
	LogVarDebug("key %s not exist in DataProvider. SetAddVar fail.", vKey.c_str());
	return false;
}
