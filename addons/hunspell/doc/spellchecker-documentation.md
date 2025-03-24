# SpellChecker Documentation

## Class Description

The `SpellChecker` class provides native spell-checking capabilities for Godot 4.4 through a GDExtension that integrates the Hunspell library. It allows for spell checking text, getting spelling suggestions for misspelled words, and managing custom dictionaries.

## Properties

| Property | Type | Description |
|----------|------|-------------|
| N/A | N/A | This class does not expose any properties directly. |

## Methods

### Dictionary Management

#### load_dictionary

```gdscript
func load_dictionary(aff_file: String, dic_file: String) -> bool
```

Loads a Hunspell dictionary for spell checking.

- **Parameters**:
  - `aff_file`: Path to the `.aff` file containing affix rules.
  - `dic_file`: Path to the `.dic` file containing the word list.
- **Returns**: `true` if the dictionary was loaded successfully, `false` otherwise.
- **Example**:
  ```gdscript
  var success = spell_checker.load_dictionary("res://addons/hunspell/dictionaries/en_US.aff", 
                                            "res://addons/hunspell/dictionaries/en_US.dic")
  ```

#### is_loaded

```gdscript
func is_loaded() -> bool
```

Checks if a dictionary is currently loaded.

- **Returns**: `true` if a dictionary is loaded, `false` otherwise.
- **Example**:
  ```gdscript
  if spell_checker.is_loaded():
      print("Dictionary is loaded and ready for use")
  ```

#### get_aff_path

```gdscript
func get_aff_path() -> String
```

Gets the path to the currently loaded `.aff` file.

- **Returns**: The path to the `.aff` file.
- **Example**:
  ```gdscript
  print("Current affix file: ", spell_checker.get_aff_path())
  ```

#### get_dic_path

```gdscript
func get_dic_path() -> String
```

Gets the path to the currently loaded `.dic` file.

- **Returns**: The path to the `.dic` file.
- **Example**:
  ```gdscript
  print("Current dictionary file: ", spell_checker.get_dic_path())
  ```

### Spell Checking

#### spell

```gdscript
func spell(word: String) -> bool
```

Checks if a word is spelled correctly according to the loaded dictionary.

- **Parameters**:
  - `word`: The word to check.
- **Returns**: `true` if the word is spelled correctly, `false` otherwise.
- **Example**:
  ```gdscript
  if spell_checker.spell("hello"):
      print("Word is spelled correctly")
  else:
      print("Word is misspelled")
  ```

#### suggest

```gdscript
func suggest(word: String) -> PackedStringArray
```

Gets spelling suggestions for a word.

- **Parameters**:
  - `word`: The word to get suggestions for.
- **Returns**: An array of suggested alternative spellings.
- **Example**:
  ```gdscript
  var suggestions = spell_checker.suggest("helo")
  for suggestion in suggestions:
      print("- ", suggestion)
  ```

### Custom Dictionary Management

#### add_word

```gdscript
func add_word(word: String) -> void
```

Adds a custom word to the dictionary.

- **Parameters**:
  - `word`: The word to add to the dictionary.
- **Example**:
  ```gdscript
  spell_checker.add_word("godot")
  ```

#### remove_word

```gdscript
func remove_word(word: String) -> void
```

Removes a word from the dictionary.

- **Parameters**:
  - `word`: The word to remove from the dictionary.
- **Example**:
  ```gdscript
  spell_checker.remove_word("godot")
  ```

## Usage Examples

### Basic Spell Checking

```gdscript
extends Node

var spell_checker

func _ready():
    # Create a SpellChecker instance
    spell_checker = SpellChecker.new()
    
    # Load a dictionary
    var success = spell_checker.load_dictionary("res://addons/hunspell/dictionaries/en_US.aff", 
                                              "res://addons/hunspell/dictionaries/en_US.dic")
    if success:
        print("Dictionary loaded successfully")
    else:
        print("Failed to load dictionary")
        return
    
    # Check some words
    check_word("hello")
    check_word("helo")
    check_word("programming")
    check_word("progamming")

func check_word(word):
    print("Checking word: ", word)
    if spell_checker.spell(word):
        print("✓ Correct: ", word)
    else:
        print("✗ Incorrect: ", word)
        var suggestions = spell_checker.suggest(word)
        if suggestions.size() > 0:
            print("  Suggestions: ", suggestions)
        else:
            print("  No suggestions available")
```

### Text Input with Spell Checking

```gdscript
extends LineEdit

var spell_checker
var misspelled_words = []

func _ready():
    # Create a SpellChecker instance
    spell_checker = SpellChecker.new()
    
    # Load a dictionary
    var success = spell_checker.load_dictionary("res://addons/hunspell/dictionaries/en_US.aff", 
                                              "res://addons/hunspell/dictionaries/en_US.dic")
    
    # Connect signals
    text_changed.connect(_on_text_changed)

func _on_text_changed(new_text):
    # Reset misspelled words
    misspelled_words.clear()
    
    # Split text into words
    var words = new_text.split(" ", false)
    
    # Check each word
    for word in words:
        # Remove any punctuation
        var clean_word = word.strip_edges()
        for ch in [",", ".", "!", "?", ":", ";"]:
            clean_word = clean_word.trim_suffix(ch)
        
        # Skip empty words
        if clean_word.length() == 0:
            continue
        
        # Check spelling
        if not spell_checker.spell(clean_word):
            misspelled_words.append(clean_word)
    
    # Update tooltip with misspelled words
    if misspelled_words.size() > 0:
        tooltip_text = "Misspelled words: " + ", ".join(misspelled_words)
        right_icon = preload("res://path/to/spell_error_icon.png")
    else:
        tooltip_text = ""
        right_icon = null
    
    # Force redraw
    queue_redraw()

func _draw():
    # Highlight misspelled words if needed
    # Note: This would require more complex logic to determine word positions
    pass
```

### Multiple Languages

```gdscript
extends Node

var spell_checker
var current_language = "en_US"
var available_languages = ["en_US", "fr_FR", "es_ES"]

func _ready():
    # Create a SpellChecker instance
    spell_checker = SpellChecker.new()
    
    # Load the default language
    change_language(current_language)

func change_language(language_code):
    var aff_path = "res://addons/hunspell/dictionaries/" + language_code + ".aff"
    var dic_path = "res://addons/hunspell/dictionaries/" + language_code + ".dic"
    
    if FileAccess.file_exists(aff_path) and FileAccess.file_exists(dic_path):
        var success = spell_checker.load_dictionary(aff_path, dic_path)
        if success:
            current_language = language_code
            print("Language changed to: ", language_code)
            return true
        else:
            print("Failed to load dictionary for: ", language_code)
            return false
    else:
        print("Dictionary files not found for: ", language_code)
        return false
```

## Dictionary Files

Hunspell uses two files for each dictionary:

1. **Affix file** (`.aff`): Contains the affix rules for the language.
2. **Dictionary file** (`.dic`): Contains the word list.

Both files must have the same base name and should be placed in the `addons/hunspell/dictionaries/` directory.

### Dictionary Sources

You can download dictionaries from the following sources:

- [LibreOffice Dictionaries](https://github.com/LibreOffice/dictionaries) - Dictionaries for many languages
- [Firefox Dictionaries](https://addons.mozilla.org/en-US/firefox/language-tools/) - Available as Firefox add-ons
- [SCOWL (And Friends)](http://wordlist.aspell.net/) - For English dictionaries

## Notes and Limitations

1. **Resource Paths**: The SpellChecker expects paths in the `res://` format, which will be automatically converted to physical file paths.

2. **Performance**: Spell checking is generally fast, but checking large amounts of text should be done efficiently to avoid performance issues.

3. **Encoding**: Hunspell dictionaries use UTF-8 encoding, which should work correctly with most languages.

4. **Custom Dictionaries**: Changes made with `add_word()` and `remove_word()` are only kept in memory and do not persist after the SpellChecker instance is destroyed.

5. **Thread Safety**: The SpellChecker is not guaranteed to be thread-safe. Use separate instances for different threads.

## Troubleshooting

### Dictionary Not Loading

If you're having trouble loading dictionaries:

1. Make sure the dictionary files (`.aff` and `.dic`) exist and are in the correct location.
2. Check the Godot console for error messages.
3. Verify that the file paths are correct and accessible.
4. Ensure dictionary files are properly formatted and encoded in UTF-8.

### Incorrect Spell Checking Results

If you're getting unexpected spell checking results:

1. Make sure you're using the appropriate dictionary for your language or region.
2. Some specialized terms may not be in standard dictionaries.
3. Consider adding specialized terms to the dictionary with `add_word()`.
