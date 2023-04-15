#include "DataProvider.h"

bool DataProvider::RegisterByteSize(const std::string& vKey, uint32_t vSizeInBytes, uint32_t* vStartOffset)
{
	if (OffsetExist(vKey))
	{
		LogVarDebug("key %s is already defined in UniformBlockStd140. RegisterVar fail.", vKey.c_str());
	}
	else if (vSizeInBytes > 0)
	{
		uint32_t newSize = vSizeInBytes;
		uint32_t lastOffset = (uint32_t)m_Datas.size();
		auto baseAlign = GetGoodAlignement(newSize);
		// il faut trouver le prochain offset qui est multiple de baseAlign
		auto startOffset = baseAlign * (uint32_t)std::ceil((double)lastOffset / (double)baseAlign);
		auto newSizeToAllocate = startOffset - lastOffset + newSize;
		m_Datas.resize(lastOffset + newSizeToAllocate);
		// on set de "lastOffset" à "lastOffset + newSizeToAllocate"
		memset(m_Datas.data() + lastOffset, 0, newSizeToAllocate);
		AddOffsetForKey(vKey, startOffset);

		if (vStartOffset)
			*vStartOffset = startOffset;

		return true;
	}

	return false;
}

bool DataProvider::OffsetExist(const std::string& vKey)
{
	return m_Offsets.find(vKey) != m_Offsets.end();
}

uint32_t DataProvider::GetGoodAlignement(uint32_t vSize)
{
	// Std140 Alignement
	uint32_t goodAlign = (uint32_t)std::pow(2, std::ceil(log(vSize) / log(2)));
	return std::min(goodAlign, 16u);
}

// non templates (define in class implementation)
void DataProvider::AddOffsetForKey(const std::string& vKey, uint32_t vOffset)
{
	m_Offsets[vKey] = vOffset;
}
