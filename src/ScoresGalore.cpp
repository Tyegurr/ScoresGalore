#include "ScoresGalore.hpp"

// protected
ScoresGalore* ScoresGalore::_instancePtr = nullptr;

// public
ScoresGalore* ScoresGalore::get()
{
    if (_instancePtr == nullptr) {
        _instancePtr = new ScoresGalore();
    }
    return _instancePtr;
}

void ScoresGalore::upCombo()
{
    currentScoring->combo++;
    _currentPlayUI->scheduleUpdateComboLabel(currentScoring->combo, true);
    if (feverTimerStarting)
        combosInsideFeverStart++;
    startFeverCheck();
}
void ScoresGalore::resetCombo() {
    if (playLayerWasSetup == false) return;
    currentScoring = new Scoring();
    resetFever();
    _currentPlayUI->stopFeverEffect(true);
    _currentPlayUI->scheduleUpdateComboLabel(currentScoring->combo, false);
}
void ScoresGalore::startFeverCheck() {
    if (isFeverActive == true) return;
    feverTimerStarting = true;
}
void ScoresGalore::resetFever() {
    feverTimerStarting = false;
    isFeverActive = false;
    timeUntilFever = 0.0f;
    combosInsideFeverStart = 0;
    feverTime = 0.0f;
}

void ScoresGalore::resetScoringForANewRun()
{
    currentScoring = new Scoring();
}
void ScoresGalore::setupForPlayLayer(PlayLayer* playLayer)
{
    resetScoringForANewRun();
    _currentPlayer = playLayer->m_player1;
    _currentPlayUI = PlayUI::create();
    playLayer->m_uiLayer->addChild(_currentPlayUI);
    playLayerWasSetup = true;
    _currentPlayLayer = playLayer;
}
PlayerObject* ScoresGalore::getCurrentPlayer()
{
    return _currentPlayer;
}
void ScoresGalore::cleanupForPlayLayer()
{
    playLayerWasSetup = false;

    
}

void ScoresGalore::handleButtonPush(PlayerButton button)
{
    if (_currentPlayer == nullptr || playLayerWasSetup == false) return;

    if (_currentPlayer->m_isSpider || _currentPlayer->m_isBird || _currentPlayer->m_isSwing || _currentPlayer->m_isDart || _currentPlayer->m_isShip)
        upCombo();
}

void ScoresGalore::onPlayLayerPostUpdate(float dt)
{
    if (feverTimerStarting) {
        timeUntilFever += dt;
        if (timeUntilFever >= 2.0f) {
            if (combosInsideFeverStart < CLICKS_UNTIL_FEVER) {
                feverTimerStarting = false;
                timeUntilFever = 0.0f;
                combosInsideFeverStart = 0;
            } else {
                log::info("Fever!!!");
                _currentPlayUI->startFeverEffect();
                isFeverActive = true;
                feverTimerStarting = false;
                timeUntilFever = 0.0f;
                combosInsideFeverStart = 0;
                feverTime = 0.0f;
            }
        }
    }
    if (isFeverActive) {
        feverTime += dt;
        if (feverTime >= FEVER_TIME) {
            isFeverActive = false;
            _currentPlayUI->stopFeverEffect(false);
        }
    }
}