#include "Animation.h"

void Animation::update()
{
    Frame += speed;
    int n = frames.size();
    if (Frame >= n) Frame -= n;
    if (n > 0) sprite.setTextureRect(frames[int(Frame)]);
}

bool Animation::isEnd()
{
    return Frame + speed >= frames.size();
}