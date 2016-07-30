#include "LoginScene.h"
#include "LoadingScene.h"
#include "JsonParser.h"

#include <iostream>

USING_NS_CC;

using namespace  cocos2d::network;

Scene* LoginScene::createScene()
{
	auto scene = Scene::create();
	auto layer = LoginScene::create();
	scene->addChild(layer);
	return scene;
}

bool LoginScene::init()
{
	Layer::init();

	auto winSize = Director::getInstance()->getWinSize();
	auto origin = Director::getInstance()->getVisibleOrigin();
	auto vis = Director::getInstance()->getVisibleSize();

	auto sprite = Sprite::create("bground1.png");
	sprite->setAnchorPoint(Vec2(0.5f, 0.5f));
	sprite->setPosition(Vec2(origin) + Vec2(vis.width / 2, vis.height / 2));
	this->addChild(sprite);

	this->findResFlag = false;
	username = ui::TextField::create("User Name", "Arial", 25);
	//make this TextField password enabled
	username->setPasswordEnabled(false);

	// set the maximum number of characters the user can enter for this TextField
	username->setMaxLength(20);
	username->setPosition(origin + Vec2(vis.width / 2, vis.height / 2 + 50));
	//username->addTouchEventListener([&](Ref* sender, ui::Widget::TouchEventType type){
	//	std::cout << "editing a TextField" << std::endl;
	//});

	this->addChild(username);

	password = ui::TextField::create("PassWord", "Arial", 25);
	password->setPasswordEnabled(true);
	password->setMaxLength(20);
	password->setPosition(origin + Vec2(vis.width / 2, vis.height / 2));
	this->addChild(password);

	//添加事件处理的函数
	username->addEventListener(CC_CALLBACK_2(LoginScene::userNameEvent, this));//XXX是函数所在的类名
	password->addEventListener(CC_CALLBACK_2(LoginScene::passWordEvent, this));//XXX是函数所在的类名

	label = Label::createWithTTF("Hello World", "fonts/Marker Felt.ttf", 24);

	// position the label on the center of the screen
	label->setPosition(Vec2(origin.x + vis.width / 2,
		origin.y + vis.height - label->getContentSize().height));

	// add the label as a child to this layer
	//this->addChild(label, 1);

	auto signin = ui::Button::create("signinbutton.png", "signinbutton.png", "signinbutton.png");
	signin->setTitleText("SignIn");
	signin->setScale(0.8f);
	signin->setPosition(origin + Vec2(vis.width / 2, 140));
	signin->setTag(20);
	signin->addTouchEventListener(CC_CALLBACK_2(LoginScene::signinCallback, this));
	this->addChild(signin);


	auto regis = ui::Button::create("signupbutton.png", "signupbutton.png", "signupbutton.png");
	regis->setTitleText("SignUp");
	regis->setScale(0.8f);
	regis->setPosition(origin + Vec2(vis.width / 2, 90));
	regis->setTag(21);
	regis->addTouchEventListener(CC_CALLBACK_2(LoginScene::signupCallback, this));
	this->addChild(regis);

	//tab key to switch editbox
	auto keyboardlistener = EventListenerKeyboard::create();
	keyboardlistener->onKeyReleased = [signin, regis](cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event){
		auto tag = event->getCurrentTarget()->getTag();
		switch (keyCode){
		case EventKeyboard::KeyCode::KEY_ENTER:
		case EventKeyboard::KeyCode::KEY_TAB:
		//if (tag == signin->getTag())
			break;
		case EventKeyboard::KeyCode::KEY_UP_ARROW:
			break;
		case EventKeyboard::KeyCode::KEY_DOWN_ARROW:
			break;
		}
	};
	//this->_eventDispatcher->addEventListenerWithSceneGraphPriority(keyboardlistener, signin);
	//this->_eventDispatcher->addEventListenerWithSceneGraphPriority(keyboardlistener, regis);

	//button->addTouchEventListener([&](Ref* sender, ui::Widget::TouchEventType type){
	//	switch (type)
	//	{
	//	case ui::Widget::TouchEventType::BEGAN:
	//		break;
	//	case ui::Widget::TouchEventType::ENDED:
	//		std::cout << "Button 1 clicked" << std::endl;
	//		break;
	//	default:
	//		break;
	//	}
	//});

	_sioClient = SocketIO::connect("http://115.28.59.106:2000/", *this);
	//_sioClient = SocketIO::connect("http://127.0.0.1:2000/", *this);

	_sioClient->setTag("Cocos2d-x Client1");

	_sioClient->on("callClientEvent", CC_CALLBACK_2(LoginScene::callClientEvent, this));
	_sioClient->on("findRes", CC_CALLBACK_2(LoginScene::findRes, this));
	return true;
}

void LoginScene::callClientEvent(SIOClient *client, const std::string& data)
{
	//std::cout << "my my my my call!" <<data<<std::endl;
}

void LoginScene::signinCallback(Ref* pSender, ui::Widget::TouchEventType type)
{
	ui::Button *sender = (ui::Button*)pSender;
	//log("%d", sender->getTag());

	switch (type)
	{
	case cocos2d::ui::Widget::TouchEventType::BEGAN:
		break;
	case cocos2d::ui::Widget::TouchEventType::MOVED:
		break;
	case cocos2d::ui::Widget::TouchEventType::ENDED:

	//触发服务器callServerEvent事件
	if (_sioClient != NULL)
	{
		//note: the var format require char type, you need to convert string to char 
		__String* content = __String::createWithFormat("{\"username\":\"%s\",\"password\":\"%s\"}", this->username->getStringValue().c_str(), this->password->getStringValue().c_str());

		_sioClient->emit("signin", content->getCString());
		//_sioClient->emit("callServerEvent", content->getCString());
	}
		break;
	case cocos2d::ui::Widget::TouchEventType::CANCELED:
		break;
	default:
		break;
	}
}

void LoginScene::signupCallback(Ref* pSender, ui::Widget::TouchEventType type)
{
	//向服务器发出消息
	if (_sioClient != NULL)
		_sioClient->send("Hello Socket.IO!");
	//触发服务器callServerEvent事件
	if (_sioClient != NULL)
	{
		//note: the var format require char type, you need to convert string to char 
		__String* content = __String::createWithFormat("{\"username\":\"%s\",\"password\":\"%s\"}", this->username->getStringValue().c_str(), this->password->getStringValue().c_str());

		_sioClient->emit("signup", content->getCString());
		//_sioClient->emit("callServerEvent", content->getCString());
	}
}

void LoginScene::findRes(SIOClient *client, const std::string& data)
{
	ValueMap dict = JsonParser::decode(data.c_str());

	int intResultCode = dict["ResultCode"].asInt();

	auto call = CallFunc::create([](){
		auto scene = LoadingScene::createScene();
		Director::getInstance()->replaceScene(TransitionFade::create(0.5, scene));
	});

	if (intResultCode < 0) {
		//this->findResFlag = false;
		log("Error Code %d", intResultCode);
		//MessageBox("账号或密码错误，请重新输入", "Connect Error!");
	}
	else {
		//this->findResFlag = true;

		//MessageBox("登录成功", "Login in succeed!");
	//等待一会儿，进入
	//if (this->findResFlag)
	//{
		this->runAction(Sequence::create(DelayTime::create(0.21), call, nullptr));
	//}
		ValueMap row = dict["Record"].asValueMap();
		//ValueVector listData = dict["Record"].asValueVector();

		//for (auto& v : listData)
		//{
			//log("--------------------------");
			//ValueMap row = v.asValueMap();

			log("UserName =%s \t PassWord =%s",
				row["username"].asString().c_str(),
				row["password"].asString().c_str());

			//selectedRowId = row["username"].asString();
		//}
	}
}
void LoginScene::userNameEvent(Ref *pSender, ui::TextField::EventType type)
{
	switch (type)
	{
	case ui::TextField::EventType::ATTACH_WITH_IME: //虚拟键盘开启时
	{   //取得触发事件的TextField对象
		ui::TextField* textField = dynamic_cast<ui::TextField*>(pSender);
		break;
	}
	case ui::TextField::EventType::DETACH_WITH_IME: //虚拟键盘关闭时
		break;
	case ui::TextField::EventType::INSERT_TEXT: //用户有输入文本时
		break;
	case ui::TextField::EventType::DELETE_BACKWARD: //用户删除文本时
		break;
	default:
		break;
	}
}
void LoginScene::passWordEvent(Ref *pSender, ui::TextField::EventType type)
{
	switch (type)
	{
	case ui::TextField::EventType::ATTACH_WITH_IME: //虚拟键盘开启时
	{
		ui::TextField* password = dynamic_cast<ui::TextField*>(pSender);
		break;
	}
	case ui::TextField::EventType::DETACH_WITH_IME: //虚拟键盘关闭时
		break;
	case ui::TextField::EventType::INSERT_TEXT: //用户有输入文本时
	{
		break;
	}
	case ui::TextField::EventType::DELETE_BACKWARD: //用户删除文本时
		break;
	default:
		break;
	}
}


//委托协议方法
void LoginScene::onConnect(SIOClient* client)
{
	log("LoginScene::onConnect called");
	__String* s = __String::createWithFormat("%s connected!", client->getTag());
	//label->setString(s->getCString());
}

void LoginScene::onMessage(SIOClient* client, const std::string& data)
{
	log("LoginScene::onMessage received: %s", data.c_str());
	__String* s = __String::createWithFormat("%s  received message.", client->getTag());
	//label->setString(s->getCString());
}

void LoginScene::onClose(SIOClient* client)
{
	log("LoginScene::onClose called");
	__String* s = __String::createWithFormat("%s closed!", client->getTag());
	//label->setString(s->getCString());

	if (client == _sioClient) {
		_sioClient = nullptr;
	}
}

void LoginScene::onError(SIOClient* client, const std::string& data)
{
	log("LoginScene::onError received: %s", data.c_str());
	__String* s = __String::createWithFormat("%s   received error .", client->getTag());
	//label->setString(s->getCString());

}

void LoginScene::cleanup()
{
	Layer::cleanup();
	if (_sioClient != nullptr)
		_sioClient->disconnect();
}
