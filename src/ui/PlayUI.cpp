#include "PlayUI.hpp"
#include <fmt/core.h>
#include <cmath>
#include <algorithm>
#include <Geode/cocos/actions/CCActionInterval.h>
#include <Geode/cocos/actions/CCActionEase.h>

int cachedOpacity = 200;

// protectedd
bool PlayUI::init()
{
    if (!CCLayer::init()) return false;

    cocos2d::CCSize sceneSize = cocos2d::CCScene::get()->getContentSize();

    cachedOpacity = Mod::get()->getSettingValue<int>("overlay-opacity");

    setID("combo-ui");
    m_comboLabelHolder = cocos2d::CCNode::create();
    m_comboLabelHolder->setPosition({ sceneSize.width - 38.0f, 38.0f });
    m_comboLabelHolder->setID("combo-label-holder");
    m_comboLabelHolder->setAnchorPoint({0.5f, 1.0f});
    addChild(m_comboLabelHolder);

    m_comboLabel = cocos2d::CCLabelBMFont::create("0x", "bigFont.fnt");
    m_comboLabel->setOpacity(cachedOpacity);
    
    m_fireSprite = cocos2d::CCSprite::createWithSpriteFrameName("glorb.png"_spr);
    m_fireSprite->setAnchorPoint({0.5f, 0.5f});
    m_fireSprite->setColor({ 200, 128, 255 });
    m_fireSprite->setOpacity(cachedOpacity);

    m_comboLabelHolder->addChild(m_fireSprite);
    m_comboLabelHolder->addChild(m_comboLabel);

    m_frenzyText = cocos2d::CCSprite::createWithSpriteFrameName("frenzy.png"_spr);
    m_frenzyText->setScale(0.175f);
    m_frenzyText->setOpacity(0);
    m_comboLabelHolder->addChild(m_frenzyText);

    m_fireSpriteNormalSize = m_fireSprite->getContentSize();
    m_comboLabelNormalSize = m_comboLabel->getContentSize();

    scheduleUpdate();

    return true;
}

// public
PlayUI* PlayUI::create()
{
    PlayUI* layer = new PlayUI();
    layer->init();
    return layer;
}
void PlayUI::updateComboLabel(int combo, bool pulse) {
    m_comboLabel->setString(fmt::format("{}x", combo).c_str());
    m_comboLabelHolder->setScale(pulse == true ? 1.28f : 1.0f);
    if (pulse) {
        m_comboLabel->setRotation(-15 + (rand() % 30));
    }
}
void PlayUI::scheduleUpdateComboLabel(int combo, bool pulse) {
    m_comboLabelIsDirty = true;
    m_comboLabelNeedsPulse = pulse;
    m_newCombo = combo;
}
void PlayUI::update(float delta) {
    if (m_comboLabelIsDirty) {
        updateComboLabel(m_newCombo, m_comboLabelNeedsPulse);
        m_comboLabelIsDirty = false;
        m_comboLabelNeedsPulse = false;
    }

    m_comboLabel->setScale(std::lerp(m_comboLabel->getScale(), 1.0f, 8.0f * delta));
    m_comboLabel->setRotation(std::lerp(m_comboLabel->getRotation(), 0.0f, 8.0f * delta));

    // fitting fire to the text
    cocos2d::CCSize comboLabelSize = m_comboLabel->getContentSize();

    m_comboLabelHolder->setScale( std::clamp( (comboLabelSize.width - m_comboLabelNormalSize.width) / (m_comboLabelNormalSize.width * 3.0f), 0.75f, 64.0f ));

    comboLabelSize.width *= m_comboLabel->getScale();
    
    float fireXScale = (comboLabelSize.width / m_fireSpriteNormalSize.width) * 1.5f;
    float lerpedXScale = std::lerp(m_fireSprite->getScaleX(), fireXScale, 10.0f * delta);
    m_fireSprite->setScaleX(lerpedXScale);
    m_fireSprite->setScaleY(lerpedXScale);

    float holderXSize = comboLabelSize.width - m_comboLabelNormalSize.width;
    float lerpedXHolderSize = std::lerp(m_comboLabelHolder->getContentSize().width, holderXSize, 7.0f * delta);
    m_comboLabelHolder->setContentSize({ lerpedXHolderSize, lerpedXHolderSize * -0.5f });
    

    if (feverEffectPlaying)
    {
    }
}

void PlayUI::startFeverEffect() {
    feverEffectPlaying = true;
    m_comboLabel->setColor({ 255, 200, 0 });

    cocos2d::CCFadeTo* frenzyTextFadeAction = cocos2d::CCFadeTo::create(0.23f, cachedOpacity);
    cocos2d::CCScaleTo* frenzyTextScaleAction = cocos2d::CCScaleTo::create(0.23f, 0.45f);
    cocos2d::CCMoveTo* frenzyTextMoveAction = cocos2d::CCMoveTo::create(0.24f, { m_frenzyText->getPositionX(), 32.0f });
    m_frenzyText->setOpacity(10);
    m_frenzyText->setScale(0.0f);
    m_frenzyText->setPositionY(38.0f);
    m_frenzyText->runAction(frenzyTextFadeAction);
    m_frenzyText->runAction(cocos2d::CCEaseBackOut::create(frenzyTextScaleAction));
    m_frenzyText->runAction(cocos2d::CCEaseBackOut::create(frenzyTextMoveAction));

    cocos2d::CCTintTo* fireTintToAction = cocos2d::CCTintTo::create(0.23f, 0, 255, 245);
    m_fireSprite->runAction(fireTintToAction);
}
void PlayUI::stopFeverEffect(bool immediate) {
    feverEffectPlaying = false;
    if (immediate) {
        m_comboLabel->setColor({ 255, 255, 255 });
        m_frenzyText->setOpacity(0);
        m_fireSprite->setColor({ 200, 128, 255 });
    } else {
        cocos2d::CCTintTo* tintAction = cocos2d::CCTintTo::create(0.25f, 255, 255, 255);
        m_comboLabel->runAction(tintAction);

        cocos2d::CCFadeTo* frenzyTextFadeAction = cocos2d::CCFadeTo::create(0.23f, 0);
        cocos2d::CCScaleTo* frenzyTextScaleAction = cocos2d::CCScaleTo::create(0.23f, 0.0f);
        cocos2d::CCMoveTo* frenzyTextMoveAction = cocos2d::CCMoveTo::create(0.19f, { m_frenzyText->getPositionX(), 0.0f });
        m_frenzyText->runAction(frenzyTextFadeAction);
        m_frenzyText->runAction(frenzyTextMoveAction);
        m_frenzyText->runAction(frenzyTextScaleAction);

        cocos2d::CCTintTo* fireTintToAction = cocos2d::CCTintTo::create(0.23f, 200, 128, 255);
        m_fireSprite->runAction(fireTintToAction);
    }
}