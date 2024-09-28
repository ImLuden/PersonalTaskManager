#include<iostream>
#include<fmt/core.h>
#include <codecvt> // codecvt_utf8
#include <locale>  // wstring_convert


// encoding function (json test)
std::string to_utf8(std::wstring& wide_string)
{
    static std::wstring_convert<std::codecvt_utf8<wchar_t>> utf8_conv;
    return utf8_conv.to_bytes(wide_string);
}

int main()
{


    json j;
    std::wstring ws = L"車B1234 こんにちは";

    j["original"] = ws;
    j["encoded"] = to_utf8(ws);

    std::cout << j << std::endl;
    std::cout << "Hello, World" << std::endl;
    fmt::print("Fmt test");
    return 0;
}