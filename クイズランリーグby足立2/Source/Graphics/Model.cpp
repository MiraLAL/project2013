#include "Graphics/Graphics.h"
#include "Graphics/Model.h"

// コンストラクタ
Model::Model(const char* filename)
{
	// リソース読み込み
	resource = std::make_shared<ModelResource>();
	resource->Load(Graphics::Instance().GetDevice(), filename);

	// ノード
	const std::vector<ModelResource::Node>& resNodes = resource->GetNodes();

	nodes.resize(resNodes.size());
	for (size_t nodeIndex = 0; nodeIndex < nodes.size(); ++nodeIndex)
	{
		auto&& src = resNodes.at(nodeIndex);
		auto&& dst = nodes.at(nodeIndex);

		dst.name = src.name.c_str();
		dst.parent = src.parentIndex >= 0 ? &nodes.at(src.parentIndex) : nullptr;
		dst.scale = src.scale;
		dst.rotate = src.rotate;
		dst.translate = src.translate;

		if (dst.parent != nullptr)
		{
			dst.parent->children.emplace_back(&dst);
		}
	}

	// 行列計算
	const DirectX::XMFLOAT4X4 transform = { 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1000, 0, 0, -1, 0, 2 };
	UpdateTransform(transform);
}

// 変換行列計算
void Model::UpdateTransform(const DirectX::XMFLOAT4X4& transform)
{
	DirectX::XMMATRIX Transform = DirectX::XMLoadFloat4x4(&transform);

	for (Node& node : nodes)
	{
		// ローカル行列算出
		DirectX::XMMATRIX S = DirectX::XMMatrixScaling(node.scale.x, node.scale.y, node.scale.z);
		DirectX::XMMATRIX R = DirectX::XMMatrixRotationQuaternion(DirectX::XMLoadFloat4(&node.rotate));
		DirectX::XMMATRIX T = DirectX::XMMatrixTranslation(node.translate.x, node.translate.y, node.translate.z);
		DirectX::XMMATRIX LocalTransform = S * R * T;

		// ワールド行列算出
		DirectX::XMMATRIX ParentTransform;
		if (node.parent != nullptr)
		{
			ParentTransform = DirectX::XMLoadFloat4x4(&node.parent->worldTransform);
		}
		else
		{
			ParentTransform = Transform;
		}
		DirectX::XMMATRIX WorldTransform = LocalTransform * ParentTransform;

		// 計算結果を格納
		DirectX::XMStoreFloat4x4(&node.localTransform, LocalTransform);
		DirectX::XMStoreFloat4x4(&node.worldTransform, WorldTransform);
	}
}

//アニメーション更新処理
void Model::UpdateAnimetion(float elapsedTime)
{
	//再生中でないなら処理しない
	if (!IsPlayAnimetion()) return;

	//指定のアニメーションデータを取得
	const std::vector<ModelResource::Animation>& animetions = resource->GetAnimations();
	const ModelResource::Animation& animetion = animetions.at(currentAnimetionIndex);

	//アニメーションデータからキーフレームデータリストを取得
	const std::vector<ModelResource::Keyframe>& keyframes = animetion.keyframes;
	int keyCount = static_cast<int>(keyframes.size());
	for (int keyIndex = 0; keyIndex < keyCount - 1; ++keyIndex)
	{
		//現在の時間がどのキーフレームの間にいるか判定する
		const ModelResource::Keyframe& keyframe0 = keyframes.at(keyIndex);
		const ModelResource::Keyframe& keyframe1 = keyframes.at(keyIndex + 1);
		if (currentAnimetionSeconds >= keyframe0.seconds &&
			currentAnimetionSeconds < keyframe1.seconds)
		{
			//再生時間とキーフレームの時間から補完率を算出する
			float rate = (currentAnimetionSeconds - keyframe0.seconds) / (keyframe1.seconds - keyframe0.seconds);

			int nodeCount = static_cast<int>(nodes.size());
			for (int nodeiIndex = 0; nodeiIndex < nodeCount; ++nodeiIndex)
			{
				//2つのキーフレーム間の補完計算
				const ModelResource::NodeKeyData& key0 = keyframe0.nodeKeys.at(nodeiIndex);
				const ModelResource::NodeKeyData& key1 = keyframe1.nodeKeys.at(nodeiIndex);

				Node& node = nodes[nodeiIndex];

				DirectX::XMVECTOR S0 = DirectX::XMLoadFloat3(&key0.scale);
				DirectX::XMVECTOR S1 = DirectX::XMLoadFloat3(&key1.scale);
				DirectX::XMVECTOR R0 = DirectX::XMLoadFloat4(&key0.rotate);
				DirectX::XMVECTOR R1 = DirectX::XMLoadFloat4(&key1.rotate);
				DirectX::XMVECTOR T0 = DirectX::XMLoadFloat3(&key0.translate);
				DirectX::XMVECTOR T1 = DirectX::XMLoadFloat3(&key1.translate);

				DirectX::XMVECTOR S = DirectX::XMVectorLerp(S0, S1, rate);
				DirectX::XMVECTOR R = DirectX::XMQuaternionSlerp(R0, R1, rate);
				DirectX::XMVECTOR T = DirectX::XMVectorLerp(T0, R1, rate);

				//計算結果をボーンに格納
				DirectX::XMStoreFloat3(&node.scale, S);
				DirectX::XMStoreFloat4(&node.rotate, R);
				DirectX::XMStoreFloat3(&node.translate, T);
			}
			break;

		}
	}

	//時間経過
	currentAnimetionSeconds += elapsedTime;

	//再生時間が週短時間を超えたら
	if (currentAnimetionSeconds >= animetion.secondsLength)
	{
		//再生時間を巻き戻す
		currentAnimetionSeconds -= animetion.secondsLength;
	}
}

//アニメーション再生
void Model::PlayAnimetion(int index) 
{
	currentAnimetionIndex = index;
	currentAnimetionSeconds = 0.0f;
}

//アニメーション再生中か
bool Model::IsPlayAnimetion()const
{
	if (currentAnimetionIndex < 0) return false;
	if (currentAnimetionIndex >= resource->GetAnimations().size())return false;
	return true;
}
