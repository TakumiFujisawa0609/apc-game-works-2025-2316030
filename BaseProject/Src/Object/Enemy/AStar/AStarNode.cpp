#include "AStarNode.h"

AStarNode::AStarNode(void)
    :
    G_Score_(),
    H_Score_(0),
    parent_(nullptr)
{
}

AStarNode::~AStarNode(void)
{
}

float AStarNode::F_Score(void) const
{
    return G_Score_ + H_Score_;
}
