#ifndef _PLAY_UI_HPP_
#define _PLAY_UI_HPP_

#include <Geode/Geode.hpp>
#include <Geode/cocos/layers_scenes_transitions_nodes/CCLayer.h>
#include <Geode/cocos/label_nodes/CCLabelBMFont.h>
#include <Geode/cocos/layers_scenes_transitions_nodes/CCScene.h>
#include <Geode/cocos/sprite_nodes/CCSprite.h>
using namespace geode::prelude;

class PlayUI : public cocos2d::CCLayer {
protected:
    bool init();
    PlayUI() {}
    
    cocos2d::CCSize m_fireSpriteNormalSize;
    cocos2d::CCSize m_comboLabelNormalSize;
public:
    static PlayUI* create();

    // variables
    bool m_comboLabelIsDirty;
    int m_newCombo;
    bool m_comboLabelNeedsPulse;
    cocos2d::CCNode* m_comboLabelHolder;
    cocos2d::CCLabelBMFont* m_comboLabel;
    cocos2d::CCSprite* m_fireSprite;
    cocos2d::CCSprite* m_frenzyText;

    bool feverEffectPlaying;

    // methods
    void updateComboLabel(int combo, bool pulse);
    void scheduleUpdateComboLabel(int combo, bool pulse);
    void update(float delta);
    void startFeverEffect();
    void stopFeverEffect(bool immediate);
};

#endif