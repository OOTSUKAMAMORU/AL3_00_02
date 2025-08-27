#pragma once
#include "KamataEngine.h"
//フェード
class Fade
{
public:
	//フェードの状態
	enum class Status 
	{
		None,
		FadeIn,
		FadeOut,
	};
	//初期化
	void Initialize();
	//更新
	void Update();
	//描画
	void Draw();
	//フェード開始
	void Start(Status status, float duration);
	//フェード終了
	void Stop();
	//フェード終了判定
	bool IsFinished() const;

private:
	uint32_t textureHandle_ = 0;
	KamataEngine::Sprite* sprite_ = nullptr;
	Status status_ = Status::None;
	float duration_ = 0.0f;
	float counter_ = 0.0f;
};
