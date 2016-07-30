
#include "JsonParser.h" 
#include "MyUtility.h"
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
#include <android/log.h>
#include <jni.h>
#define  LOG_TAG    "myErr"
#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)
#define  LOGD(...)  __android_log_print(ANDROID_LOG_DEBUG,LOG_TAG,__VA_ARGS__)
#define  LOGW(...)  __android_log_print(ANDROID_LOG_WARN,LOG_TAG,__VA_ARGS__)
#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
#endif


ValueMap JsonParser::decode(string content)
{
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
	LOGE("%s\n", content.c_str());
#endif

	rapidjson::Document document;
	document.Parse<0>(content.c_str());
	ValueMap dict;
	//解析错误
	CCASSERT(!document.HasParseError(), "Parsing to document failure.");

	CCLOG("Parsing to document succeeded.");

	CC_ASSERT(document.IsObject());
	CC_ASSERT(document.HasMember("ResultCode"));
	const rapidjson::Value &val_ResultCode = document["ResultCode"];

	//判断ResultCode类型 
	CC_ASSERT(val_ResultCode.IsInt());
	int resultCode = val_ResultCode.GetInt();
	dict["ResultCode"] = Value(resultCode);

	//CC_ASSERT(document.HasMember("Record"));
	//const rapidjson::Value &record = document["Record"];
	//CC_ASSERT(record.IsArray());

	//if (record.Size() == 0){
		//return dict;
	//}

	//unsigned int i = 0;

	//const rapidjson::Value &records = record[i];

	//CC_ASSERT(document.HasMember("args"));

	//const rapidjson::Value &args= document["args"]; //读取ResultCode 
	//CC_ASSERT(args.IsArray());

	//if ( args.Size() == 0) {
		//return dict;
	//}

	//unsigned int i = 0;
	//获得一条记录对象
	//const rapidjson::Value &arg = args[i]; 
	//CC_ASSERT(arg.IsObject() && arg.HasMember("ResultCode"));	
	//const rapidjson::Value &val_ResultCode= arg["ResultCode"]; //读取ResultCode 

	//判断ResultCode类型 
	//CC_ASSERT(val_ResultCode.IsInt());
	//int resultCode = val_ResultCode.GetInt(); 

	//dict["ResultCode"] = Value(resultCode);

	//判断是否有Record
	if (document.IsObject() && document.HasMember("Record")) {

		const rapidjson::Value &records= document["Record"]; //读取中文的数组 

		//判断是不是数组 
		//CC_ASSERT(records.IsArray());
		//CC_ASSERT(records.IsObject());
		if (records.Size() == 0) {
			dict["ResultCode"] = Value(-1);
			return dict;
		}
		 //if (!records.IsArray())
		else if (records.Size() == 1)
		{
			ValueMap rowone;
			const rapidjson::Value &record = records[0];

			//判断这个record有没有username键 
			CC_ASSERT(record.HasMember("username"));
			const rapidjson::Value &user = record["username"];
			std::string user_str = MyUtility::gbk_2_utf8(user.GetString());
			const char* users = user_str.c_str();
			rowone["username"] = Value(users);

			//判断这个record有没有Content键 
			CC_ASSERT(record.HasMember("password"));
			const rapidjson::Value &pass = record["password"];

			std::string pass_str = MyUtility::gbk_2_utf8(pass.GetString());
			const char* passes = pass_str.c_str();
			rowone["password"] = Value(passes);
			
			dict["Record"] = Value(rowone);

			return dict;
		}

		ValueVector listData;

		for(unsigned int i = 0; i < records.Size();i++) //如果不是数组，这一行会报错 
		{ 
			ValueMap row;

			//获得一条记录对象
			const rapidjson::Value &record = records[i]; 

			//判断这个record有没有username键 
			CC_ASSERT(record.HasMember("username"));
			const rapidjson::Value &val_CDate = record["username"]; 

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
			std::string str_date = MyUtility::gbk_2_utf8(val_CDate.GetString());
			const char* user = str_date.c_str();
			row["username"] = Value(user);
#else

			//判断CDate类型 
			CC_ASSERT(val_CDate.IsString());
			const char* user = val_CDate.GetString(); 
			//const char* user = str_date.c_str();
			row["username"] = Value(user);

#endif
			//判断这个record有没有Content键 
			CC_ASSERT(record.HasMember("password"));
			const rapidjson::Value &val_Content = record["password"]; 

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
			std::string str_content = MyUtility::gbk_2_utf8(val_Content.GetString());
			const char* pWord = str_content.c_str();
			row["password"] = Value(pWord);
#else
			
			//判断Content类型 
			CC_ASSERT(val_Content.IsString());
			const char* pWord = val_Content.GetString(); 
			row["password"] = Value(pWord);
#endif

			listData.push_back(Value(row));
		}

		dict["Record"] = Value(listData);

	}
	return dict;

}
