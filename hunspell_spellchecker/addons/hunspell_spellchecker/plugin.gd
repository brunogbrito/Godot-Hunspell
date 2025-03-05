@tool
extends EditorPlugin

func _enter_tree():
    # Register the HunspellSpellchecker node
    add_custom_type("HunspellSpellchecker", "Node", 
                   preload("res://addons/hunspell_spellchecker/nodes/hunspell_node.gd"), 
                   preload("res://addons/hunspell_spellchecker/icon.svg"))

func _exit_tree():
    # Clean up when the plugin is disabled
    remove_custom_type("HunspellSpellchecker")