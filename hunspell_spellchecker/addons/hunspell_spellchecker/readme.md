# Hunspell Spellchecker Addon for Godot

This addon adds spellchecking capabilities to your Godot projects using the Hunspell library.

## Features

- Spell checking for multiple languages
- Word suggestions for misspelled words
- Custom dictionary support
- Easy integration with UI elements
- Signal-based workflow for reactive interfaces

## Installation

1. Download the addon from the Godot Asset Library
2. In your Godot project, go to AssetLib tab and search for "Hunspell Spellchecker"
3. Download and install the addon
4. Enable the addon in Project Settings > Plugins

### Manual Installation

1. Download or clone this repository
2. Copy the `addons/hunspell_spellchecker` folder to your project's `addons` folder
3. Enable the addon in Project Settings > Plugins

## Usage

### Basic Usage

1. Add a `HunspellSpellchecker` node to your scene
2. Set the dictionary and affixes paths in the Inspector
3. Call the node's methods in your scripts:

```gdscript
# Check if a word is spelled correctly
if $HunspellSpellchecker.check_word("hello"):
    print("Correct spelling!")
else:
    print("Misspelled word!")
    
    # Get suggestions
    var suggestions = $HunspellSpellchecker.suggest("hello")
    print("Suggestions: ", suggestions)
```

### Node Properties

| Property | Description |
|----------|-------------|
| dictionary_path | Path to the .dic dictionary file |
| affixes_path | Path to the .aff affixes file |
| auto_initialize | Whether to initialize on _ready |
| case_sensitive | Whether spelling checks are case-sensitive |
| max_suggestions | Maximum number of suggestions to return |

### Available Methods

- `initialize()` - Initialize the spellchecker
- `check_word(word)` - Check if a word is spelled correctly
- `suggest(word)` - Get suggestions for a misspelled word
- `add_word(word)` - Add a word to the runtime dictionary
- `is_initialized()` - Check if the spellchecker is initialized

### Signals

- `initialized(success)` - Emitted when the spellchecker is initialized
- `word_checked(word, is_correct)` - Emitted when a word is checked
- `suggestions_generated(word, suggestions)` - Emitted when suggestions are generated
- `word_added(word)` - Emitted when a word is added to the dictionary
- `error_occurred(message)` - Emitted when an error occurs

## Dictionaries

The addon comes with English (US) dictionary by default. Additional dictionaries can be added to the `addons/hunspell_spellchecker/thirdparty/dictionaries` folder.

Dictionaries consist of two files:
- `.dic` file - Dictionary file
- `.aff` file - Affixes file

You can find dictionaries for many languages at:
- [OpenOffice Extensions](https://extensions.openoffice.org/en/search?f%5B0%5D=field_project_tags%3A157)
- [LibreOffice Extensions](https://extensions.libreoffice.org/en/extensions?getCategories=Dictionary)

## Example

See the example scene in `addons/hunspell_spellchecker/examples/basic_spellcheck.tscn` for a simple implementation.

## License

This addon is licensed under the MIT License. See the LICENSE file for details.

The Hunspell library is licensed under a combination of GPL/LGPL/MPL licenses.
