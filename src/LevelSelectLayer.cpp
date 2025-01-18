#include "LevelSelectLayer.hpp"

using namespace geode::prelude;

CCScene* LevelSelectLayer::scene(int page) {
    auto ret = CCScene::create();
    ret->addChild(create(page));
    return ret;
}

LevelSelectLayer* LevelSelectLayer::create(int page) {
    auto ret = new LevelSelectLayer();
    if(ret->init(page)) {
        ret->autorelease();
        return ret;
    }
    delete ret;
    return nullptr;
}

bool LevelSelectLayer::init(int page) {
    if(!CCLayer::init())
        return false;

    auto am = AchievementManager::sharedState();
    auto gm = GameManager::get();
    auto gsm = GameStatsManager::get();
    auto glm = GameLevelManager::get();

    auto director = CCDirector::get();
    auto winSize = director->getWinSize();

    if(gsm->isItemUnlocked(UnlockType::GJItem, 4)) {
        gm->setUGV("6", true);
    }

    setKeyboardEnabled(true);
    setKeypadEnabled(true);

    m_windowWidth = winSize.width;
    m_masterDetectiveUnlocked = am->isAchievementEarned("geometry.ach.secret04");

    m_backgroundSprite = CCSprite::create("GJ_gradientBG.png");
    m_backgroundSprite->setAnchorPoint({ 0.f, 0.f });
    addChild(m_backgroundSprite, -2);

    m_backgroundSprite->setScaleX((winSize.width + 10.f) / m_backgroundSprite->getTextureRect().size.width);
    m_backgroundSprite->setScaleY((winSize.height + 10.f) / m_backgroundSprite->getTextureRect().size.height);
    m_backgroundSprite->setPosition(ccp(-5.f, -5.f));
    m_backgroundSprite->setColor({ 40, 125, 255 });

    m_groundLayer = GJGroundLayer::create(gm->m_loadedGroundID, 1);
    m_groundLayer->setPosition(ccp(0, 50)); // Actually GJGroundLayer::positionGround
    addChild(m_groundLayer, 0);

    auto topBar = CCSprite::createWithSpriteFrameName("GJ_topBar_001.png");
    topBar->setAnchorPoint(ccp(0.5f, 1.f));
    topBar->setPosition(ccp(winSize.width / 2, director->getScreenTop() + 1.f));
    addChild(topBar, 1);

    auto sideArtLeft = CCSprite::createWithSpriteFrameName("GJ_sideArt_001.png");
    sideArtLeft->setAnchorPoint(ccp(0.f, 0.f));
    sideArtLeft->setPosition(ccp(director->getScreenLeft() - 1.f, director->getScreenBottom() - 1.f));
    addChild(sideArtLeft, 1);

    auto sideArtRight = CCSprite::createWithSpriteFrameName("GJ_sideArt_001.png");
    sideArtRight->setAnchorPoint(ccp(1.f, 0.f));
    sideArtRight->setPosition(ccp(director->getScreenRight() + 1.f, director->getScreenBottom() - 1.f));
    sideArtRight->setFlipX(true);
    addChild(sideArtRight, 1);

    auto levelPages = CCArray::create();
    auto mainLevels = CCArray::create();

    for(size_t i = 0; i < 22; i++)
        mainLevels->addObject(glm->getMainLevel(i + 1, true));

    for(size_t i = 0; i < 3; i++)
        levelPages->addObject(LevelPage::create(nullptr));

    auto theTower = GJGameLevel::create();
    theTower->m_levelID = -2;
    mainLevels->addObject(theTower);

    auto comingSoon = GJGameLevel::create();
    comingSoon->m_levelID = -1;
    mainLevels->addObject(comingSoon);

    m_scrollLayer = BoomScrollLayer::create(levelPages, 0, true, mainLevels, this);
    addChild(m_scrollLayer);
    m_scrollLayer->setPagesIndicatorPosition(ccp(winSize.width / 2, director->getScreenBottom() + 15.f));
    m_scrollLayer->m_extendedLayer->m_delegate = this;

    int pageCount = m_scrollLayer->getTotalPages();
    if(page == 0) {
        scrollLayerMoved(m_scrollLayer->m_extendedLayer->getPosition());
    } else {
        if(page - 1 == pageCount) m_scrollLayer->instantMoveToPage(pageCount - 2);
        m_scrollLayer->instantMoveToPage(page);
    }

    auto soundtrackLabel = CCLabelBMFont::create("Download the soundtrack", "bigFont.fnt");
    soundtrackLabel->setScale(0.5f);
    auto soundtrackBtn = CCMenuItemSpriteExtra::create(soundtrackLabel, this, menu_selector(LevelSelectLayer::onDownload));
    soundtrackBtn->setSizeMult(1.1f);

    auto soundtrackMenu = CCMenu::create(soundtrackBtn, 0);
    addChild(soundtrackMenu);
    soundtrackMenu->setPosition(ccp(winSize.width / 2, director->getScreenBottom() + 35.f));

    auto arrowMenu = CCMenu::create();
    addChild(arrowMenu, 5);

    CCSprite* leftNavSpr = nullptr;

    if(!PlatformToolbox::isControllerConnected()) {
        leftNavSpr = CCSprite::createWithSpriteFrameName("navArrowBtn_001.png");
        leftNavSpr->setFlipX(true);
    } else {
        leftNavSpr = CCSprite::createWithSpriteFrameName("controllerBtn_DPad_Left_001.png");
    }

    auto leftNavBtn = CCMenuItemSpriteExtra::create(leftNavSpr, this, menu_selector(LevelSelectLayer::onPrev));
    arrowMenu->addChild(leftNavBtn);
    leftNavBtn->setSizeMult(2.f);
    leftNavBtn->setPosition(arrowMenu->convertToNodeSpace(ccp(director->getScreenLeft() + 25.f, winSize.height / 2)));

    auto rightNavSpr = CCSprite::createWithSpriteFrameName(PlatformToolbox::isControllerConnected() ? "controllerBtn_DPad_Right_001.png" : "navArrowBtn_001.png");

    auto rightNavBtn = CCMenuItemSpriteExtra::create(rightNavSpr, this, menu_selector(LevelSelectLayer::onNext));
    arrowMenu->addChild(rightNavBtn);
    rightNavBtn->setSizeMult(2.f);
    rightNavBtn->setPosition(arrowMenu->convertToNodeSpace(ccp(director->getScreenRight() - 25.f, winSize.height / 2)));

    auto backSpr = CCSprite::createWithSpriteFrameName("GJ_arrow_01_001.png");
    auto backBtn = CCMenuItemSpriteExtra::create(backSpr, this, menu_selector(LevelSelectLayer::onBack));
    backBtn->setSizeMult(1.6f);

    auto backMenu = CCMenu::create(backBtn, 0);
    addChild(backMenu, 1);
    backMenu->setPosition(ccp(director->getScreenLeft() + 25.f, director->getScreenTop() - 22.f));

    auto infoMenu = CCMenu::create();

    gm->m_somethingInMenuLayer = false;
    auto infoSpr = CCSprite::createWithSpriteFrameName("GJ_infoIcon_001.png");
    auto infoBtn = CCMenuItemSpriteExtra::create(infoSpr, this, menu_selector(LevelSelectLayer::onInfo));
    infoBtn->setSizeMult(2.f);

    infoMenu->addChild(infoBtn);
    addChild(infoMenu);
    infoMenu->setPosition(ccp(director->getScreenRight() - 20.f, director->getScreenTop() - 20.f));

    if(PlatformToolbox::isControllerConnected()) GameToolbox::addBackButton(this, backBtn);

    return true;
}

cocos2d::ccColor3B LevelSelectLayer::colorForPage(int page) {
    auto gm = GameManager::get();

    if(page == 0) return gm->colorForIdx(5);
    else if (page % 22 == 0) return { 37, 44, 52 };
    
    switch(page) {
        case 1: return gm->colorForIdx(7);
        case 2: return gm->colorForIdx(8);
        case 3: return gm->colorForIdx(9);
        case 4: return gm->colorForIdx(10);
        case 5: return gm->colorForIdx(11);
        case 6: return gm->colorForIdx(1);
        case 7: return gm->colorForIdx(3);
        case 8: return gm->colorForIdx(4);
        default: return colorForPage(page % 9);
    }

    return { 255, 255, 255 };
}

ccColor3B LevelSelectLayer::getColorValue(int level1, int level2, float a3) {

    float mod = a3 / 0.6666667 - 0.2f;
    if (mod < 1.0f) {
        if (mod <= 0.0f)
            mod = 0.0f;
    }
    else
        mod = 1.0f;

    ccColor3B col1 = colorForPage(level1);
    ccColor3B col2 = colorForPage(level2);

    ccColor3B col3 = {
    static_cast<GLubyte>((col2.r * mod) + col1.r * (1.0f - mod)),
    static_cast<GLubyte>((col2.g * mod) + col1.g * (1.0f - mod)),
    static_cast<GLubyte>((col2.b * mod) + col1.b * (1.0f - mod))
    };

    return col3;
}

void LevelSelectLayer::scrollLayerMoved(CCPoint point) {
    int pageCount = m_scrollLayer->getTotalPages();

    float x = -point.x / m_windowWidth;
        while(x < 0.f)
        x += pageCount;

    int floorX = std::floor(x);

    int x2 = 0;
    if (pageCount != 0) {
        x2 = floorX / pageCount;
    }
    int x3 = floorX - x2 * pageCount;
    x2 = x3 + 1;
    if (pageCount == x2) {
        x2 = 0;
    }

    auto color = getColorValue(x3, x2, x - floorX);
    m_backgroundSprite->setColor(color);

    m_groundLayer->updateGround01Color({ static_cast<GLubyte>(color.r * 0.8), static_cast<GLubyte>(color.g * 0.8), static_cast<GLubyte>(color.b * 0.8) });
    m_groundLayer->updateGround02Color({ static_cast<GLubyte>(color.r * 0.9), static_cast<GLubyte>(color.g * 0.9), static_cast<GLubyte>(color.b * 0.9) });
}

void LevelSelectLayer::updatePageWithObject(CCObject* page, CCObject* level) {
    static_cast<LevelPage*>(page)->updateDynamicPage(static_cast<GJGameLevel*>(level));
}

void LevelSelectLayer::onDownload(CCObject* sender) {
    auto layer = SongsLayer::create();
    addChild(layer, 100);
    layer->showLayer(false);
}

void LevelSelectLayer::onPrev(CCObject* sender) {
    m_scrollLayer->quickUpdate();
    m_scrollLayer->moveToPage(m_scrollLayer->m_page - 1);
}

void LevelSelectLayer::onNext(CCObject* sender) {
    m_scrollLayer->quickUpdate();
    m_scrollLayer->moveToPage(m_scrollLayer->m_page + 1);
}

void LevelSelectLayer::onBack(CCObject* sender) {
    CCDirector::get()->replaceScene(CCTransitionFade::create(0.5f, MenuLayer::scene(false)));
}

void LevelSelectLayer::onInfo(CCObject* sender) {
    static_cast<LevelPage*>(m_scrollLayer->getPage(m_scrollLayer->m_page))->onInfo(sender);
}

void LevelSelectLayer::onPlay(CCObject* sender) {
    static_cast<LevelPage*>(m_scrollLayer->getPage(m_scrollLayer->m_page))->onPlay(sender);
}

void LevelSelectLayer::keyBackClicked() {
    onBack(nullptr);
}

void LevelSelectLayer::keyDown(enumKeyCodes key) {
    if(key == enumKeyCodes::KEY_Right) {
        onNext(nullptr);
        return;
    }

    if(key == enumKeyCodes::KEY_Left) {
        onPrev(nullptr);
        return;
    }

    if(key == enumKeyCodes::CONTROLLER_A || key == enumKeyCodes::KEY_Enter || key == enumKeyCodes::KEY_Space) {
        onPlay(nullptr);
        return;
    }

    CCLayer::keyDown(key);
}