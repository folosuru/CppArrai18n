#pragma once
#ifndef CPPARRAI18N_CPPARRAI18N_HPP_
#define CPPARRAI18N_CPPARRAI18N_HPP_
#include "Arrai18n_def.hpp"
#include <fstream>
#include <unordered_map>
#include <optional>
#include <unordered_set>
#include <algorithm>
#include <utility>
#include <memory>
#include <array>

namespace Arrai18n {
namespace data {
class Node {
public:
    [[nodiscard]]
    virtual std::string format(const args_list&) const noexcept = 0;
};
class textNode : public Node {
public:
    explicit textNode(std::string text_) : text(std::move(text_)) {}
    [[nodiscard]]
    std::string format(const args_list&) const noexcept override {
        return text;
    }
    std::string text;
};
class replaceNode : public Node {
public:
    explicit replaceNode(size_t index_): index(index_) {}
    [[nodiscard]]
    std::string format(const args_list& string) const noexcept override {
        return string[index];
    }
    size_t index;
};

class text {
public:
    explicit text(std::vector<std::shared_ptr<Node>> node_) : node(std::move(node_)){}
    [[nodiscard]]
    std::string format(const args_list& args) const noexcept {
        std::string result;
        for (const auto & i :node) {
            result.append(i->format(args));
        }
        return result;
    }
    std::vector<std::shared_ptr<Node>> node;
};

typedef std::unordered_map<key_name, data::text> text_map;
class General {
private:
    General() = default;
    inline static General* instance = nullptr;
public:
    static General* getInstance() {
        if (General::instance == nullptr) {
            General::instance = new General();
        }
        return General::instance;
    }
    std::unordered_map<lang_name,std::shared_ptr<text_map>> lang_map;
    std::shared_ptr<text_map> default_lang;
};
}  // data

namespace parser {
inline std::pair<lang_name, data::text_map> parse(std::ifstream);
/**
 * get lang name
 * @param line e.g: "[ja-JP]"
 * @return lang_name e.g.: "ja-JP"
 */
inline lang_name get_langName(std::string line);

inline std::string trim(std::string);

/**
 * generate text.
 * @param text e.g.: ["hoge"]
 * @return data::text
 */
inline data::text extract_format(const std::string& text);

/**
 * parse line...
 * @param line e.g.: [a.b.c = "hogehoge"]
 * @return optional{"a.b.c", "hogehoge"}
 * @return std::nullopt (when parse failed)
 */
inline std::optional<std::pair<key_name, std::string>> parse_line(std::string line);

inline std::pair<lang_name, data::text_map> parse(std::ifstream ifstream) {
    std::string line;
    std::string lang_name;
    data::text_map result_map;
    if (!ifstream) {
        throw std::runtime_error("cannnot read file!");
    }
    while (std::getline(ifstream, line)) {
        std::string trimmed = trim(line);
        if (trimmed[0] == '[') {
            lang_name = get_langName(trimmed);
            continue;
        }
        auto able_parse = parse_line(trimmed);
        if (!able_parse) {
            continue;
        }
        auto [name, value] = able_parse.value();
        result_map.insert({name,extract_format(value)});
    }
    return {lang_name,result_map};
}
inline std::string trim(std::string str) {
    return str.erase(0, str.find_first_not_of(' '))
        .erase(str.find_last_not_of(' ') + 1, str.size() - str.find_last_not_of(' '));
}
inline std::optional<std::pair<key_name , std::string>> parse_line(std::string line) {
    if (line[0] == '#') {
        return std::nullopt;
    }
    enum class phase {
        key,
        equal,
        value
    };
    std::string key;
    std::string value;
    phase now_phase = phase::key;
    for (const auto& i : line) {
        switch (now_phase) {
            case phase::key: {
                if (i == ' ') {
                    continue;
                }
                if (i == '=') {
                    now_phase = phase::equal;
                    continue;
                }
                key += i;
                break;
            }
            case phase::equal: {
                if (i == ' ') {
                    continue;
                } else {
                    now_phase = phase::value;
                    value += i;
                }
                break;
            }
            case phase::value: {
                value += i;
            }
        }
    }
    if (now_phase != phase::value) {
        return std::nullopt;
    }
    if (key.empty()) {
        return std::nullopt;
    }
    return std::make_pair(key,value);
}
inline data::text extract_format(const std::string& text) {
    constexpr std::array<char,6> token_list{'"', '\\', '{', '}', 'n', 'r'};
    std::vector<std::shared_ptr<data::Node>> nodes;
    std::string now_buffer;
    bool escape = false;
    bool in_param = false;
    bool in_quote = false;
    for (const auto& i : text) {
        if (in_param) {
            if (i == '}') {
                nodes.push_back(std::shared_ptr<data::Node>(new data::replaceNode(std::stoi(now_buffer))));
                now_buffer.clear();
                in_param = false;
                continue;
            }
            if (i == '"') {
                throw std::runtime_error("parse error: bracket not closed");
            }
            now_buffer += i;
            continue;
        }
        if (escape) {
            if (std::find(token_list.begin(), token_list.end(), i) == token_list.end()) {
                throw std::runtime_error("parse error: invalid escape character");
            }
            if (i == 'n') {
                now_buffer += "\n";
            }
            if (i == 'r') {
                now_buffer += "\r";
            }
            now_buffer += i;
            escape = false;
            continue;
        }
        if (i == '\\') {
            escape = true;
            continue;
        }
        if (i == '{') {
            nodes.push_back(std::shared_ptr<data::Node>(new data::textNode(now_buffer)));
            now_buffer.clear();
            in_param = true;
            continue;
        }
        if (i == '"') {
            if (!in_quote) {  // 1回目のクオーテーションは無視
                in_quote = true;
                continue;
            } else {  // 2回目のクオーテーションで切り上げ
                nodes.push_back(std::shared_ptr<data::Node>(new data::textNode(now_buffer)));
                break;
            }
        }
        now_buffer += i;
    }
    return data::text(nodes);
}
inline std::string get_langName(std::string line) {
    auto text = trim(std::move(line));
    return text.substr(1,text.size()-2);
}
}
inline void load(const std::string& file) {
    auto& map = data::General::getInstance()->lang_map;
    auto lang_map = parser::parse(std::ifstream(file));
    if (map.find(lang_map.first) == map.end()) {
        data::General::getInstance()->lang_map.insert({lang_map.first, std::make_shared<data::text_map>(lang_map.second)});
    } else {
        data::General::getInstance()->lang_map.at(lang_map.first)->merge(lang_map.second);
    }
}

inline std::shared_ptr<data::text_map> getLanguageTextMap(const lang_name& lang_);

inline std::string GetText(const lang_name& lang_, const std::string& text, const std::vector<std::string>& args) {
    auto lang_text_map = getLanguageTextMap(lang_);
    if (auto result = lang_text_map->find(text); result != lang_text_map->end()) {
        return result->second.format(args);
    } else {
        return "Arrai18n: [" + text + "] is not found in language " + lang_;
    }
}

inline std::string trl(const lang_name& lang_, const std::string& text, const std::vector<std::string>& args = {}) {
    return GetText(lang_, text, args);
}
inline std::string trl(const lang_name& lang_, const trl_text& text) {
    return GetText(lang_, text.key, text.args);
}

inline std::shared_ptr<data::text_map> getLanguageTextMap(const lang_name& lang_) {
    auto& lang_map = data::General::getInstance()->lang_map;
    if (auto result = lang_map.find(lang_); result != lang_map.end()) {
        return result->second;
    } else if (auto result_default = data::General::getInstance()->default_lang; result_default) {
        return result_default;
    } else {
        throw std::runtime_error("cannot find language");
    }
}


inline void setDefaultLanguage(const lang_name& default_lang) {
    data::General::getInstance()->default_lang = data::General::getInstance()->lang_map.at(default_lang);
}

}

#endif // CPPARRAI18N_CPPARRAI18N_HPP_
