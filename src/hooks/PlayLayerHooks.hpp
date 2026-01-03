#ifndef _PLAY_LAYER_HOOKS_HPP_
#define _PLAY_LAYER_HOOKS_HPP_

#include <Geode/Geode.hpp>
#include <Geode/modify/PlayLayer.hpp>
#include "../ScoresGalore.hpp"
using namespace geode::prelude;

class $modify(PlayLayerHooks, PlayLayer) {
    struct Fields {
        bool m_customizedSetup;
        ~Fields() {
            ScoresGalore::get()->cleanupForPlayLayer();
        }
    };

    bool init(GJGameLevel* level, bool useReplay, bool dontCreateObjects) {
        if (!PlayLayer::init(level, useReplay, dontCreateObjects)) return false;
        return true;
    }

    void setupForThisPlayLayer()
    {
        ScoresGalore::get()->setupForPlayLayer(this);
    }

    // shitty solution for being setup way too early but it works
    void postUpdate(float dt)
    {
        PlayLayer::postUpdate(dt);
        if (m_uiLayer != nullptr && !m_fields->m_customizedSetup) {
            setupForThisPlayLayer();
            m_fields->m_customizedSetup = true;
        }
        ScoresGalore::get()->onPlayLayerPostUpdate(dt);
    }

    void resetLevel()
    {
        PlayLayer::resetLevel();
        ScoresGalore::get()->resetCombo();
    }
};

#endif