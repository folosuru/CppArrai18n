#pragma once
#ifndef CPPARRAI18N_CPPARRAI18N_HPP_
#define CPPARRAI18N_CPPARRAI18N_HPP_
#include <fstream>
#include <string>
#include <unordered_map>
#include <optional>
#include <unordered_set>
#include <array>
#include <algorithm>
#include <utility>
#include <string>
#include <vector>
#include <memory>
namespace Arrai18n {
typedef std::string lang_name;
typedef std::string key_name;
typedef std::vector<std::string> args_list;
std::string trl(const std::string& text, const lang_name& lang_, const std::vector<std::string>&);
void load(const std::string&);
namespace data {
class Node {
public:
    [[nodiscard]]
    virtual std::string format(args_list) = 0;
    virtual std::string getRaw() = 0;
};
class textNode : public Node {
public:
    explicit textNode(std::string text_) : text(std::move(text_)) {}
    [[nodiscard]]
    std::string format(args_list) override {
        return text;
    }
    std::string getRaw() override {
        return text;
    }
    std::string text;
};
class replaceNode : public Node {
public:
    explicit replaceNode(size_t index_): index(index_) {}
    [[nodiscard]]
    std::string format(args_list string) override {
        return string[index];
    }
    std::string getRaw() override {
        return std::to_string(index);
    }
    size_t index;
};
class text {
public:
    explicit text(std::vector<std::shared_ptr<Node>> node_) : node(std::move(node_)){}
    std::string format(const args_list& args) {
        std::string result;
        for (const auto & i :node) {
            result.append(i->format(args));
        }
        return result;
    }
    std::vector<std::shared_ptr<Node>> node;
};
}
typedef std::unordered_map<key_name, data::text> text_map;
class General {
private:
    General() = default;
    static General* instance;
public:
    static General* getInstance() {
        if (General::instance == nullptr) {
            General::instance = new General();
        }
        return General::instance;
    }
    std::unordered_map<lang_name,text_map> lang_map;
};
General* General::instance = nullptr;

namespace parser {
std::pair<lang_name, text_map> parse(std::ifstream);
/**
 * get lang name
 * @param line e.g: "[ja-JP]"
 * @return lang_name e.g.: "ja-JP"
 */
lang_name get_langName(std::string line);

std::string trim(std::string);

/**
 * generate text.
 * @param text e.g.: ["hoge"]
 * @return data::text
 */
data::text extract_format(const std::string& text);

/**
 * parse line...
 * @param line e.g.: [a.b.c = "hogehoge"]
 * @return optional{"a.b.c", "hogehoge"}
 * @return std::nullopt (when parse failed)
 */
std::optional<std::pair<key_name, std::string>> parse_line(std::string line);


std::pair<lang_name, text_map> parse(std::ifstream ifstream) {
    std::string line;
    std::string lang_name;
    text_map result_map;
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
std::string trim(std::string str) {
    return str.erase(0, str.find_first_not_of(' '))
        .erase(str.find_last_not_of(' ') + 1, str.size() - str.find_last_not_of(' '));
}
std::optional<std::pair<key_name , std::string>> parse_line(std::string line) {
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
data::text extract_format(const std::string& text) {
    constexpr std::array<char,4> token_list{'"', '\\', '{', '}'};
    std::vector<std::shared_ptr<data::Node>> nodes;
    std::string now_buffer;
    bool escape = false;
    bool in_param = false;
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
            if (!now_buffer.empty()) {
                nodes.push_back(std::shared_ptr<data::Node>(new data::textNode(now_buffer)));
                now_buffer.clear();
            }
            continue;
        }
        now_buffer += i;
    }
    return data::text(nodes);
}
std::string get_langName(std::string line) {
    auto text = trim(std::move(line));
    return text.substr(1,text.size()-2);
}
}
void load(const std::string& file) {
    General::getInstance()->lang_map.insert(parser::parse(std::ifstream(file)));
}
std::string trl(const std::string& text, const lang_name& lang_, const std::vector<std::string>& args) {
    return General::getInstance()->lang_map.at(lang_).at(text).format(args);
}
}

#endif // CPPARRAI18N_CPPARRAI18N_HPP_
