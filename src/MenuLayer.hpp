#ifndef MENULAYER_HPP
#define MENULAYER_HPP

#include <Geode/DefaultInclude.hpp>

class MenuLayer : public cocos2d::CCLayer, public FLAlertLayerProtocol {
public:
    ~MenuLayer();

    static MenuLayer* create();
    static cocos2d::CCScene* scene();

    bool init();

    void keyBackClicked();
    void keyDown(cocos2d::enumKeyCodes);
    void FLAlert_Clicked(FLAlertLayer*, bool);

    void firstNetworkTest();
    void showTOS();
    void updateUserProfileButton();
    void endGame();
    void willClose();
    void openOptions(bool video);
    void onOptionsInstant();

    void onPlay(CCObject*);
    void onGarage(CCObject*);
    void onCreator(CCObject*);

    void onAchievements(CCObject*);
    void onOptions(CCObject*);
    void onStats(CCObject*);
    void onNewgrounds(CCObject*);

    void onRobtop(CCObject*);
    void onFacebook(CCObject*);
    void onTwitter(CCObject*);
    void onYouTube(CCObject*);
    void onTwitch(CCObject*);
    void onDiscord(CCObject*);

    void onMoreGames(CCObject*);
    void onQuit(CCObject*);
    void onMyProfile(CCObject*);
    void onDaily(CCObject*);
private:
    bool m_showingTOS;
    cocos2d::CCSprite* m_viewProfileSprite = nullptr;
    cocos2d::CCLabelBMFont* m_profileLabel = nullptr;
    CCMenuItemSpriteExtra* m_profileButton = nullptr;
    MenuGameLayer* m_menuGameLayer = nullptr;
    
};
#endif
