#ifndef HUNSPELL_REGISTER_TYPES_H
#define HUNSPELL_REGISTER_TYPES_H

#include <godot_cpp/core/class_db.hpp>
using namespace godot;

void initialize_hunspell_module(ModuleInitializationLevel p_level);
void uninitialize_hunspell_module(ModuleInitializationLevel p_level);

#endif // HUNSPELL_REGISTER_TYPES_H