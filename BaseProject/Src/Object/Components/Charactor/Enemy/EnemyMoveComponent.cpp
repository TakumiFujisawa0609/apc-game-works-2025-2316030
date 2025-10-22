#include "../../../../Utility/AsoUtility.h"
#include "../Object/ObjectBase/ActorBase.h"
#include "../../../Common/Transform.h"
#include "../../../Player/Player.h"
#include "../../../Enemy/EnemyBase.h"
#include "../../../Enemy/Patrol/PatrolPath.h"
#include "../../../Enemy/Patrol/PatrolNode.h"
#include "EnemyMoveComponent.h"

EnemyMoveComponent::EnemyMoveComponent(std::shared_ptr<Charactor> owner, Player& player)
	:
	EnemyComponent(owner, player),
	isWaiting_(false),
	currentWaitTime_(0.0f),
    outRotation_({}),
    dis_(0.0f),
    currentNode_(0)
{
}

EnemyMoveComponent::~EnemyMoveComponent(void)
{
}

void EnemyMoveComponent::Update(float deltaTime)
{

	// 敵ベースを取得
	std::shared_ptr<ActorBase> owner = this->GetOwner().lock();

	if (!owner)
	{
		return;
	}

	Transform transform = owner->GetTransform();

	// 回転(モデルが移動している方向を向かせる)


	// 当たり判定


	// 重力計算


	// 移動処理


}

void EnemyMoveComponent::Patrol(float deltaTime, Transform& transform, std::shared_ptr<PatrolPath> path, int& currentIndex, VECTOR& moveDir, VECTOR& movePow, float moveSpeed, Quaternion& outRotation)
{
    std::shared_ptr<Charactor> owner = this->GetCharactor().lock();
    if (!owner || !path)return;

    EnemyBase* enemyBase = static_cast<EnemyBase*>(owner.get());
    if (!enemyBase) return;

    // outMoveDirを初期化 (移動しない場合のため)
    moveDir = AsoUtility::VECTOR_ZERO;

    // 現在の目標ノードを取得
    const PatrolNode& targetNode = path->GetNodeIndex(currentIndex);
    VECTOR targetPos = targetNode.GetPos();
    currentNode_ = currentNode_;

    // ----------------------------------------------------
    // 待機処理
    // ----------------------------------------------------
    if (isWaiting_)
    {
        currentWaitTime_ -= deltaTime;

        if (currentWaitTime_ <= 0.0f)
        {
            // 待機終了。次のノードへ進む
            isWaiting_ = false;

            // 状態をPATROL（移動）に戻す
            //enemyBase->ChangeState(EnemyBase::STATE::PATROL);
        }
        else
        {
            // 次のノードのインデックスを更新
            path->GetNextNode(currentIndex);
            currentNode_ = currentIndex;

            // 待機中は移動を停止
            moveDir = AsoUtility::VECTOR_ZERO;

            // 回転はそのまま維持
            outRotation = transform.quaRot;
            return;
        }
    }

    // ----------------------------------------------------
    // 移動/回転処理
    // ----------------------------------------------------

    // 現在位置（Y軸は無視して水平移動）
    VECTOR currentPos = transform.pos;
    currentPos.y = 0.0f;
    targetPos.y = 0.0f;

    VECTOR moveVector = VSub(targetPos, currentPos);
    float distance = VSize(moveVector);
    dis_ = distance;

    // 目標位置に到達したかをチェック（許容誤差1.0f）
    if (distance < 5.5f)
    {
        // 目標に到達したら待機状態に遷移
        isWaiting_ = true;
        currentWaitTime_ = targetNode.GetWaitTime();

        // 移動を停止
        moveDir = AsoUtility::VECTOR_ZERO;

        //// ノード固有のアクションに応じて状態を変更
        //if (targetNode.GetActType() == PatrolNode::ACTTYPE::LOCKAROUND)
        //{
        //    // 周囲を見回すアニメーションなどに変更
        //    //enemyBase->ChangeState(EnemyBase::STATE::IDLE);
        //}
        //else
        //{
        //    // 通常の待機アニメーション
        //    //enemyBase->ChangeState(EnemyBase::STATE::IDLE);
        //}
    }
    else
    {
        // 移動方向を設定 (出力)
        VECTOR moveDirection = VNorm(moveVector);
        moveDir = moveDirection;
        //moveSpeed = 12.0f;

        movePow = VScale(moveDir, 3.0f);

        float length = AsoUtility::MagnitudeF(moveDir);
        if (length < 0.001f)return;

        // モデルの回転角度を設定 (出力)
        // Z軸（前方向）から移動方向への回転
        float rotationY = atan2f(moveDirection.x, moveDirection.z);
        Quaternion targetRotation = Quaternion::Euler({ 0.0f, rotationY, 0.0f });
   
        // 現在の回転と目標の回転の間を滑らかに補間
        outRotation = Quaternion::Slerp(transform.quaRot, targetRotation, 5.0f * deltaTime);
    }
}

float EnemyMoveComponent::GetDis(void)
{
    return dis_;
}

int EnemyMoveComponent::GetCurrentNode(void)
{
    return currentNode_;
}

void EnemyMoveComponent::DrawDebug(std::shared_ptr<PatrolPath> path, int currentIndex)
{


}
