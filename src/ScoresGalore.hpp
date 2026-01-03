#ifndef _SCORES_GALORE_HPP_
#define _SCORES_GALORE_HPP_

#include "classes/classes.h"
#include <Geode/binding/PlayLayer.hpp>
#include <Geode/binding/PlayerObject.hpp>
#include "ui/ui.h"

const int CLICKS_UNTIL_FEVER = 25;
const float FEVER_TIME = 8.0f;

class ScoresGalore {
protected:
    // you "singletons" don't understand, do you?
    static ScoresGalore* _instancePtr;
    PlayerObject* _currentPlayer;
    PlayUI* _currentPlayUI;
    PlayLayer* _currentPlayLayer;    

    ScoresGalore() {}
public:
    ScoresGalore(const ScoresGalore& obj) = delete;
    static ScoresGalore* get();

    bool playLayerWasSetup = false;
    Scoring* currentScoring;

    // fever stuff
    bool feverTimerStarting;
    float timeUntilFever;
    int combosInsideFeverStart;

    bool isFeverActive;
    float feverTime;

    void upCombo();
    void resetCombo();
    void startFeverCheck();

    void resetFever();

    void resetScoringForANewRun();
    void setupForPlayLayer(PlayLayer* playLayer);
    PlayerObject* getCurrentPlayer();
    void cleanupForPlayLayer();
    void handleButtonPush(PlayerButton button);
    void onPlayLayerPostUpdate(float dt);
};

#endif