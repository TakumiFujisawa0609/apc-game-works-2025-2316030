#pragma once
#include<string>
class StringUtiligy
{
public:
	/// <summary>
	/// stringオブジェクト(SJIS)からwstringオブジェクトに変換する
	/// </summary>
	/// <param name="str">stringオブジェクト</param>
	/// <returns>ワイド文字列を返す</returns>
	static std::wstring GetWStringFromString(const std::string& str);
};

