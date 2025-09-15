#include "StringUtiligy.h"
#include<Windows.h>
//
std::wstring StringUtiligy::GetWStringFromString(const std::string& str)
{
    //この関数を正しく使うには2回呼び出す必要がある
    std::wstring ret;
    //一回目の呼び出しは返される文字列の文字列数
    auto length = MultiByteToWideChar(CP_ACP,//現在のコードページ
                        MB_PRECOMPOSED | MB_ERR_INVALID_CHARS,//エラッたら情報を返す
                        str.c_str(),//元の文字列へのポインタ
                        str.length(),//元の文字列の長さ
                        nullptr,
                        0);//最後の引数をnullptr,0にすることで
                            //wstringに必要な文字列数を返している
    if (length <= 0) {
        return L"";
    }
    ret.resize(length);//必要なメモリを確保

    //二回目の呼び出しは実際の文字列を得る
    MultiByteToWideChar(CP_ACP,
        MB_PRECOMPOSED | MB_ERR_INVALID_CHARS,
        str.c_str(),
        str.length(),
        ret.data(),//得られたワイド文字列を入れるアドレス
        ret.size());//得られたワイド文字列を入れるメモリサイズ
    return ret;
}
