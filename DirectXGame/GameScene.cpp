#include "GameScene.h"
#include "MyMath.h"

using namespace KamataEngine;
// デストラクタ
GameScene::~GameScene() 
{
	for (std::vector<WorldTransform*>& worldTransformBlockLine : worldTransformBlocks_) 
	{
		for (WorldTransform* worldTransformBlock:worldTransformBlockLine)
		{
			delete worldTransformBlock;
		}
	}
	worldTransformBlocks_.clear();
	//3Dモデルデータの解放

	delete player_;
	delete model_;
	delete debugCamera_;
}
void GameScene::Initialize()
{
	debugCamera_ = new DebugCamera(1280, 720);

	//要素数
	const uint32_t kNumBlockVirtical = 10;
	const uint32_t kNumBlockHorizontal = 20;
	//ブロック1個分の横幅
	const float kBlockWidth = 2.0f;
	const float kBloakHeight = 2.0f;
	//要素数を変更する
	worldTransformBlocks_.resize(kNumBlockVirtical);
	for (uint32_t i = 0; i < kNumBlockVirtical; ++i) 
	{
		worldTransformBlocks_[i].resize(kNumBlockHorizontal);
	}
	//キューブの生成
	for (uint32_t i=0;i<kNumBlockVirtical;++i) 
	{
		for (uint32_t j = 0; j < kNumBlockHorizontal;++j)
		{
			if ((i+j) % 2 == 0)
				continue;
			worldTransformBlocks_[i][j] = new WorldTransform();
			worldTransformBlocks_[i][j]->Initialize();
			worldTransformBlocks_[i][j]->translation_.x = kBlockWidth * j;
			worldTransformBlocks_[i][j]->translation_.y = kBloakHeight * i;
		}
	}
	//3Dモデルデータの生成
	model_Block = Model::CreateFromOBJ("cube");
	// カメラ
	camera_.Initialize();
	worldTransform_.Initialize();
	camera_.Initialize();
	model_ = Model::Create();
	textureHandle_ = TextureManager::Load("uvChecker.png");
	player_ = new Player();
	player_->Initialize(model_, textureHandle_, &camera_);
}
void GameScene::Update() 
{
	//ブロックの更新
	for (std::vector<WorldTransform*>& worldTransformBlockLine : worldTransformBlocks_) 
	{

		for (WorldTransform* worldTransformBlock : worldTransformBlockLine) 
		{
			if (!worldTransformBlock)
				continue;
			worldTransformBlock->matWorld_ = MakeAffineMatrix(worldTransformBlock->scale_, worldTransformBlock->rotation_, worldTransformBlock->translation_);
			// 定数バッファに転送する
			worldTransformBlock->TransferMatrix();
			debugCamera_->Update();

		}
	}
	player_->Update();
#ifdef _DEBUG
	if (Input::GetInstance()->TriggerKey(DIK_0)) 
	{
		isDebugCameraActive_ = !isDebugCameraActive_;
	}
	if (isDebugCameraActive_) 
	{
		debugCamera_->Update();
		camera_.matView = debugCamera_->GetCamera().matView;
		camera_.matProjection = camera_.matProjection = debugCamera_->GetCamera().matProjection;
		//ビュープロジェクション行列の更新と転送
		camera_.TransferMatrix();
	} else 
	{
		camera_.UpdateMatrix();
	}
#endif
}
void GameScene::Draw()
{
	DirectXCommon* dxCommon = DirectXCommon::GetInstance();
	Model::PreDraw(dxCommon->GetCommandList());
	// ブロックの描画
	for (std::vector<WorldTransform*>& worldTransformBlockLine : worldTransformBlocks_) 
	{
		for (WorldTransform* worldTransformBlock : worldTransformBlockLine) 
		{
			if (!worldTransformBlock)
				continue;
			model_Block->Draw(*worldTransformBlock, camera_);
		}
	}
	player_->Draw();
	model_->Draw(worldTransform_, camera_, textureHandle_);
	Model::PostDraw();
}