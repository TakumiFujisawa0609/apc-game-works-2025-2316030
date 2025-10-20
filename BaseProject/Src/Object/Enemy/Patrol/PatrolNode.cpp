#include "PatrolNode.h"

PatrolNode::PatrolNode(VECTOR pos, float time, ACTTYPE type)
    :
    pos_(pos),
    waitTime_(time),
    actionType_(type)
{
}

PatrolNode::~PatrolNode(void)
{
}

VECTOR PatrolNode::GetPos(void) const
{
    return pos_;
}

float PatrolNode::GetWaitTime(void)
{
    return waitTime_;
}

PatrolNode::ACTTYPE PatrolNode::GetActType(void)
{
    return actionType_;
}

void PatrolNode::DebugDraw(void)
{
    DrawSphere3D(pos_, 16.0f, 8, GetColor(255, 0, 0), GetColor(255, 0, 0), false);
}
