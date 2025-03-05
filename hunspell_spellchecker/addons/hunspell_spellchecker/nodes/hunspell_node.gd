@tool
class_name HunspellSpellchecker
extends Node

# Dictionary properties that will be exposed in the inspector
@export_file("*.dic") var dictionary_path: String = "res://addons/hunspell_spellchecker/thirdparty/dictionaries/en_US.dic":
	set(value):
		dictionary_path = value
		if Engine.is_editor_hint():
			notify_property_list_changed()

@export_file("*.aff") var affixes_path: String = "res://addons/hunspell_spellchecker/thirdparty/dictionaries/en_US.aff":
	set(value):
		affixes_path = value
		if Engine.is_editor_hint():
			notify_property_list_changed()

# Additional properties
@export var auto_initialize: bool = true
@export var case_sensitive: bool = false
@export var max_suggestions: int = 10

# Signals
signal initialized(success)
signal word_checked(word, is_correct)
signal suggestions_generated(word, suggestions)
signal word_added(word)
signal error_occurred(message)

# Internal variables
var _spellchecker = null
var _initialized: bool = false

func _ready():
	if auto_initialize and not Engine.is_editor_hint():
		initialize()

# Initialize the spellchecker with the dictionary and affixes files
func initialize() -> bool:
	if _initialized and _spellchecker:
		return true
	
	print("Creating HunspellSpellChecker instance...")
	_spellchecker = HunspellSpellChecker.new()
	
	# Extract and prepare dictionary files if needed
	var user_dict_path = _prepare_dictionary_files()
	if user_dict_path.is_empty():
		push_error("Failed to prepare dictionary files")
		emit_signal("error_occurred", "Failed to prepare dictionary files")
		emit_signal("initialized", false)
		return false
	
	print("Initializing spellchecker with:")
	print("  Dictionary: ", user_dict_path.get("dict", ""))
	print("  Affixes: ", user_dict_path.get("aff", ""))
	
	var success = _spellchecker.initialize(user_dict_path.get("dict", ""), user_dict_path.get("aff", ""))
	_initialized = success
	
	if success:
		print("Hunspell spellchecker initialized successfully!")
	else:
		push_error("Failed to initialize Hunspell spellchecker")
		emit_signal("error_occurred", "Failed to initialize spellchecker")
	
	emit_signal("initialized", success)
	return success

# Function to prepare dictionary files for use
func _prepare_dictionary_files() -> Dictionary:
	# Check if files exist in resource path
	print("Checking dictionary files existence...")
	print("Dictionary path: ", dictionary_path)
	print("Affixes path: ", affixes_path)
	
	if not FileAccess.file_exists(dictionary_path) or not FileAccess.file_exists(affixes_path):
		push_error("Dictionary or affixes file not found at specified paths")
		return {}
	
	# Copy dictionary files to user directory for proper access
	var user_data_dir = OS.get_user_data_dir()
	var dict_dir = user_data_dir.path_join("hunspell_dictionaries")
	
	# Create directory if it doesn't exist
	var dir = DirAccess.open(user_data_dir)
	if not dir or not dir.dir_exists("hunspell_dictionaries"):
		print("Creating hunspell_dictionaries directory in: ", user_data_dir)
		dir.make_dir("hunspell_dictionaries")
	
	# Extract filenames from paths
	var dict_filename = dictionary_path.get_file()
	var aff_filename = affixes_path.get_file()
	
	# User paths for dictionary files
	var user_dict_path = dict_dir.path_join(dict_filename)
	var user_aff_path = dict_dir.path_join(aff_filename)
	
	# Copy files if they don't exist or need updating
	if not FileAccess.file_exists(user_dict_path) or not FileAccess.file_exists(user_aff_path):
		print("Copying dictionary files to user directory...")
		if not _copy_file(dictionary_path, user_dict_path) or not _copy_file(affixes_path, user_aff_path):
			push_error("Failed to copy dictionary files to user directory")
			return {}
	
	print("Dictionary files prepared successfully:")
	print("  User dict path: ", user_dict_path)
	print("  User aff path: ", user_aff_path)
	
	return {
		"dict": user_dict_path,
		"aff": user_aff_path
	}

# Helper function to copy a file
func _copy_file(from_path: String, to_path: String) -> bool:
	var source = FileAccess.open(from_path, FileAccess.READ)
	if not source:
		push_error("Failed to open source file: " + from_path)
		return false
		
	var dest = FileAccess.open(to_path, FileAccess.WRITE)
	if not dest:
		push_error("Failed to open destination file: " + to_path)
		return false
	
	# Copy content
	var content = source.get_buffer(source.get_length())
	dest.store_buffer(content)
	
	source.close()
	dest.close()
	print("File copied successfully: ", from_path, " -> ", to_path)
	return true

# Check if a word is spelled correctly
func check_word(word: String) -> bool:
	if not _ensure_initialized():
		return false
	
	var check_word = word
	if not case_sensitive:
		check_word = check_word.to_lower()
	
	print("Checking word: '" + check_word + "'")
	var result = _spellchecker.check_word(check_word)
	print("Spell check result for '" + check_word + "': " + str(result))
	
	emit_signal("word_checked", word, result)
	return result

# Get suggestions for a misspelled word
func suggest(word: String) -> PackedStringArray:
	if not _ensure_initialized():
		return PackedStringArray()
	
	var check_word = word
	if not case_sensitive:
		check_word = check_word.to_lower()
	
	var suggestions = _spellchecker.suggest(check_word)
	
	# Limit the number of suggestions if needed
	if max_suggestions > 0 and suggestions.size() > max_suggestions:
		suggestions = suggestions.slice(0, max_suggestions)
	
	emit_signal("suggestions_generated", word, suggestions)
	return suggestions

# Add a word to the runtime dictionary
func add_word(word: String) -> bool:
	if not _ensure_initialized():
		return false
	
	var check_word = word
	if not case_sensitive:
		check_word = check_word.to_lower()
	
	var result = _spellchecker.add_word(check_word)
	if result:
		emit_signal("word_added", word)
	
	return result

# Helper function to ensure the spellchecker is initialized
func _ensure_initialized() -> bool:
	if not _initialized or not _spellchecker:
		var message = "Spellchecker not initialized. Call initialize() first."
		push_error(message)
		emit_signal("error_occurred", message)
		return false
	return true

# Check if the spellchecker is initialized
func is_initialized() -> bool:
	return _initialized and _spellchecker != null

# Cleanup when the node is removed
func _exit_tree():
	if _spellchecker:
		_spellchecker = null
