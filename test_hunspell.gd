extends Node

func _ready():
	print("Testing Hunspell GDExtension")
	
	# Create a SpellChecker instance
	var spell_checker = SpellChecker.new()
	print("SpellChecker instance created")
	
	# Try to load a dictionary
	var aff_path = "res://addons/hunspell/dictionaries/en_US.aff"
	var dic_path = "res://addons/hunspell/dictionaries/en_US.dic"
	
	print("Loading dictionary from:")
	print("- AFF: ", aff_path)
	print("- DIC: ", dic_path)
	
	var success = spell_checker.load_dictionary(aff_path, dic_path)
	print("Dictionary loaded: ", success)
	
	if success:
		# Test some words
		var test_words = ["hello", "world", "godot", "spellchcker", "mispeled"]
		
		for word in test_words:
			var is_correct = spell_checker.spell(word)
			print("Word '", word, "' is ", "correct" if is_correct else "incorrect")
			
			if not is_correct:
				var suggestions = spell_checker.suggest(word)
				print("Suggestions: ", suggestions)
		
		print("Hunspell test complete")
	else:
		print("Failed to load dictionary")
