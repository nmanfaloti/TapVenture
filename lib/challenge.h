#ifndef CHALLENGE_H
#define CHALLENGE_H

int launchChallenge();
void resetChallenge();
void updateChallenge();
void displayChallengeTimer();

extern int challengeActive;
extern int challengeTarget;
extern unsigned int challengeStartTime;
extern unsigned int challengeDuration;
extern time_t lastChallengeTime;

#endif // CHALLENGE_H