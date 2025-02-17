#include <Geode/Geode.hpp>
using namespace geode::prelude;
#include <Geode/modify/CommentCell.hpp>
#include <string>
#include <unordered_map>
#include <fstream>
#include <sstream>

class $modify(CommentCell) {
    std::unordered_map<std::string, std::string> loadRules(const std::string& path) {
        std::unordered_map<std::string, std::string> rules;
        std::ifstream file(path);
        if (!file) {
            geode::log::warn("Failed to fetch replacement rules, is it even set?");
            return rules;
        }
        std::string line;
        while (std::getline(file, line)) {
            std::istringstream iss(line);
            std::string key, arrow, value;
            if (iss >> key >> arrow >> value && arrow == "==>") {
                rules[key] = value;
            }
        }
        geode::log::info("Hello!");
        return rules;
    }

    std::string replace(const std::string& text, const std::unordered_map<std::string, std::string>& rules) {
        std::string modified = text;
        for (const auto& [key, value] : rules) {
            std::string::size_type pos = 0;
            while ((pos = modified.find(key, pos)) != std::string::npos) {
                modified.replace(pos, key.length(), value);
                pos += value.length();
            }
        }
        return modified;
    }

    void loadFromComment(GJComment* comment) {
        auto path = Mod::get()->getSettingValue<std::filesystem::path>("replaceconf").string();
        static auto rules = loadRules(path);
        comment->m_commentString = replace(comment->m_commentString, rules);
        CommentCell::loadFromComment(comment);
    }
};