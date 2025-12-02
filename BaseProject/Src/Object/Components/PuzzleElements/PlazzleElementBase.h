#pragma once
#include "../../ObjectBase/ActorBase.h"
class PlazzleElementBase :
    public ActorBase
{
public:

    PlazzleElementBase(void);
    virtual ~PlazzleElementBase(void);

    void Init(void) override;
    void Update(float deltaTime) override;
    void OnUpdate(float deltaTime) override;
    void Draw(void) override;


    /// <summary>
    /// ワールド座標系の3Dモデルをスクリーン座標系で描画する
    /// </summary>
    /// <param name="ModelHandle">モデルハンドル</param>
    /// <param name="screenPosX">スクリーンx座標</param>
    /// <param name="screenPosY">スクリーンy座標</param>
    /// <param name="screenAngleRad">スクリーン空間での回転角度</param>
    /// <param name="scale">スクリーン空間での拡大率</param>
    void DrawModelInScreenSpace(
        int ModelHandle,
        float screenPosX,
        float screenPosY,
        float screenAngleRad,
        float scale);

protected:

    // モデルの初期化
    void InitModel(float distance, VECTOR size, Quaternion quaRot, VECTOR quaLocal);

    // 画像側の初期化
    void InitImg(float posX, float posY, float scale, float angle);

    // 画像ハンドル
    int imgH_;

    //　スクリーン座標
    float posX_, posY_;

    // 描画拡大率
    float scale_;

    // 角度
    float angle_;

private:

};

