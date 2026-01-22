#pragma once
#include "../../ObjectBase/ActorBase.h"
class PlazzleElementBase :
    public ActorBase
{
public:

    PlazzleElementBase(void);
    virtual ~PlazzleElementBase(void);

    virtual void Init(void) = 0;
    void Update(float deltaTime) override;
    virtual void OnUpdate(float deltaTime) = 0;
    virtual void Draw(void) = 0;


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

    void InitModel(float distance, VECTOR size, Quaternion quaRot, VECTOR quaLocal);

    // 画像側の初期化
    void InitImg(float posX, float posY, float scale, float angle);

    int imgH_;              // 画像ハンドル
    float posX_, posY_;     //　スクリーン座標
    float scale_;            // 描画拡大率
    float angle_;           // 角度

private:

};

