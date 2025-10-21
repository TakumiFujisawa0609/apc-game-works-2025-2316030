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

float PatrolNode::GetWaitTime(void) const
{
    return waitTime_;
}

PatrolNode::ACTTYPE PatrolNode::GetActType(void) const
{
    return actionType_;
}

void PatrolNode::DebugDraw(void) const
{
    DrawSphere3D(pos_, 32.0f, 16, GetColor(255, 0, 0), GetColor(255, 0, 0), false);
}
