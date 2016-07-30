#ifndef _LOGINSCENE_H
#define _LOGINSCENE_H
#include "cocos2d.h"

#include "ui/CocosGUI.h"
#include "network/SocketIO.h"

class LoginScene :public cocos2d::Layer, public cocos2d::network::SocketIO::SIODelegate
{
	cocos2d::network::SIOClient *_sioClient;
	cocos2d::Label* label;

public:
	virtual bool init();
	CREATE_FUNC(LoginScene);
	static cocos2d::Scene* createScene();

	virtual void onConnect(cocos2d::network::SIOClient* client);
	virtual void onMessage(cocos2d::network::SIOClient* client, const std::string& data);
	virtual void onClose(cocos2d::network::SIOClient* client);
	virtual void onError(cocos2d::network::SIOClient* client, const std::string& data);
	
	//test server call client
	virtual void callClientEvent(cocos2d::network::SIOClient *client, const std::string& data);
	
	//return result server find database 
	virtual void findRes(cocos2d::network::SIOClient *client, const std::string& data);

	virtual void cleanup();
private:
	cocos2d::ui::TextField* password;
	cocos2d::ui::TextField* username;
	void signinCallback(Ref* pSender, cocos2d::ui::Widget::TouchEventType type);
	void signupCallback(Ref* pSender, cocos2d::ui::Widget::TouchEventType type);
	void userNameEvent(Ref *pSender, cocos2d::ui::TextField::EventType type);
	void passWordEvent(Ref *pSender, cocos2d::ui::TextField::EventType type);
	bool findResFlag;

};
#endif // !_LOGINSCENE_H
