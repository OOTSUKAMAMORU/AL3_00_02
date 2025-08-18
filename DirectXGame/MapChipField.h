#pragma once
#include "KamataEngine.h"
#include <map>
enum class MapChipType {
	kBlank, // 空白
	kBlock, // ブロック
};
struct MapChipData {
	std::vector<std::vector<MapChipType>> data;
};
/// <summary>
/// マップチップフィールド
/// </summary>
class MapChipField 
{
public:
	uint32_t GetNumBlockVirtical() const { return kNumBlockVirtical; }
	uint32_t GetNumBlockHorizontal() const { return kNumBlockHorizontal; }
	void LoadMapChipCsv(const std::string& filePath);
	void ResetMapChipData();
	MapChipData mapChipData_;
	// 1ブロックのサイズ
	static inline const float kBlockWidth = 1.0f;
	static inline const float kBlockHeight = 1.0f;
	// ブロックの個数
	static inline const uint32_t kNumBlockVirtical = 20;
	static inline const uint32_t kNumBlockHorizontal = 100;
	MapChipType GetMapChipTypeByIndex(uint32_t xIndex, uint32_t yIndex);
	KamataEngine::Vector3 GetMapChipPositionByIndex(uint32_t xIndex, uint32_t yIndex);
	struct IndexSet 
	{
		uint32_t xIndex;
		uint32_t yIndex;
	};
	IndexSet GetMapChipIndexSetByPosition(const Vector3& position);
	struct  Rect 
	{
		float left;
		float right;
		float bottom;
		float top;
	};
	Rect GetRectByIndex(uint32_t xIndex, uint32_t yIndex);
};
