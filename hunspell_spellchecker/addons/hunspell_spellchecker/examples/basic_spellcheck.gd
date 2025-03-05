extends Control

@onready var spellchecker = $HunspellSpellchecker
@onready var word_input = $VBoxContainer/WordInput
@onready var result_label = $VBoxContainer/ResultLabel
@onready var suggestions_container = $VBoxContainer/SuggestionsContainer
@onready var suggestions_label = $VBoxContainer/SuggestionsContainer/SuggestionsLabel
@onready var add_button = $VBoxContainer/ButtonsContainer/AddButton
@onready var status_label = $VBoxContainer/StatusLabel

func _ready():
	# Connect signals
	spellchecker.initialized.connect(_on_initialized)
	spellchecker.word_checked.connect(_on_word_checked)
	spellchecker.suggestions_generated.connect(_on_suggestions_generated)
	spellchecker.word_added.connect(_on_word_added)
	spellchecker.error_occurred.connect(_on_error_occurred)
	
	# Initialize the spellchecker if not set to auto-initialize
	if not spellchecker.auto_initialize:
		var success = spellchecker.initialize()
		if not success:
			status_label.text = "Failed to initialize spellchecker"
	
	# Focus the input field
	word_input.grab_focus()

func _on_initialized(success):
	if success:
		status_label.text = "Spellchecker initialized successfully"
	else:
		status_label.text = "Failed to initialize spellchecker"

func _on_check_button_pressed():
	var word = word_input.text.strip_edges()
	if word.is_empty():
		result_label.text = "Please enter a word"
		return
	
	# Check the word
	spellchecker.check_word(word)
	
	# Always get suggestions (they're only shown if the word is misspelled)
	spellchecker.suggest(word)

func _on_word_checked(word, is_correct):
	if is_correct:
		result_label.text = "The word '%s' is spelled correctly" % word
		result_label.modulate = Color.GREEN
		suggestions_container.visible = false
		add_button.disabled = true
	else:
		result_label.text = "The word '%s' is spelled incorrectly" % word
		result_label.modulate = Color.RED
		suggestions_container.visible = true
		add_button.disabled = false

func _on_suggestions_generated(word, suggestions):
	if suggestions.size() > 0:
		var sugg_text = "Suggestions: " + ", ".join(suggestions)
		suggestions_label.text = sugg_text
	else:
		suggestions_label.text = "No suggestions available"

func _on_add_button_pressed():
	var word = word_input.text.strip_edges()
	if word.is_empty():
		result_label.text = "Please enter a word"
		return
	
	spellchecker.add_word(word)

func _on_word_added(word):
	status_label.text = "Added '%s' to dictionary" % word
	# Recheck the word to show it's now correct
	spellchecker.check_word(word)

func _on_error_occurred(message):
	status_label.text = "Error: " + message
	status_label.modulate = Color.RED

func _on_word_input_text_submitted(new_text):
	_on_check_button_pressed()

func _on_clear_button_pressed():
	word_input.clear()
	result_label.text = ""
	suggestions_container.visible = false
	word_input.grab_focus()
