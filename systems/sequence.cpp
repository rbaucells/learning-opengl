#include "sequence.h"
#include "../systems/workQueue.h"

void Sequence::start() {

}
void Sequence::add(const TweenBase& tween) {
    curIndex++;
    actions[curIndex].push_back(tween);
}
void Sequence::join(const TweenBase& tween) {
    actions[curIndex].push_back(tween);
}
void Sequence::stop() {

}
