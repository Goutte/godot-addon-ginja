#include "register_types.h"

/*
#include <godot/gdnative_interface.h>

#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/core/defs.hpp>
#include <godot_cpp/godot.hpp>
#include <godot_cpp/variant/utility_functions.hpp>
//#include <godot_cpp/classes/xr_server.hpp>

#include "ginja.h"
//#include "xr_interface_reference.h"
*/


using namespace godot;

//Ref<XRInterfaceReference> xr_interface_reference;

void initialize_ginja(ModuleInitializationLevel p_level) {
    
	if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE) {
		return;
	}
    
	//ClassDB::register_class<Ginja>();

//	XRServer *xr_server = XRServer::get_singleton();
//	ERR_FAIL_NULL(xr_server);
//	xr_interface_reference.instantiate();
//	xr_server->add_interface(xr_interface_reference);
}

void terminate_ginja(ModuleInitializationLevel p_level) {
    
	if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE) {
		return;
	}
    
// 	if (xr_interface_reference.is_valid()) {
// 		if (xr_interface_reference->is_initialized()) {
// 			xr_interface_reference->uninitialize();
// 		}
// 
// 		XRServer *xr_server = XRServer::get_singleton();
// 		ERR_FAIL_NULL(xr_server);
// 		xr_server->remove_interface(xr_interface_reference);
// 
// 		xr_interface_reference.unref();
// 	}

	// Note: our class will be unregistered automatically
}

extern "C" {

// Initialization.

//GDNativeBool GDN_EXPORT inja_library_init(
GDNativeBool GDN_EXPORT initialize_ginja_extension(
	const GDNativeInterface *p_interface,
	const GDNativeExtensionClassLibraryPtr p_library,
	GDNativeInitialization *r_initialization
) {
	godot::GDExtensionBinding::InitObject init_obj(p_interface, p_library, r_initialization);

	init_obj.register_initializer(initialize_ginja);
	init_obj.register_terminator(terminate_ginja);
    init_obj.set_minimum_library_initialization_level(MODULE_INITIALIZATION_LEVEL_SCENE);

	return init_obj.init();
}

}
