#include "register_types.h"


using namespace godot;

//Ref<XRInterfaceReference> xr_interface_reference;


void initialize_ginja(ModuleInitializationLevel p_level) {
    
	if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE) {
		return;
	}
    
	ClassDB::register_class<Ginja>();

//	XRServer *xr_server = XRServer::get_singleton();
//	ERR_FAIL_NULL(xr_server);
//	xr_interface_reference.instantiate();
//	xr_server->add_interface(xr_interface_reference);
}


void terminate_ginja(ModuleInitializationLevel p_level) {

	if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE) {
		return;
	}

	// Note: our class will be unregistered automatically

	// Reference snippet
// 	if (xr_interface_reference.is_valid()) {
// 		if (xr_interface_reference->is_initialized()) {
// 			xr_interface_reference->uninitialize();
// 		}
// 		XRServer *xr_server = XRServer::get_singleton();
// 		ERR_FAIL_NULL(xr_server);
// 		xr_server->remove_interface(xr_interface_reference);
// 		xr_interface_reference.unref();
// 	}

}


extern "C" {

// Subscribe/Register our extension with Godot.
// See godot-cpp repository doc.  (when it'll be up to date)
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
