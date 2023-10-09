#include <iostream>
#include <string>
#include "Arrai18n.hpp"

void make_LangFile();

int main() {
    make_LangFile();

    Arrai18n::load("ja-jp.txt");
    Arrai18n::load("en-us.txt");

    // trl("language code", "text mame", std::vector<std::string>{"arg1", "arg2", ...})
    std::cout << Arrai18n::trl("ja-JP", "main.open", {"index.html"}) << std::endl;
    std::cout << Arrai18n::trl("en-US", "main.open", {"index.html"}) << std::endl;
    std::cout << std::endl;

    // trl_text{"text name", std::vector<std::string>{"arg1", "arg2", ...}}
    Arrai18n::trl_text text = {"main.saved", {"cat.png", "very_cute_cat.png"}};
    // trl("language code", trl_text);
    std::cout << Arrai18n::trl("ja-JP", text) << std::endl;
    std::cout << Arrai18n::trl("en-US", text) << std::endl;
    return 0;
}

/**
 * make language file at current directory.
 */
void make_LangFile() {
std::ofstream("ja-jp.txt") <<
R"([ja-JP]
main.open = "\"{0}\"を開く"
main.saved = "{0}を{1}として保存しました。"
)" << std::endl;

std::ofstream("en-us.txt") <<
R"([en-US]
main.open = "Open \"{0}\""
main.saved = "Saved {0} as {1}."
)" << std::endl;
}
