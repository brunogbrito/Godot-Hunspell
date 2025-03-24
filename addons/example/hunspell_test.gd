extends Control

var spell_checker

@onready var dict_status = $Panel/VBox/DictStatus
@onready var word_input = $Panel/VBox/HBox/WordInput
@onready var check_button = $Panel/VBox/HBox/CheckButton
@onready var result_label = $Panel/VBox/ResultLabel
@onready var suggestions_list = $Panel/VBox/SuggestionsList
@onready var log_text = $Panel/VBox/LogText
@onready var dict_selector = $Panel/VBox/DictSelector

var available_dictionaries = []

func _ready():
	# Set up UI
	check_button.pressed.connect(_on_check_pressed)
	word_input.text_submitted.connect(_on_word_submitted)
	dict_selector.item_selected.connect(_on_dictionary_selected)
	
	# Initialize SpellChecker
	spell_checker = SpellChecker.new()
	log_message("SpellChecker instance created")
	
	# Scan for dictionaries
	scan_dictionaries()

func log_message(message):
	print(message)
	log_text.text += message + "\n"
	log_text.scroll_vertical = log_text.get_line_count()

func scan_dictionaries():
	log_message("Scanning for dictionaries...")
	
	var dir = DirAccess.open("res://addons/hunspell/dictionaries")
	if dir:
		dir.list_dir_begin()
		var file_name = dir.get_next()
		
		while file_name != "":
			if file_name.ends_with(".dic"):
				var lang_code = file_name.get_basename()
				log_message("Found dictionary: " + lang_code)
				
				available_dictionaries.append({
					"name": lang_code,
					"aff": "res://addons/hunspell/dictionaries/" + lang_code + ".aff",
					"dic": "res://addons/hunspell/dictionaries/" + lang_code + ".dic"
				})
				
				dict_selector.add_item(lang_code)
			
			file_name = dir.get_next()
		
		if available_dictionaries.size() > 0:
			dict_selector.select(0)
			load_dictionary(0)
		else:
			log_message("No dictionaries found. Place .aff and .dic files in addons/hunspell/dictionaries/")
			dict_status.text = "Status: No dictionaries found"
	else:
		log_message("ERROR: Cannot access dictionaries directory")
		dict_status.text = "Status: Cannot access dictionaries directory"

func load_dictionary(index):
	if index < 0 or index >= available_dictionaries.size():
		log_message("Invalid dictionary index")
		return
		
	var dict_info = available_dictionaries[index]
	log_message("Loading dictionary: " + dict_info.name)
	log_message("Affix file: " + dict_info.aff)
	log_message("Dictionary file: " + dict_info.dic)
	
	# Check if files exist
	if not FileAccess.file_exists(dict_info.aff):
		log_message("ERROR: Affix file not found: " + dict_info.aff)
		dict_status.text = "Status: Affix file not found"
		return
		
	if not FileAccess.file_exists(dict_info.dic):
		log_message("ERROR: Dictionary file not found: " + dict_info.dic)
		dict_status.text = "Status: Dictionary file not found"
		return
	
	# Try to load the dictionary
	var success = spell_checker.load_dictionary(dict_info.aff, dict_info.dic)
	
	if success:
		log_message("Dictionary loaded successfully")
		dict_status.text = "Status: Dictionary loaded: " + dict_info.name
		
		# Test with a few common words
		test_common_words()
	else:
		log_message("ERROR: Failed to load dictionary")
		dict_status.text = "Status: Failed to load dictionary"

func test_common_words():
	log_message("\nTesting dictionary with common words:")
	var test_words = ["the", "and", "test", "hello", "world"]
	
	for word in test_words:
		var result = spell_checker.spell(word)
		log_message("Word '" + word + "': " + ("Correct" if result else "Incorrect"))

func _on_dictionary_selected(index):
	load_dictionary(index)

func _on_check_pressed():
	check_word(word_input.text)

func _on_word_submitted(word):
	check_word(word)

func check_word(word):
	if word.strip_edges() == "":
		result_label.text = "Please enter a word"
		return
		
	if not spell_checker.is_loaded():
		result_label.text = "No dictionary loaded"
		return
	
	log_message("\nChecking word: " + word)
	var is_correct = spell_checker.spell(word)
	
	if is_correct:
		result_label.text = "Word '" + word + "' is spelled correctly"
		suggestions_list.clear()
	else:
		result_label.text = "Word '" + word + "' is misspelled"
		
		# Get suggestions
		var suggestions = spell_checker.suggest(word)
		log_message("Suggestions: " + str(suggestions))
		
		suggestions_list.clear()
		if suggestions.size() > 0:
			for suggestion in suggestions:
				suggestions_list.add_item(suggestion)
		else:
			suggestions_list.add_item("(No suggestions available)")
