#include <Geode/Geode.hpp>

using namespace geode::prelude;
#include <Geode/modify/SupportLayer.hpp>
#include <Geode/modify/ProfilePage.hpp>
#include <Geode/modify/RateStarsLayer.hpp>
#include <Geode/modify/RateDemonLayer.hpp>
#include <Geode/modify/LevelInfoLayer.hpp>

template<typename Base, typename T>
inline bool instanceof(const T* ptr) {
	return dynamic_cast<const Base*>(ptr) != nullptr;
}

class modCheck
{
public:

	void DelayMod(CCObject* sender)
	{

		auto scene = CCDirector::get()->getRunningScene();
	

		CCObject* pObj = nullptr;

				CCARRAY_FOREACH(((CCScene*)(scene))->getChildren(), pObj) {

					if (instanceof<UploadActionPopup>(pObj)) {

						UploadActionPopup* Check = static_cast<UploadActionPopup*>(pObj);

						if (Mod::get()->getSettingValue<int64_t>("modType") == 1)
						{
							Check->showSuccessMessage("Success! Moderator access granted.");
						}
						if (Mod::get()->getSettingValue<int64_t>("modType") == 2)
						{
							Check->showSuccessMessage("Success! Elder Moderator \n access granted.");
						}
					}
				}
			
		
		
	}

	void DelayRate(CCObject* sender)
	{
		auto scene = CCDirector::get()->getRunningScene();


		CCObject* pObj = nullptr;

		CCARRAY_FOREACH(((CCScene*)(scene))->getChildren(), pObj) {

			if (instanceof<UploadActionPopup>(pObj)) {

				UploadActionPopup* Check = static_cast<UploadActionPopup*>(pObj);
				Check->showSuccessMessage("Rating submitted!");
			}
		}

		
		
		
	}
};

class $modify(SupportLayer) {
	void onRequestAccess(CCObject* sender) {
		auto GM = GameManager::sharedState();

		if (Mod::get()->getSettingValue<int64_t>("modType") == 3)
		{
			SupportLayer::onRequestAccess(sender);
			GM->m_hasRP = 0;
		}
		else
		{
			UploadActionPopup* popup = UploadActionPopup::create(nullptr, "Loading...");
			popup->show();
			popup->runAction((CCSequence::create(
				CCDelayTime::create(1),
				CCCallFunc::create(this, callfunc_selector(modCheck::DelayMod)),
				nullptr
			)));
				GM->m_hasRP = Mod::get()->getSettingValue<int64_t>("modType");
		}
	}	
};

class $modify(ProfilePage)
{
	void loadPageFromUserInfo(GJUserScore* p0)
	{
		int type = Mod::get()->getSettingValue<int64_t>("modType");
		if (this->m_ownProfile)
		{
			switch(type) {
				case 1: p0->m_modBadge = type; break;
				case 2: p0->m_modBadge = type; break;
				case 3: p0->m_modBadge = type; break;
			}
		}
		ProfilePage::loadPageFromUserInfo(p0);
	}
};

class $modify(RateStarsLayer)
{
	void onRate(CCObject* sender)
	{
		CCLayer* layer = static_cast<CCLayer*>(this->getChildren()->objectAtIndex(0));

		if (layer->getChildrenCount() == 3) {
			UploadActionPopup* popup = UploadActionPopup::create(nullptr, "Sending rating...");
			popup->show();
			popup->runAction((CCSequence::create(
				CCDelayTime::create(1),
				CCCallFunc::create(this, callfunc_selector(modCheck::DelayRate)),
				nullptr
			)));
		}
		else {
			RateStarsLayer::onRate(sender);
		}
	}
};

class $modify(RateDemonLayer)
{
	void onRate(CCObject * sender)
	{
		UploadActionPopup* popup = UploadActionPopup::create(nullptr, "Sending rating...");
		popup->show();
		popup->runAction((CCSequence::create(
			CCDelayTime::create(1),
			CCCallFunc::create(this, callfunc_selector(modCheck::DelayRate)),
			nullptr
		)));
	}
};

class $modify(LevelInfoLayer)
{
	void levelDeleteFailed(int a1)
	{
		auto scene = CCDirector::get()->getRunningScene();
		auto obj = (FLAlertLayer*)scene->getChildren()->lastObject();
		
		
		if (Mod::get()->getSettingValue<int64_t>("modType") == 1 || Mod::get()->getSettingValue<int64_t>("modType") == 2)
		{

			

			FLAlertLayer::create("Level Deleted", "The level has been removed from the server", "OK")->show();
			/*scene->removeChild(obj);*/

			CCObject* pObj = nullptr;

			CCARRAY_FOREACH(((LevelInfoLayer*)(this))->getChildren(), pObj) {

				if (instanceof<LoadingCircle>(pObj)) {

					LoadingCircle* loadingCircle = static_cast<LoadingCircle*>(pObj);

					loadingCircle->setVisible(false);
				}
			}
		}
		else
		{
			LevelInfoLayer::levelDeleteFailed(a1);
		}
	}
};
