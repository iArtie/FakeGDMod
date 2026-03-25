#include <Geode/Geode.hpp>

using namespace geode::prelude;
#include <Geode/modify/SupportLayer.hpp>
#include <Geode/modify/ProfilePage.hpp>
#include <Geode/modify/RateStarsLayer.hpp>
#include <Geode/modify/RateDemonLayer.hpp>
#include <Geode/modify/LevelInfoLayer.hpp>

template <typename Base, typename T>
inline bool instanceof(const T* ptr) {
    return dynamic_cast<const Base*>(ptr) != nullptr;
}

static const char* getBadgeFrame(int type) {
    switch (type) {
        case 2: return "modBadge_02_001.png";
        case 3: return "modBadge_03_001.png";
        default: return "modBadge_01_001.png";
    }
}

static CCSprite* createBadgeSprite(int type) {
    if (auto badge = CCSprite::createWithSpriteFrameName(getBadgeFrame(type))) {
        return badge;
    }

    // Keep compatibility with older sprite names just in case.
    switch (type) {
        case 2: return CCSprite::createWithSpriteFrameName("mod_badge_02_001.png");
        case 3: return CCSprite::createWithSpriteFrameName("mod_badge_03_001.png");
        default: return CCSprite::createWithSpriteFrameName("mod_badge_01_001.png");
    }
}

class modCheck {
public:
    void DelayMod(CCObject* sender) {
        auto scene = CCDirector::get()->getRunningScene();
        CCObject* pObj = nullptr;

        CCARRAY_FOREACH(static_cast<CCScene*>(scene)->getChildren(), pObj) {
            if (instanceof<UploadActionPopup>(pObj)) {
                auto check = static_cast<UploadActionPopup*>(pObj);

                if (Mod::get()->getSettingValue<int64_t>("modType") == 1) {
                    check->showSuccessMessage("Success! Moderator access granted.");
                }
                if (Mod::get()->getSettingValue<int64_t>("modType") == 2) {
                    check->showSuccessMessage("Success! Elder Moderator \n access granted.");
                }
            }
        }
    }

    void DelayRate(CCObject* sender) {
        auto scene = CCDirector::get()->getRunningScene();
        CCObject* pObj = nullptr;

        CCARRAY_FOREACH(static_cast<CCScene*>(scene)->getChildren(), pObj) {
            if (instanceof<UploadActionPopup>(pObj)) {
                auto check = static_cast<UploadActionPopup*>(pObj);
                check->showSuccessMessage("Rating submitted!");
            }
        }
    }
};

class $modify(SupportLayer) {
    void onRequestAccess(CCObject* sender) {
        auto gm = GameManager::sharedState();

        if (Mod::get()->getSettingValue<int64_t>("modType") == 3) {
            SupportLayer::onRequestAccess(sender);
            gm->m_hasRP = 0;
        }
        else {
            auto popup = UploadActionPopup::create(nullptr, "Loading...");
            popup->show();
            popup->runAction(CCSequence::create(
                CCDelayTime::create(1),
                CCCallFunc::create(this, callfunc_selector(modCheck::DelayMod)),
                nullptr
            ));
            gm->m_hasRP = Mod::get()->getSettingValue<int64_t>("modType");
        }
    }
};

class $modify(ProfilePage) {
    void getUserInfoFinished(GJUserScore* score) {
        ProfilePage::getUserInfoFinished(score);

        if (!score) return;
        if (score->m_accountID != GJAccountManager::get()->m_accountID) return;

        int type = Mod::get()->getSettingValue<int64_t>("modType");
        auto badge = createBadgeSprite(type);
        if (!badge) return;

        badge->setID("fakegdmod-badge");
        badge->setScale(0.6f);

        if (!this->m_usernameLabel) return;

        auto parent = this->m_usernameLabel->getParent();
        if (!parent) return;

        if (auto old = parent->getChildByID("fakegdmod-badge")) {
            old->removeFromParent();
        }

        auto pos = this->m_usernameLabel->getPosition();
        auto size = this->m_usernameLabel->getContentSize() * this->m_usernameLabel->getScale();
        badge->setPosition({
            pos.x - size.width / 2.f - badge->getContentSize().width * 0.6f / 2.f - 8.f,
            pos.y
        });
        parent->addChild(badge, this->m_usernameLabel->getZOrder() + 1);
    }
};

class $modify(RateStarsLayer) {
    void onRate(CCObject* sender) {
        auto layer = static_cast<CCLayer*>(this->getChildren()->objectAtIndex(0));

        if (layer->getChildrenCount() == 3) {
            auto popup = UploadActionPopup::create(nullptr, "Sending rating...");
            popup->show();
            popup->runAction(CCSequence::create(
                CCDelayTime::create(1),
                CCCallFunc::create(this, callfunc_selector(modCheck::DelayRate)),
                nullptr
            ));
        }
        else {
            RateStarsLayer::onRate(sender);
        }
    }
};

class $modify(RateDemonLayer) {
    void onRate(CCObject* sender) {
        auto popup = UploadActionPopup::create(nullptr, "Sending rating...");
        popup->show();
        popup->runAction(CCSequence::create(
            CCDelayTime::create(1),
            CCCallFunc::create(this, callfunc_selector(modCheck::DelayRate)),
            nullptr
        ));
    }
};

class $modify(LevelInfoLayer) {
    void levelDeleteFailed(int a1) {
        auto scene = CCDirector::get()->getRunningScene();
        auto obj = static_cast<FLAlertLayer*>(scene->getChildren()->lastObject());
        (void)obj;

        if (Mod::get()->getSettingValue<int64_t>("modType") == 1 || Mod::get()->getSettingValue<int64_t>("modType") == 2) {
            FLAlertLayer::create("Level Deleted", "The level has been removed from the server", "OK")->show();

            CCObject* pObj = nullptr;
            CCARRAY_FOREACH(static_cast<LevelInfoLayer*>(this)->getChildren(), pObj) {
                if (instanceof<LoadingCircle>(pObj)) {
                    auto loadingCircle = static_cast<LoadingCircle*>(pObj);
                    loadingCircle->setVisible(false);
                }
            }
        }
        else {
            LevelInfoLayer::levelDeleteFailed(a1);
        }
    }
};
