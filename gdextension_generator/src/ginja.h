#ifndef GINJA_H
#define GINJA_H

//#include <string> // I suppose it's already there ; shouldn't we have this, though?
#include <godot_cpp/core/error_macros.hpp>
#include <godot_cpp/core/binder_common.hpp>
#include <godot_cpp/classes/control.hpp>
#include <godot_cpp/classes/ref_counted.hpp>
#include <godot_cpp/classes/json.hpp>
#include <godot_cpp/variant/utility_functions.hpp>

#include "json.hpp"
#include "inja.hpp"


namespace godot {

// Note: RefCounted (formerly Reference) will be renamed in the future (as it should), 
// but I don't know if it'll make Godot 4 (I hope).  Meanwhile…
class Ginja : public RefCounted {
	GDCLASS(Ginja, RefCounted);

protected:
	/// Convention of the GdExtension API, where we'll declare the methods/consts available in Gdcript.
	static void _bind_methods();

	/// String conversion shenanigans  (UTF-8)
	std::string gs2s(const String &gstr);
	String s2gs(const std::string &str);

	/// Inja Environment the user may configure before rendering
	inja::Environment environment;
// 	inja::Environment environment { "templates/" };
// 	inja::Environment environment = inja::Environment("templates/");
// 	inja::Environment *environment_ptr = nullptr;

private:
// 	bool initialised = false;
// 	XRServer *xr_server = nullptr;
// 	double eye_height = 1.85;

public:
	// Constants.

	// Property setters and getters
    
// 	double get_eye_height() const;
// 	void set_eye_height(const double p_eye_height);
// 	double get_intraocular_dist() const;
// 	void set_intraocular_dist(const double p_intraocular_dist);


	// Functions.
    
    // I wanna keep these snippets until I grok virtual and override
// 	virtual StringName _get_name() const override;
// 	virtual bool _is_initialized() const override;
// 	virtual bool _initialize() override;
// 	virtual int64_t _get_tracking_status() const override;
// 	virtual int64_t _get_view_count() override;
// 	virtual Transform3D _get_camera_transform() override;
// 	virtual PackedFloat64Array _get_projection_for_view(int64_t p_view, double p_aspect, double p_z_near, double p_z_far) override;
// 	virtual void _commit_views(const RID &p_render_target, const Rect2 &p_screen_rect) override;
// 	virtual void _process() override;


	/// Renders the provided template using the provided variables.
	/// Main API of Ginja.
	String render(const String& string_template, const Dictionary& variables);
	
	
	/// Sets the opener and closer for template statements
	void set_statement(const String& open, const String& close);

	/// Sets the opener for template line statements
	void set_line_statement(const String& open);

	/// Sets the opener and closer for template expressions
	void set_expression(const String& open, const String& close);

	/// Sets the opener and closer for template comments
	void set_comment(const String& open, const String& close);

	/// Sets whether to remove the first newline after a block
	void set_trim_blocks(bool trim_blocks);

	/// Sets whether to strip the spaces and tabs from the start of a line to a block
	void set_lstrip_blocks(bool lstrip_blocks);
	
	/// Define a custom function
	void add_function_variadic(const String& name, const RID& object, const String& method);
// 	void add_function_variadic(const String& name, const Ref<Object> object, const String& method);
	
	void add_function(const String& name, const int args_count, const RID& object, const String& method);
 	
 	/// Inja API, but the word `callback` is poorly chosen
 	void add_callback(const String& name, const int args_count, const RID& object, const String& method);
// 	void add_callback(const String& name, const int args_count, Object* object, const String& method);


	// FuncRef is the way to go in Godot 4
	// Not even sure whether FuncRef is already in godot-cpp (can't find it)
	//void add_callback(const String& name, const int args_count, FuncRef& fn);
	
	// UNUSED: needs work upstream first
	// Sets the path where Inja will look for template files.
	// Used by `include` and `extend` statements, for example.
	void set_templates_path(const String &templates_path);

	Ginja();
	~Ginja();
};

} // namespace godot

#endif // ! GINJA_H
