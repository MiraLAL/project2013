#pragma once

#include <memory>
#include <vector>
#include <DirectXMath.h>
#include "Graphics/ModelResource.h"

// モデル
class Model
{
public:
	Model(const char* filename);
	~Model() {}

	struct Node
	{
		const char*			name;
		Node*				parent;
		DirectX::XMFLOAT3	scale;
		DirectX::XMFLOAT4	rotate;
		DirectX::XMFLOAT3	translate;
		DirectX::XMFLOAT4X4	localTransform;
		DirectX::XMFLOAT4X4	worldTransform;

		std::vector<Node*>	children;
	};

	// 行列計算
	void UpdateTransform(const DirectX::XMFLOAT4X4& transform);

	// ノードリスト取得
	const std::vector<Node>& GetNodes() const { return nodes; }
	std::vector<Node>& GetNodes() { return nodes; }

	// リソース取得
	const ModelResource* GetResource() const { return resource.get(); }

	//アニメーション更新処理
	void UpdateAnimetion(float elapsedTime);

	//アニメーション再生
	//void PlayAnimetion(int index);

	//アニメーション再生中か
	//bool IsPlayAnimetion() const;

private:
	std::shared_ptr<ModelResource>	resource;
	std::vector<Node>				nodes;

	int currentAnimetionIndex = -1;
	float currentAnimetionSeconds = 0.0f;
};
