#ifndef LEVELSELECTLAYER_HPP
#define LEVELSELECTLAYER_HPP

#include <Geode/DefaultInclude.hpp>

class LevelSelectLayer : public cocos2d::CCLayer, public BoomScrollLayerDelegate, public DynamicScrollDelegate {
public:
    static cocos2d::CCScene* scene(int);
    static LevelSelectLayer* create(int);

    void updatePageWithObject(cocos2d::CCObject*, cocos2d::CCObject*);
    void scrollLayerMoved(cocos2d::CCPoint);

    void keyBackClicked();
    void keyDown(cocos2d::enumKeyCodes);

    void scrollLayerScrollingStarted(BoomScrollLayer* p0) {};
    void scrollLayerScrolledToPage(BoomScrollLayer* p0, int) {};
    void scrollLayerWillScrollToPage(BoomScrollLayer* p0, int) {};

    void onDownload(cocos2d::CCObject*);
    void onPrev(cocos2d::CCObject*);
    void onNext(cocos2d::CCObject*);
    void onBack(cocos2d::CCObject*);
    void onInfo(cocos2d::CCObject*);
    void onPlay(cocos2d::CCObject*);

    cocos2d::ccColor3B colorForPage(int page);
    cocos2d::ccColor3B getColorValue(int leve, int level2, float a3);

    bool init(int);
private:
    float m_windowWidth = 0.f;
    GJGroundLayer* m_groundLayer = nullptr;
    cocos2d::CCSprite* m_backgroundSprite = nullptr;
    BoomScrollLayer* m_scrollLayer;
    bool m_masterDetectiveUnlocked = false;
};
#endif