#ifndef HUNSPELL_REGISTER_TYPES_H
#define HUNSPELL_REGISTER_TYPES_H

#include <godot_cpp/core/class_db.hpp>

using namespace godot;

void initialize_hunspell_module(ModuleInitializationLevel p_level);
void uninitialize_hunspell_module(ModuleInitializationLevel p_level);

extern "C" {
    // The name of this function is referenced in the .gdextension file
    GDExtensionBool GDE_EXPORT hunspell_library_init(GDExtensionInterfaceGetProcAddress p_get_proc_address, GDExtensionClassLibraryPtr p_library, GDExtensionInitialization *r_initialization);
}

#endif // HUNSPELL_REGISTER_TYPES_H
