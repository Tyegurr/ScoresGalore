#ifndef _PLAYER_OBJECT_HOOKS_HPP_
#define _PLAYER_OBJECT_HOOKS_HPP_

#include <Geode/Geode.hpp>
#include <Geode/modify/PlayerObject.hpp>
#include "../ScoresGalore.hpp"
using namespace geode::prelude;

class $modify(PlayerObjectHooks, PlayerObject)
{
    bool pushButton(PlayerButton button) {
        if (!PlayerObject::pushButton(button)) return false;

        if (ScoresGalore::get()->getCurrentPlayer() == this)
            ScoresGalore::get()->handleButtonPush(button);
        
        return true;
    }
    void incrementJumps()
    {
        PlayerObject::incrementJumps();
        if (ScoresGalore::get()->getCurrentPlayer() == this)
            ScoresGalore::get()->upCombo();
    }

    void update(float dt) {
        PlayerObject::update(dt);
    }
};

#endif