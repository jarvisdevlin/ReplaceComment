#include <Geode/Geode.hpp>
using namespace geode::prelude;
#include <Geode/modify/CommentCell.hpp>
#include <string>
#include <unordered_map>
#include <fstream>
#include <sstream>

class $modify(CommentCell) {
    // this is where magic happens

    std::unordered_map<std::string, std::string> loadRules(const std::string& path) {
        // loads rules, i think
        std::unordered_map<std::string, std::string> rules;
        std::ifstream file(path);
        if (!file) {
            geode::log::warn("Failed to fetch replace.txt from config."); // womp womp
            return rules;
        }
        std::string line;
        while (std::getline(file, line)) {
            // surely nothing can go wrong here
            std::istringstream iss(line);
            std::string key, arrow, value;
            if (iss >> key >> arrow >> value && arrow == "==>") {
                rules[key] = value;
                geode::log::info("Loaded rule: " + key + " ==> " + value);
            }
        }
        return rules;
    }

    std::string replace(const std::string& text, const std::unordered_map<std::string, std::string>& rules) {
        // funny stuff goes here
        std::string modified = text;
        for (const auto& [key, value] : rules) {
            std::string::size_type pos = 0;
            while ((pos = modified.find(key, pos)) != std::string::npos) {
                modified.replace(pos, key.length(), value);
                pos += value.length();
                geode::log::info("Replaced " + key + " with " + value + ".");
            }
        }
        return modified;
    }

    void lol(GJComment* comment) {
        // im crying
        auto path = Mod::get()->getSettingValue<std::filesystem::path>("replaceconf").string();
        static auto rules = loadRules(path);

        // this replaces the comment, attempts to
        comment->m_commentString = replace(comment->m_commentString, rules);
        CommentCell::lol(comment);
    }
};