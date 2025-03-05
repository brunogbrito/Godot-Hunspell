#include "hunspell_gdextension.h"
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/core/defs.hpp>
#include <godot_cpp/godot.hpp>
#include <godot_cpp/variant/utility_functions.hpp>

using namespace godot;

HunspellSpellChecker::HunspellSpellChecker() : hunspell_instance(nullptr) {
    // Initialize with default values
    project_dir = "";
}

HunspellSpellChecker::~HunspellSpellChecker() {
    // Clean up Hunspell instance
    if (hunspell_instance) {
        delete hunspell_instance;
        hunspell_instance = nullptr;
    }
}

void HunspellSpellChecker::_bind_methods() {
    // Bind methods to Godot
    ClassDB::bind_method(D_METHOD("initialize", "dict_path", "aff_path"), &HunspellSpellChecker::initialize);
    ClassDB::bind_method(D_METHOD("check_word", "word"), &HunspellSpellChecker::check_word);
    ClassDB::bind_method(D_METHOD("suggest", "word"), &HunspellSpellChecker::suggest);
    ClassDB::bind_method(D_METHOD("add_word", "word"), &HunspellSpellChecker::add_word);
    
    // Bind property getters and setters
    ClassDB::bind_method(D_METHOD("get_dictionary_path"), &HunspellSpellChecker::get_dictionary_path);
    ClassDB::bind_method(D_METHOD("set_dictionary_path", "path"), &HunspellSpellChecker::set_dictionary_path);
    ClassDB::bind_method(D_METHOD("get_affixes_path"), &HunspellSpellChecker::get_affixes_path);
    ClassDB::bind_method(D_METHOD("set_affixes_path", "path"), &HunspellSpellChecker::set_affixes_path);
    
    // Add project dir method
    ClassDB::bind_method(D_METHOD("set_project_dir", "dir"), &HunspellSpellChecker::set_project_dir);
    
    // Register properties
    ADD_PROPERTY(PropertyInfo(Variant::STRING, "dictionary_path"), "set_dictionary_path", "get_dictionary_path");
    ADD_PROPERTY(PropertyInfo(Variant::STRING, "affixes_path"), "set_affixes_path", "get_affixes_path");
}

bool HunspellSpellChecker::initialize(const String& dict_path, const String& aff_path) {
    // Clean up any existing instance
    if (hunspell_instance) {
        delete hunspell_instance;
        hunspell_instance = nullptr;
    }
    
    // Save paths
    dictionary_path = dict_path;
    affixes_path = aff_path;
    
    // Hunspell needs actual file paths, not Godot resource paths
    // First, try with the paths as provided
    std::string aff_path_str = affixes_path.utf8().get_data();
    std::string dict_path_str = dictionary_path.utf8().get_data();
    
    UtilityFunctions::print("Initializing Hunspell with:");
    UtilityFunctions::print("  Aff file: " + affixes_path);
    UtilityFunctions::print("  Dict file: " + dictionary_path);
    
    // Create new Hunspell instance
    hunspell_instance = new Hunspell(aff_path_str.c_str(), dict_path_str.c_str());
    
    // Test if the initialization worked
    if (hunspell_instance) {
        UtilityFunctions::print("Hunspell instance created");
        return true;
    } else {
        UtilityFunctions::printerr("Failed to create Hunspell instance");
        return false;
    }
}

void HunspellSpellChecker::set_project_dir(const String& dir) {
    project_dir = dir;
    UtilityFunctions::print("Project directory set to: " + project_dir);
}

bool HunspellSpellChecker::check_word(const String& word) {
    if (!hunspell_instance) {
        UtilityFunctions::printerr("Hunspell instance is null in check_word");
        return false;
    }
    
    // Convert Godot String to std::string
    std::string utf8_word = word.utf8().get_data();
    
    // Debug output
    UtilityFunctions::print("Checking word: " + word);
    
    // Check the word
    int result = hunspell_instance->spell(utf8_word.c_str());
    
    UtilityFunctions::print("Spell check result: " + String::num_int64(result));
    
    return result != 0;
}

PackedStringArray HunspellSpellChecker::suggest(const String& word) {
    PackedStringArray suggestions;
    
    if (!hunspell_instance) {
        UtilityFunctions::printerr("Hunspell instance is null in suggest");
        return suggestions;
    }
    
    // Convert Godot String to std::string
    std::string utf8_word = word.utf8().get_data();
    
    UtilityFunctions::print("Getting suggestions for: " + word);
    
    // Get suggestions from Hunspell
    char** sugglist;
    int suggcount = hunspell_instance->suggest(&sugglist, utf8_word.c_str());
    
    UtilityFunctions::print("Found " + String::num_int64(suggcount) + " suggestions");
    
    // Convert suggestions to Godot PackedStringArray
    for (int i = 0; i < suggcount; i++) {
        suggestions.push_back(String(sugglist[i]));
    }
    
    // Free suggestion list
    hunspell_instance->free_list(&sugglist, suggcount);
    
    return suggestions;
}

bool HunspellSpellChecker::add_word(const String& word) {
    if (!hunspell_instance) {
        UtilityFunctions::printerr("Hunspell instance is null in add_word");
        return false;
    }
    
    // Convert Godot String to std::string
    std::string utf8_word = word.utf8().get_data();
    
    // Add the word to the runtime dictionary
    hunspell_instance->add(utf8_word.c_str());
    
    return true;
}

String HunspellSpellChecker::get_dictionary_path() const {
    return dictionary_path;
}

void HunspellSpellChecker::set_dictionary_path(const String& path) {
    dictionary_path = path;
}

String HunspellSpellChecker::get_affixes_path() const {
    return affixes_path;
}

void HunspellSpellChecker::set_affixes_path(const String& path) {
    affixes_path = path;
}