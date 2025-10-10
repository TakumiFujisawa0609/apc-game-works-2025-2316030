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

VECTOR PatrolNode::GetPos(void)
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
