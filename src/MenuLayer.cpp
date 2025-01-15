#include "MenuLayer.hpp"

using namespace geode::prelude;

CCScene* MenuLayer::scene() {
    auto ret = CCScene::create();
    ret->addChild(create());
    return ret;
}

MenuLayer* MenuLayer::create() {
    auto ret = new MenuLayer();
    if(ret->init()) {
        ret->autorelease();
        return ret;
    }
    delete ret;
    return nullptr;
}

bool MenuLayer::init() {
    if(!CCLayer::init())
        return false;

    auto gm = GameManager::get();
    auto director = CCDirector::get();
    auto winSize = director->getWinSize();

    gm->fadeInMenuMusic();
    gm->m_inMenuLayer = true;
    gm->m_sceneEnum = 0;
    gm->m_somethingInMenuLayer = false;

    m_menuGameLayer = MenuGameLayer::create();
    addChild(m_menuGameLayer);

    auto logoSpr = CCSprite::createWithSpriteFrameName("GJ_logo_001.png");
    logoSpr->setPosition(ccp(winSize.width / 2.f, director->getScreenTop() - 50.f));
    addChild(logoSpr);

    auto btnMenu = CCMenu::create();
    btnMenu->setPosition(winSize / 2 + ccp(0.f, 10.f));
    addChild(btnMenu);

    auto playSpr = CCSprite::createWithSpriteFrameName("GJ_playBtn_001.png");
    auto playBtn = CCMenuItemSpriteExtra::create(playSpr, this, menu_selector(MenuLayer::onPlay));
    btnMenu->addChild(playBtn);

    auto garageSpr = CCSprite::createWithSpriteFrameName("GJ_garageBtn_001.png");
    auto garageBtn = CCMenuItemSpriteExtra::create(garageSpr, this, menu_selector(MenuLayer::onGarage));
    garageBtn->setPosition(ccp(-110.f, 0.f));
    btnMenu->addChild(garageBtn);

    if(!gm->m_clickedGarage) {
        auto chrSelectSpr = CCSprite::createWithSpriteFrameName("GJ_chrSel_001.png");
        chrSelectSpr->setPosition(btnMenu->convertToWorldSpace(garageBtn->getPosition()) - ccp(50, 50));
        addChild(chrSelectSpr);
    }

    auto creatorSpr = CCSprite::createWithSpriteFrameName("GJ_creatorBtn_001.png");
    auto creatorBtn = CCMenuItemSpriteExtra::create(creatorSpr, this, menu_selector(MenuLayer::onCreator));
    creatorBtn->setPosition(ccp(110.f, 0.f));
    btnMenu->addChild(creatorBtn);

    if(!gm->m_clickedEditor) {
        auto lvlEditSpr = CCSprite::createWithSpriteFrameName("GJ_lvlEdit_001.png");
        lvlEditSpr->setPosition(btnMenu->convertToWorldSpace(creatorBtn->getPosition()) + ccp(50, -50));
        addChild(lvlEditSpr);
    }

    auto bottomMenu = CCMenu::create();
    addChild(bottomMenu);

    auto achSpr = CCSprite::createWithSpriteFrameName("GJ_achBtn_001.png");
    auto achBtn = CCMenuItemSpriteExtra::create(achSpr, this, menu_selector(MenuLayer::onAchievements));
    bottomMenu->addChild(achBtn);

    auto optionsSpr = CCSprite::createWithSpriteFrameName("GJ_optionsBtn_001.png");
    auto optionsBtn = CCMenuItemSpriteExtra::create(optionsSpr, this, menu_selector(MenuLayer::onOptions));
    bottomMenu->addChild(optionsBtn);

    auto statsSpr = CCSprite::createWithSpriteFrameName("GJ_statsBtn_001.png");
    auto statsBtn = CCMenuItemSpriteExtra::create(statsSpr, this, menu_selector(MenuLayer::onStats));
    bottomMenu->addChild(statsBtn);

    auto ngSpr = CCSprite::createWithSpriteFrameName("GJ_ngBtn_001.png");
    auto ngBtn = CCMenuItemSpriteExtra::create(ngSpr, this, menu_selector(MenuLayer::onNewgrounds));
    bottomMenu->addChild(ngBtn);

    bottomMenu->setPosition(ccp(winSize.width / 2, director->getScreenBottom() + 45.f));
    bottomMenu->alignItemsHorizontallyWithPadding(5.f);

    auto robtopSpr = CCSprite::createWithSpriteFrameName("robtoplogo_small.png");
    robtopSpr->setScale(0.8f);
    auto robtopBtn = CCMenuItemSpriteExtra::create(robtopSpr, this, menu_selector(MenuLayer::onRobtop));
    robtopBtn->setSizeMult(1.1f);

    auto socialMenu = CCMenu::create(robtopBtn, 0);
    socialMenu->setPosition(ccp(director->getScreenLeft() + 50.f, director->getScreenBottom() + 25.f));
    addChild(socialMenu);

    auto fbSpr = CCSprite::createWithSpriteFrameName("gj_fbIcon_001.png");
    fbSpr->setScale(0.8f);
    auto fbBtn = CCMenuItemSpriteExtra::create(fbSpr, this, menu_selector(MenuLayer::onFacebook));
    fbBtn->setSizeMult(1.5f);
    socialMenu->addChild(fbBtn);

    auto twSpr = CCSprite::createWithSpriteFrameName("gj_twIcon_001.png");
    twSpr->setScale(0.8f);
    auto twBtn = CCMenuItemSpriteExtra::create(twSpr, this, menu_selector(MenuLayer::onTwitter));
    twBtn->setSizeMult(1.5f);
    socialMenu->addChild(twBtn);

    auto ytSpr = CCSprite::createWithSpriteFrameName("gj_ytIcon_001.png");
    ytSpr->setScale(0.8f);
    auto ytBtn = CCMenuItemSpriteExtra::create(ytSpr, this, menu_selector(MenuLayer::onYouTube));
    ytBtn->setSizeMult(1.5f);
    socialMenu->addChild(ytBtn);

    auto twitchSpr = CCSprite::createWithSpriteFrameName("gj_twitchIcon_001.png");
    twitchSpr->setScale(0.8f);
    auto twitchBtn = CCMenuItemSpriteExtra::create(twitchSpr, this, menu_selector(MenuLayer::onTwitch));
    twitchBtn->setSizeMult(1.5f);
    socialMenu->addChild(twitchBtn);

    auto discordSpr = CCSprite::createWithSpriteFrameName("gj_discordIcon_001.png");
    discordSpr->setScale(0.8f);
    auto discordBtn = CCMenuItemSpriteExtra::create(discordSpr, this, menu_selector(MenuLayer::onDiscord));
    discordBtn->setSizeMult(1.5f);
    socialMenu->addChild(discordBtn);

    CCPoint socialPoint = socialMenu->convertToNodeSpace(ccp(director->getScreenLeft() + 22.f, director->getScreenBottom() + 55.f));

    fbBtn->setPosition(socialPoint);
    twBtn->setPosition(socialPoint + ccp(29.f, 0.f));
    ytBtn->setPosition(twBtn->getPosition() + ccp(29.f, 0.f));
    twitchBtn->setPosition(ytBtn->getPosition() + ccp(29.f, 0.f));
    discordBtn->setPosition(ytBtn->getPosition() + ccp(29.f, -29.f));

    auto moreGamesSpr = CCSprite::createWithSpriteFrameName("GJ_moreGamesBtn_001.png");
    moreGamesSpr->setScale(0.9f);
    auto moreGamesBtn = CCMenuItemSpriteExtra::create(moreGamesSpr, this, menu_selector(MenuLayer::onMoreGames));

    auto moreGamesMenu = CCMenu::create(moreGamesBtn, 0);
    moreGamesMenu->setPosition(ccp(director->getScreenRight() - 43.f, director->getScreenBottom() + 45.f));
    addChild(moreGamesMenu);

    gm->m_unknownBool3 = false;

    auto closeSpr = CCSprite::createWithSpriteFrameName("GJ_closeBtn_001.png");
    closeSpr->setScale(0.7f);
    auto closeBtn = CCMenuItemSpriteExtra::create(closeSpr, this, menu_selector(MenuLayer::onQuit));
    closeBtn->setPosition(moreGamesMenu->convertToNodeSpace(ccp(18.f, director->getScreenTop() - 18.f)));
    moreGamesMenu->addChild(closeBtn);

    if(PlatformToolbox::isControllerConnected()) {
        auto controllerPlaySpr = CCSprite::createWithSpriteFrameName("controllerBtn_Start_001.png");
        controllerPlaySpr->setPosition(btnMenu->convertToWorldSpace(playBtn->getPosition()) - ccp(0.f, 66.f));
        addChild(controllerPlaySpr);

        auto controllerCreatorSpr = CCSprite::createWithSpriteFrameName("controllerBtn_Y_001.png");
        controllerCreatorSpr->setPosition(btnMenu->convertToWorldSpace(creatorBtn->getPosition()) - ccp(0.f, 45.f));
        controllerCreatorSpr->setScale(0.8f);
        addChild(controllerCreatorSpr);

        auto controllerGarageSpr = CCSprite::createWithSpriteFrameName("controllerBtn_X_001.png");
        controllerGarageSpr->setPosition(btnMenu->convertToWorldSpace(garageBtn->getPosition()) - ccp(0.f, 45.f));
        controllerGarageSpr->setScale(0.8f);
        addChild(controllerGarageSpr);

        auto controllerSettingsSpr = CCSprite::createWithSpriteFrameName("controllerBtn_DPad_Down_001.png");
        controllerSettingsSpr->setPosition(bottomMenu->convertToWorldSpace(optionsBtn->getPosition()) - ccp(0.f, 34.f));
        controllerSettingsSpr->setScale(0.5f);
        addChild(controllerSettingsSpr);

        if(!gm->getGameVariable("0028")) {
            int yOffset = GJAccountManager::get()->m_accountID > 0 ? 15.f : 0.f;

            auto controllerMouseSpr = CCSprite::createWithSpriteFrameName("controllerBtn_LThumb_001.png");
            controllerMouseSpr->setPosition(ccp(director->getScreenLeft() + 25.f, winSize.height / 2 + 25.f + yOffset));
            addChild(controllerMouseSpr);

            auto controllerClickSpr = CCSprite::createWithSpriteFrameName("controllerBtn_A_001.png");
            controllerClickSpr->setPosition(controllerMouseSpr->getPosition() - ccp(0.f, 30.f));
            addChild(controllerClickSpr);

            auto controllerMouseLabel = CCLabelBMFont::create("Mouse", "goldFont.fnt");
            controllerMouseLabel->setScale(0.5f);
            controllerMouseLabel->setAnchorPoint(ccp(0.f, 0.6f));
            controllerMouseLabel->setPosition(controllerMouseSpr->getPosition() + ccp(16.f, 1.f));
            addChild(controllerMouseLabel);

            auto controllerClickLabel = CCLabelBMFont::create("Click", "goldFont.fnt");
            controllerClickLabel->setScale(0.5f);
            controllerClickLabel->setAnchorPoint(ccp(0.f, 0.6f));
            controllerClickLabel->setPosition(controllerClickSpr->getPosition() + ccp(16.f, 1.f));
            addChild(controllerClickLabel);
        }
    }

    auto profileSpr = CCSprite::createWithSpriteFrameName("GJ_profileButton_001.png");
    m_profileButton = CCMenuItemSpriteExtra::create(profileSpr, this, menu_selector(MenuLayer::onMyProfile));
    btnMenu->addChild(m_profileButton, 2);
    m_profileButton->setPosition(btnMenu->convertToNodeSpace(ccp(45.f, 105.f)));

    m_profileLabel = CCLabelBMFont::create(" ", "goldFont.fnt");
    addChild(m_profileLabel);
    m_profileLabel->setPosition(btnMenu->convertToWorldSpace(m_profileButton->getPosition()) + ccp(2.f, 36.f));
    m_profileLabel->limitLabelWidth(70.f, 0.7f, 0.f);

    if(!gm->getGameVariable("0048") && gm->m_clickedGarage) {
        m_viewProfileSprite = CCSprite::createWithSpriteFrameName("GJ_viewProfileTxt_001.png");
        addChild(m_viewProfileSprite);
        m_viewProfileSprite->setPosition(btnMenu->convertToWorldSpace(m_profileButton->getPosition()) + ccp(76.f, -1.f));
    }
    updateUserProfileButton();
    gm->m_menuLayer = this;

    auto dailyRewardSpr = CCSprite::createWithSpriteFrameName("GJ_dailyRewardBtn_001.png");
    auto dailyRewardBtn = CCMenuItemSpriteExtra::create(dailyRewardSpr, this, menu_selector(MenuLayer::onDaily));
    dailyRewardBtn->setPosition(bottomMenu->convertToNodeSpace(ccp(director->getScreenRight() - 40.f, winSize.height / 2 + 20.f)));
    dailyRewardBtn->setSizeMult(1.5f);
    bottomMenu->addChild(dailyRewardBtn);

    if(!gm->getUGV("30")) {
        m_showingTOS = true;
        auto callfunc = CCCallFunc::create(this, callfunc_selector(MenuLayer::showTOS));
        auto delay = CCDelayTime::create(0.01);
        auto sequence = CCSequence::create(delay, callfunc, 0);
        runAction(sequence);
    }

    if(!GameLevelManager::get()->m_testedNetwork) {
        auto callfunc = CCCallFunc::create(this, callfunc_selector(MenuLayer::firstNetworkTest));
        auto delay = CCDelayTime::create(0.01);
        auto sequence = CCSequence::create(delay, callfunc, 0);
        runAction(sequence);
    }
    
    setKeyboardEnabled(true);
    setKeypadEnabled(true);

    return true;
}

void MenuLayer::keyBackClicked() {
    onQuit(nullptr);
}

void MenuLayer::keyDown(enumKeyCodes key) {

    if(key == CONTROLLER_Y)
        return onCreator(nullptr);

    if(key == enumKeyCodes::KEY_Space || key == enumKeyCodes::KEY_Enter || key == enumKeyCodes::CONTROLLER_Start)
        return onPlay(nullptr);

    if(key == enumKeyCodes::CONTROLLER_Down)
        return onOptions(nullptr);

    if(key == enumKeyCodes::CONTROLLER_X)
        return onGarage(nullptr);

    CCLayer::keyDown(key);
}

void MenuLayer::FLAlert_Clicked(FLAlertLayer* alert, bool btn2) {
    if(btn2) {
        switch(alert->getTag()) {
            case 0: {
                auto app = CCApplication::get();
                app->setShutdownCalled(true);
                app->trySaveGame(true);

                auto delay = CCDelayTime::create(0.5f);
                auto callfunc = CCCallFunc::create(this, callfunc_selector(MenuLayer::endGame));
                auto sequence = CCSequence::create(delay, callfunc, 0);
                CCDirector::get()->getActionManager()->addAction(sequence, this, false);

                break;
            }
            case 1: {
                GameManager::get()->setGameVariable("0034", true);
            }
            case 2: {
                CCApplication::get()->sharedApplication()->openURL(CCString::createWithFormat("https://www.robtopgames.com/download/gj%i", 0)->getCString());
            }
            case 3: {
                CCApplication::get()->sharedApplication()->openURL("https://www.newgrounds.com/audio");
            }
        }
    }
}

void MenuLayer::firstNetworkTest() {
    GameLevelManager->get()->performNetworkTest();
}

void MenuLayer::showTOS() {
    TOSPopup::create()->show();
}

void MenuLayer::updateUserProfileButton() {
    auto am = GJAccountManager::get();
    bool loggedIn = am->m_accountID > 0;

    m_profileButton->setVisible(loggedIn);
    m_profileLabel->setVisible(loggedIn);

    if(loggedIn) {
        m_profileLabel->setString(am->m_username.c_str());
        m_profileLabel->limitLabelWidth(70.f, 0.7f, 0.f);
    }
}

void MenuLayer::endGame() {
    CCDirector::get()->end();
}

void MenuLayer::willClose() {
    GameManager::get()->m_inMenuLayer = false;
}

void MenuLayer::openOptions(bool video) {
    auto layer = OptionsLayer::create();
    addChild(layer, 100);
    layer->showLayer(video);
    if(video) {
        auto videoLayer = VideoOptionsLayer::create();
        layer->addChild(videoLayer, 100);
    }
}

void MenuLayer::onOptionsInstant() {
    openOptions(true);
}

void MenuLayer::onPlay(CCObject* sender) {
    if(!m_showingTOS) {
        willClose();
        GameManager::sharedState()->m_sceneEnum = 8;
        CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(0.5f, LevelSelectLayer::scene(0)));
    }
}

void MenuLayer::onGarage(CCObject* sender) {
    if(!m_showingTOS) {
        willClose();
        GameManager::sharedState()->m_clickedGarage = true;
        CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(0.5f, GJGarageLayer::scene()));
    }
}

void MenuLayer::onCreator(CCObject* sender) {
    if(!m_showingTOS) {
        willClose();
        GameManager::sharedState()->m_clickedEditor = true;
        CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(0.5f, CreatorLayer::scene()));
    }
}

void MenuLayer::onAchievements(CCObject* sender) {
    auto layer = AchievementsLayer::create();
    addChild(layer, 100);
    layer->showLayer(false);
}

void MenuLayer::onOptions(CCObject* sender) {
    openOptions(false);
}

void MenuLayer::onStats(CCObject* sender) {
    auto layer = StatsLayer::create();
    addChild(layer, 100);
    layer->showLayer(false);
}

void MenuLayer::onNewgrounds(CCObject* sender) {
    auto alert = FLAlertLayer::create(this, "Newgrounds", "Visit Newgrounds to find awesome music?", "Cancel", "Open", 300.f);
    alert->setTag(3);
    alert->show();
}

void MenuLayer::onRobtop(CCObject* sender) {
    CCApplication::get()->openURL("https://www.robtopgames.com");
}

void MenuLayer::onFacebook(CCObject* sender) {
    CCApplication::get()->openURL("https://www.facebook.com/geometrydash");
}

void MenuLayer::onTwitter(CCObject* sender) {
    CCApplication::get()->openURL("https://twitter.com/robtopgames");
}

void MenuLayer::onYouTube(CCObject* sender) {
    CCApplication::get()->openURL("https://www.youtube.com/user/RobTopGames");
}

void MenuLayer::onTwitch(CCObject* sender) {
    CCApplication::get()->openURL("https://www.twitch.tv/directory/category/geometry-dash");
}

void MenuLayer::onDiscord(CCObject* sender) {
    CCApplication::get()->openURL("https://discord.com/invite/geometrydash");
}

void MenuLayer::onMoreGames(CCObject* sender) {
    auto gm = GameManager::get();
    gm->setGameVariable("0053", true);
    gm->reportAchievementWithID("geometry.ach.moreGames", 100, false);
    auto moreGames = GJMoreGamesLayer::create();
    addChild(moreGames, 100);
    moreGames->showLayer(false);
}

void MenuLayer::onQuit(CCObject* sender) {
    auto alert = FLAlertLayer::create(this, "Quit Game", "Are you sure you want to <cr>quit</c>?", "Cancel", "Yes", 300.f, false, 0.f, 1.f);
    alert->setTag(0);
    alert->show();
}

void MenuLayer::onMyProfile(CCObject* sender) {
    GameManager::get()->setGameVariable("0048", true);
    ProfilePage::create(GJAccountManager::get()->m_accountID, true)->show();
}

void MenuLayer::onDaily(CCObject* sender) {
    RewardsPage::create()->show();
}

MenuLayer::~MenuLayer() {
    GameManager::get()->m_menuLayer = nullptr;
    willClose();
}