#include <Geode/Geode.hpp>

using namespace geode::prelude;
#include <Geode/modify/SupportLayer.hpp>
#include <Geode/modify/ProfilePage.hpp>
#include <Geode/modify/RateStarsLayer.hpp>
#include <Geode/modify/RateDemonLayer.hpp>
#include <Geode/modify/LevelInfoLayer.hpp>
class modCheck
{
public:

	void DelayMod(CCObject* sender)
	{
	/*	UploadActionPopup* modCheck = (UploadActionPopup*)sender;*/

		auto scene = CCDirector::get()->getRunningScene();
	
		UploadActionPopup* Check = (UploadActionPopup*)scene->getChildren()->objectAtIndex(1);

		if (Mod::get()->getSettingValue<int64_t>("modType") == 1)
		{
			Check->showSuccessMessage("Success! Moderator access granted.");
		}
		if (Mod::get()->getSettingValue<int64_t>("modType") == 2)
		{
			Check->showSuccessMessage("Success! Elder Moderator \n access granted.");
		}
	}

	void DelayRate(CCObject* sender)
	{
		auto scene = CCDirector::get()->getRunningScene();

		UploadActionPopup* Check = (UploadActionPopup*)scene->getChildren()->objectAtIndex(2);
		Check->showSuccessMessage("Rating submitted!");
		
		
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
			
			if (type == 1)
			{
				p0->m_modBadge = 1;
			}
			if (type == 2)
			{
				p0->m_modBadge = 2;
			}
			if (type == 3)
			{
				p0->m_modBadge = 3;
			}
		}
		
		ProfilePage::loadPageFromUserInfo(p0);

		
	}
};

class $modify(RateStarsLayer)
{
	void onRate(CCObject* sender)
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
		LevelInfoLayer::levelDeleteFailed(a1);

		

		auto obj = (FLAlertLayer*)scene->getChildren()->lastObject();

		
		FLAlertLayer::create("Level Deleted", "The level has been removed from the server", "OK")->show();
		scene->removeChild(obj);
	}
};
