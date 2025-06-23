#pragma once
#include <KamataEngine.h>
class Skydome 
{
public:
	//<summary>
	//初期化
	void Initialize(KamataEngine::Model* model, KamataEngine::Camera* camera);
	//更新
	void Update();
	//描画
	void Draw();

private:
	//ワールド変換データ
	KamataEngine::WorldTransform worldTransform_;
	KamataEngine::Model* model_ = nullptr;
	KamataEngine::Camera* camera_;
};