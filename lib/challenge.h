#ifndef CHALLENGE_H
#define CHALLENGE_H

#include <time.h>

typedef struct {
    int active;
    unsigned int startTime;
    int duration;
    int target;
    int reward;
    int cooldown;
    time_t lastTime;
} Challenge_t;

extern Challenge_t challenge;

int launchChallenge();
void resetChallenge();
void updateChallenge();
void displayChallengeTimer(int elapsedTimeChallenge);

#endif // CHALLENGE_H