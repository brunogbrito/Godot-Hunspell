# Hunspell Dictionaries

Place Hunspell dictionary files (.aff and .dic) in this directory.

## Where to Find Dictionaries

You can download dictionaries from the following sources:

1. [LibreOffice Dictionaries](https://github.com/LibreOffice/dictionaries) - Dictionaries for many languages
2. [Firefox Dictionaries](https://addons.mozilla.org/en-US/firefox/language-tools/) - Available as Firefox add-ons
3. [SCOWL (And Friends)](http://wordlist.aspell.net/) - For English dictionaries

## Dictionary Format

Each language requires two files:
- `.aff` file: Contains affix rules for the language
- `.dic` file: Contains the word list

Both files must have the same base name, for example:
- `en_US.aff`
- `en_US.dic`

## Adding Dictionaries

1. Download the `.aff` and `.dic` files for your desired language
2. Place them in this directory
3. Restart your Godot project or reload the dictionaries

## Important: Exporting Your Project

When exporting your project, you must include these dictionary files by adding them to your export filters:

1. Go to Project → Export → Resources → Filters to export non-resource files/folders
2. Add the following pattern:
   ```
   addons/hunspell/dictionaries/*.aff,addons/hunspell/dictionaries/*.dic
   ```

Without this step, your exported game will not have access to the dictionaries, and spell checking will not work.

## Common Dictionary Names

Here are some common dictionary file names:

- `en_US.aff` / `en_US.dic` - English (United States)
- `en_GB.aff` / `en_GB.dic` - English (United Kingdom)
- `fr_FR.aff` / `fr_FR.dic` - French
- `es_ES.aff` / `es_ES.dic` - Spanish
- `de_DE.aff` / `de_DE.dic` - German
- `it_IT.aff` / `it_IT.dic` - Italian
- `pt_BR.aff` / `pt_BR.dic` - Portuguese (Brazil)
- `ru_RU.aff` / `ru_RU.dic` - Russian
- `ja_JP.aff` / `ja_JP.dic` - Japanese
- `zh_CN.aff` / `zh_CN.dic` - Chinese (Simplified)

## License Notice

Please be aware that dictionaries may have their own licenses. Make sure to check the license of the dictionaries you use and comply with their terms.
