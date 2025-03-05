#ifndef HUNSPELL_GDEXTENSION_H
#define HUNSPELL_GDEXTENSION_H

#include <godot_cpp/classes/ref_counted.hpp>
#include <godot_cpp/variant/packed_string_array.hpp>
#include <godot_cpp/variant/string.hpp>

// Include Hunspell header
#include "hunspell.hxx"

namespace godot {

class HunspellSpellChecker : public RefCounted {
    GDCLASS(HunspellSpellChecker, RefCounted)

private:
    Hunspell* hunspell_instance;
    String dictionary_path;
    String affixes_path;
    
    // For storing project directory path
    String project_dir;

protected:
    static void _bind_methods();

public:
    HunspellSpellChecker();
    ~HunspellSpellChecker();

    // Initialize Hunspell with dictionary and affixes files
    bool initialize(const String& dict_path, const String& aff_path);
    
    // Check if a word is spelled correctly
    bool check_word(const String& word);
    
    // Get suggestions for a misspelled word
    PackedStringArray suggest(const String& word);
    
    // Add a word to the runtime dictionary
    bool add_word(const String& word);

    // Get/set methods for properties
    String get_dictionary_path() const;
    void set_dictionary_path(const String& path);
    
    String get_affixes_path() const;
    void set_affixes_path(const String& path);
    
    // Get the project directory
    void set_project_dir(const String& dir);
};

}

#endif // HUNSPELL_GDEXTENSION_H