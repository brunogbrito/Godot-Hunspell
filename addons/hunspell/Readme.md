# Hunspell GDExtension for Godot 4.4

A native spell-checking solution for Godot 4.4 using the [Hunspell](https://github.com/hunspell/hunspell) library.

## Features

- Fast, native spell-checking through Godot's GDExtension system
- Support for multiple languages through standard Hunspell dictionaries
- Easy-to-use API for spell checking and getting suggestions
- Support for custom dictionaries (adding/removing words)

## Installation

### Method 1: Godot Asset Library

1. Open your Godot project
2. Navigate to the AssetLib tab
3. Search for "Hunspell Spellchecker"
4. Download and install the addon

### Method 2: Manual Installation

1. Download the latest release from the [GitHub repository](https://github.com/brunogbrito/Godot-Hunspell)
2. Extract the contents into your project's `addons/` directory
3. Download Hunspell dictionaries for your desired languages and place the `.aff` and `.dic` files in the `dictionaries` folder

## Setup

1. Create a `dictionaries` folder inside the `addons/hunspell/` directory if it doesn't exist already
2. Download Hunspell dictionaries for your desired languages and place the `.aff` and `.dic` files in the `dictionaries` folder
   - Dictionary files can be found from [LibreOffice dictionaries](https://github.com/LibreOffice/dictionaries) or other sources
   - Each language typically requires two files: a `.aff` file and a `.dic` file with the same base name (e.g., `en_US.aff` and `en_US.dic`)

## Basic Usage

```gdscript
# Create a SpellChecker instance
var spell_checker = SpellChecker.new()

# Load a dictionary
var success = spell_checker.load_dictionary("res://addons/hunspell/dictionaries/en_US.aff", 
                                           "res://addons/hunspell/dictionaries/en_US.dic")
if success:
    # Check if a word is spelled correctly
    if spell_checker.spell("hello"):
        print("Word is spelled correctly")
    else:
        print("Word is misspelled")
        
        # Get suggestions for misspelled words
        var suggestions = spell_checker.suggest("hello")
        print("Suggestions: ", suggestions)
        
    # Add a custom word to the dictionary
    spell_checker.add_word("godot")
```

## Example Project

The addon includes a simple test scene (`hunspell_test.tscn`) that demonstrates how to use the SpellChecker class. You can use this as a reference for implementing spell checking in your own projects.

## License

This addon is distributed under the MIT license. See the [LICENSE](https://github.com/brunogbrito/Godot-Hunspell?tab=MIT-1-ov-file) file for more information.

The Hunspell library is distributed under a combination of licenses (GPL/LGPL/MPL). See the [Hunspell license](https://github.com/hunspell/hunspell/blob/master/license.hunspell) for more information.

## Credits

- [Hunspell](https://github.com/hunspell/hunspell) - The spell checking library
- [godot-cpp](https://github.com/godotengine/godot-cpp) - C++ bindings for the Godot API

## Support

If you encounter any issues or have questions, please file an issue on the [GitHub repository](https://github.com/brunogbrito/Godot-Hunspell/issues).
