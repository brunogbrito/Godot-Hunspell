#include "spell_checker.h"

#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/variant/utility_functions.hpp>
#include <godot_cpp/classes/file_access.hpp>
#include <godot_cpp/classes/os.hpp>
#include <godot_cpp/classes/project_settings.hpp> // Added ProjectSettings header
#include <godot_cpp/classes/dir_access.hpp>  // Added DirAccess header

// Standard library includes
#include <cstdlib> // For free()

// Include the Hunspell header here
#include "hunspell.hxx"

using namespace godot;

SpellChecker::SpellChecker() : hunspell(nullptr) {
}

SpellChecker::~SpellChecker() {
    if (hunspell) {
        delete hunspell;
        hunspell = nullptr;
    }
}

void SpellChecker::_bind_methods() {
    ClassDB::bind_method(D_METHOD("load_dictionary", "aff_file", "dic_file"), &SpellChecker::load_dictionary);
    ClassDB::bind_method(D_METHOD("is_loaded"), &SpellChecker::is_loaded);
    ClassDB::bind_method(D_METHOD("spell", "word"), &SpellChecker::spell);
    ClassDB::bind_method(D_METHOD("suggest", "word"), &SpellChecker::suggest);
    ClassDB::bind_method(D_METHOD("add_word", "word"), &SpellChecker::add_word);
    ClassDB::bind_method(D_METHOD("remove_word", "word"), &SpellChecker::remove_word);
    ClassDB::bind_method(D_METHOD("get_aff_path"), &SpellChecker::get_aff_path);
    ClassDB::bind_method(D_METHOD("get_dic_path"), &SpellChecker::get_dic_path);
}

bool SpellChecker::load_dictionary(const String &aff_file, const String &dic_file) {
    // Clean up existing instance
    if (hunspell) {
        delete hunspell;
        hunspell = nullptr;
    }

    // Get the absolute paths for the dictionary files
    String aff_abs_path;
    String dic_abs_path;
    
    // Convert res:// paths to physical file paths
    if (aff_file.begins_with("res://")) {
        UtilityFunctions::print("Converting resource path to physical path: ", aff_file);
        
        // First check if file exists through FileAccess
        Ref<FileAccess> f = FileAccess::open(aff_file, FileAccess::READ);
        if (f.is_valid()) {
            // Need to read the file contents and write to a temporary file
            Vector<uint8_t> buffer;
            buffer.resize(f->get_length());
            f->get_buffer(buffer.ptrw(), buffer.size());
            
            // Create a temporary file path
            String temp_dir = OS::get_singleton()->get_user_data_dir().path_join("hunspell_temp");
            String aff_filename = aff_file.get_file();
            
            // Ensure temp directory exists
            DirAccess::make_dir_recursive_absolute(temp_dir);
            
            // Write to temp file
            aff_abs_path = temp_dir.path_join(aff_filename);
            Ref<FileAccess> temp_file = FileAccess::open(aff_abs_path, FileAccess::WRITE);
            if (temp_file.is_valid()) {
                temp_file->store_buffer(buffer.ptr(), buffer.size());
                UtilityFunctions::print("Wrote dictionary file to: ", aff_abs_path);
            } else {
                UtilityFunctions::printerr("Failed to create temporary affix file");
                return false;
            }
        } else {
            UtilityFunctions::printerr("Could not open affix file: ", aff_file);
            return false;
        }
    } else {
        aff_abs_path = aff_file;
    }
    
    if (dic_file.begins_with("res://")) {
        UtilityFunctions::print("Converting resource path to physical path: ", dic_file);
        
        // First check if file exists through FileAccess
        Ref<FileAccess> f = FileAccess::open(dic_file, FileAccess::READ);
        if (f.is_valid()) {
            // Need to read the file contents and write to a temporary file
            Vector<uint8_t> buffer;
            buffer.resize(f->get_length());
            f->get_buffer(buffer.ptrw(), buffer.size());
            
            // Create a temporary file path
            String temp_dir = OS::get_singleton()->get_user_data_dir().path_join("hunspell_temp");
            String dic_filename = dic_file.get_file();
            
            // Ensure temp directory exists
            DirAccess::make_dir_recursive_absolute(temp_dir);
            
            // Write to temp file
            dic_abs_path = temp_dir.path_join(dic_filename);
            Ref<FileAccess> temp_file = FileAccess::open(dic_abs_path, FileAccess::WRITE);
            if (temp_file.is_valid()) {
                temp_file->store_buffer(buffer.ptr(), buffer.size());
                UtilityFunctions::print("Wrote dictionary file to: ", dic_abs_path);
            } else {
                UtilityFunctions::printerr("Failed to create temporary dictionary file");
                return false;
            }
        } else {
            UtilityFunctions::printerr("Could not open dictionary file: ", dic_file);
            return false;
        }
    } else {
        dic_abs_path = dic_file;
    }

    UtilityFunctions::print("Using affix file: ", aff_abs_path);
    UtilityFunctions::print("Using dictionary file: ", dic_abs_path);

    // Create a new Hunspell instance
    try {
        UtilityFunctions::print("Creating Hunspell instance...");
        hunspell = new Hunspell(aff_abs_path.utf8().get_data(), dic_abs_path.utf8().get_data());
        aff_path = aff_file;
        dic_path = dic_file;
        UtilityFunctions::print("Hunspell: Dictionary loaded successfully");
        
        // Test if the dictionary actually works
        UtilityFunctions::print("Testing dictionary with word 'test'");
        int result = hunspell->spell("test");
        UtilityFunctions::print("Spell check result for 'test': ", result);
        
        return true;
    } catch (std::exception &e) {
        UtilityFunctions::printerr("Hunspell: Failed to load dictionary: ", e.what());
        return false;
    } catch (...) {
        UtilityFunctions::printerr("Hunspell: Unknown error when loading dictionary");
        return false;
    }
}

bool SpellChecker::is_loaded() const {
    return hunspell != nullptr;
}

bool SpellChecker::spell(const String &word) {
    if (!hunspell) {
        UtilityFunctions::printerr("Hunspell: Dictionary not loaded");
        return false;
    }
    
    return hunspell->spell(word.utf8().get_data()) != 0;
}

PackedStringArray SpellChecker::suggest(const String &word) {
    PackedStringArray suggestions;
    
    if (!hunspell) {
        UtilityFunctions::printerr("Hunspell: Dictionary not loaded");
        return suggestions;
    }
    
    // Use the correct Hunspell suggest API
    char** suggestionList;
    int count = hunspell->suggest(&suggestionList, word.utf8().get_data());
    
    // Convert the C-style string array to PackedStringArray
    for (int i = 0; i < count; i++) {
        suggestions.push_back(String(suggestionList[i]));
        // In Hunspell, we should free these strings
        std::free(suggestionList[i]);
    }
    
    // Free the array itself
    std::free(suggestionList);
    
    return suggestions;
}

void SpellChecker::add_word(const String &word) {
    if (!hunspell) {
        UtilityFunctions::printerr("Hunspell: Dictionary not loaded");
        return;
    }
    
    hunspell->add(word.utf8().get_data());
}

void SpellChecker::remove_word(const String &word) {
    if (!hunspell) {
        UtilityFunctions::printerr("Hunspell: Dictionary not loaded");
        return;
    }
    
    hunspell->remove(word.utf8().get_data());
}

String SpellChecker::get_aff_path() const {
    return aff_path;
}

String SpellChecker::get_dic_path() const {
    return dic_path;
}
