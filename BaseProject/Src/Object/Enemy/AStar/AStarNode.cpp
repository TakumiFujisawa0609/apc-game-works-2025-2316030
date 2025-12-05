#include <algorithm>
#include <cmath>
#include <limits>
#include "AStarNode.h"

AStarNode::AStarNode(void)
    :
#ifdef max
#undef max
#endif
    gridX_(0),
    gridZ_(0),
    isWalkable_(false),
    worldPos_({}),
    G_Score_(std::numeric_limits<float>::max()),
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

void AStarNode::Reset(void)
{
    G_Score_ = std::numeric_limits<float>::max();
    H_Score_ = 0.0f;
    parent_ = nullptr;
}
