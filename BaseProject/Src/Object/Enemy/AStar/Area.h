#pragma once
#include <vector>
#include <DxLib.h>

// エリア間の接続点(ポータル)を表す構造体
struct AreaConnectionPoint {

	AreaConnectionPoint(VECTOR pos, int id);
	~AreaConnectionPoint(void);

	VECTOR worldPos_;		// 接続点のワールド座標
	int connectedAreaId_;	// 接続先のエリアID
};


class Area
{
public:
	Area(int id, VECTOR center, float sizeX, float sizeZ);
	~Area(void);

	// 指定座標がエリア内にあるか
	bool Contains(const VECTOR& pos) const;

	int id_;
	
	// エリアの境界情報
	VECTOR center_;
	float sizeX_, sizeZ_;

	// このエリアから外部へ出るための接続点(ポータル)
	std::vector<AreaConnectionPoint> connections_;
};

