#ifndef REGISTER_TYPES_H
#define REGISTER_TYPES_H


#include <godot/gdnative_interface.h>

#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/core/defs.hpp>
#include <godot_cpp/godot.hpp>
#include <godot_cpp/variant/utility_functions.hpp>

//#include "ginja.h"


using namespace godot;

void initialize_ginja(ModuleInitializationLevel p_level);
void terminate_ginja(ModuleInitializationLevel p_level);


#endif // ! REGISTER_TYPES_H
