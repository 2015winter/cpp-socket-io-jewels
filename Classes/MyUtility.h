//
//  Created by 关东升 on 2014-5-18.
//  本书网站：http://www.cocoagame.net
//  智捷iOS课堂在线课堂：http://v.51work6.com
//  智捷iOS课堂新浪微博：http://weibo.com/u/3215753973
//  作者微博：http://weibo.com/516inc
//  QQ：1575716557 邮箱：jylong06@163.com
//  QQ交流群：389037167/327403678
//


#ifndef  _MY_UTILITY_H_
#define  _MY_UTILITY_H_

#include "cocos2d.h"

using namespace std;

class MyUtility 
{
public:
	static string getCurrentTime();

	static string gbk_2_utf8(const string text);
};

#endif // _MY_UTILITY_H_
