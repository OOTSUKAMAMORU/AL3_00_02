#include "TitleScene.h"
#include "MyMath.h"
#include <numbers>
#include "TitleScene.h"
using namespace KamataEngine;
//デストラクタ
TitleScene::~TitleScene() 
{
	//モデル
	delete model_;
	delete modelPlayer_;
	delete fade_;
}
//初期化
void TitleScene::Initialize() 
{
	//3Dモデルの生成
	model_ = Model::CreateFromOBJ("titleFont");
	modelPlayer_ = Model::CreateFromOBJ("player");
	//カメラの初期化
	camera_.Initialize();
	//ワールド変換の初期化
	worldTransform_.Initialize();
	worldTransform_.scale_ = {2,2,2};
	worldTransform_.translation_ = {0,8,0};

	worldTransformPlayer_.Initialize();
	worldTransformPlayer_.scale_ = {10,10,10};
	worldTransformPlayer_.translation_= {0,-8,0};
	worldTransformPlayer_.rotation_.y=std::numbers::pi_v<float>;
	//フェード
	fade_ = new Fade();
	fade_->Initialize();
	fade_->Start(Fade::Status::FadeIn, 1.0f);
}
//更新
void TitleScene::Update()
{
	switch (phase_) 
	{
	case Phase::kMain:
		// タイトルシーンの終了条件
		if (Input::GetInstance()->PushKey(DIK_SPACE)) 
		{
			finished_ = false;
			// フェードアウト開始
			phase_ = Phase::kFadeOut;
			fade_->Start(Fade::Status::FadeOut, 1.0f);
		}
		break;
	case Phase::kFadeIn:
		// フェード
		fade_->Update();
		if (fade_->IsFinished()) 
		{
			phase_ = Phase::kMain;
		}
		break;
	case Phase::kFadeOut:
		// フェード
		fade_->Update();
		if (fade_->IsFinished()) 
		{
			finished_ = true;
		}
	}
	worldTransform_.matWorld_ = MakeAffineMatrix(worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);
	//行列を定数バッファに転送
	worldTransform_.TransferMatrix();
	//回転
	rotate_ += 0.1f;
	worldTransformPlayer_.rotation_.y = std::sin(rotate_) + std::numbers::pi_v<float>;
	worldTransformPlayer_.matWorld_ = MakeAffineMatrix(worldTransformPlayer_.scale_, worldTransformPlayer_.rotation_, worldTransformPlayer_.translation_);
	//行列を定数バッファに転送
	worldTransformPlayer_.TransferMatrix();
}
//描画
void TitleScene::Draw()
{
	DirectXCommon* dxCommon = DirectXCommon::GetInstance();
	Model::PreDraw(dxCommon->GetCommandList());
	model_->Draw(worldTransform_,camera_);
	modelPlayer_->Draw(worldTransformPlayer_, camera_);
	Model::PostDraw();
	//フェード
	fade_->Draw();
}

