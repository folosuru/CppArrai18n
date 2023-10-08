#include <iostream>
#include <string>
#include "Arrai18n.hpp"

void make_LangFile();

int main() {
    make_LangFile();

    Arrai18n::load("ja-jp.txt");
    Arrai18n::load("en-us.txt");
    std::cout << Arrai18n::trl("main.saved","ja-JP",{"virus.exe", "iexplore.exe"}) << std::endl;
    std::cout << Arrai18n::trl("main.saved","en-US",{"virus.exe", "iexplore.exe"}) << std::endl;
    return 0;
}


void make_LangFile() {
std::ofstream("ja-jp.txt") <<
R"([ja-JP]
main.open = "{0}を開く"
main.saved = "{0}を{1}として保存しました。"
)" << std::endl;
std::ofstream("en-us.txt") <<
R"([en-US]
main.open = "Open {0}"
main.saved = "Saved {0} as {1}."
)" << std::endl;
}