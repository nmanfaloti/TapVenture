#ifndef CHALLENGE_H
#define CHALLENGE_H

int launchChallenge();
void resetChallenge();
void updateChallenge();
void displayChallengeTimer(TTF_Font* font);

extern int challengeActive;
extern int challengeTarget;
extern unsigned int challengeStartTime;
extern unsigned int challengeDuration;

#endif // CHALLENGE_H