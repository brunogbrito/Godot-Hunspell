#ifndef SPELL_CHECKER_H
#define SPELL_CHECKER_H

#include <godot_cpp/classes/ref_counted.hpp>
#include <godot_cpp/variant/packed_string_array.hpp>
#include <godot_cpp/variant/string.hpp>

// Forward declaration to avoid including the full Hunspell header
class Hunspell;

namespace godot {

class SpellChecker : public RefCounted {
    GDCLASS(SpellChecker, RefCounted)

private:
    Hunspell* hunspell;
    String aff_path;
    String dic_path;

protected:
    static void _bind_methods();

public:
    SpellChecker();
    ~SpellChecker();

    // Dictionary management
    bool load_dictionary(const String &aff_file, const String &dic_file);
    bool is_loaded() const;
    
    // Spell checking
    bool spell(const String &word);
    PackedStringArray suggest(const String &word);
    
    // Custom dictionary management
    void add_word(const String &word);
    void remove_word(const String &word);
    
    // Get dictionary paths
    String get_aff_path() const;
    String get_dic_path() const;
};

}

#endif // SPELL_CHECKER_H
