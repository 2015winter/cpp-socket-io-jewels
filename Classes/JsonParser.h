//
//  Created by 关东升 on 2014-5-18.
//  本书网站：http://www.cocoagame.net
//  智捷iOS课堂在线课堂：http://v.51work6.com
//  智捷iOS课堂新浪微博：http://weibo.com/u/3215753973
//  作者微博：http://weibo.com/516inc
//  QQ：1575716557 邮箱：jylong06@163.com
//  QQ交流群：389037167/327403678
//



#ifndef __JSON_Parser_SCENE_H__
#define __JSON_Parser_SCENE_H__

#include "cocos2d.h"

//#include "rapidjson.h"
//rapidjson 需要
#include "json/document.h"
#include "json/writer.h"
#include "json/reader.h"
#include "json/stringbuffer.h"

USING_NS_CC;

using namespace std;

class JsonParser
{
public:
	
	static ValueMap decode(string content); 

};

#endif // __JSON_Parser_SCENE_H__
