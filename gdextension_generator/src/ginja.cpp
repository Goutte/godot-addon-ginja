#include "ginja.h"

namespace godot {


std::string Ginja::gs2s(const String& gstr)
{
	return gstr.utf8().get_data();
}


String Ginja::s2gs(const std::string& str)
{
	return String::utf8(str.c_str());
}


// Remember: `template` is a reserved word
String Ginja::render(const String& string_template, const Dictionary& variables)
{

	const std::string s_template = Ginja::gs2s(string_template);

	// To pivot our Dictionary of Variants to a nlohmann::json object,
	// we're using serialization and then deserialization. It's cheap.
	// There may be drawbacks, though. Slower, bigger memory footprint, type loss, etc.
	// This has NOT been extensively tested. Contributions are welcome :3

	// Is C++ handling the malloc()/free() for us?  Looks like it.  Neat!
	// NOPE: our leaked instances come from here.  Something is wrong.
	JSON jsonTool;
	//JSON jsonTool2; // yup, doubles the leak
	//JSON* jsonTool3; // nah, crashes
	
	// This ain't the same ID than the leaks, WTF is happening
	//UtilityFunctions::print(jsonTool.get_instance_id());
	
	// First we serialize our Dictionary using Godot's JSON.
	const String gs_variables = jsonTool.stringify(variables);
	//const String gs_variables = jsonTool3->stringify(variables);
	
	//free(jsonTool); // nope
	//jsonTool = NULL; // nope, not a pointer
	//jsonTool.unreference(); // does not crash, but what am i doing?

	// Input template path is ignored when parsing strings, see
	// https://github.com/pantor/inja/issues/193
	//inja::Environment environment_yolo { "./templates/", "/tmp/" };

	try {
		// Then we deserialize using nlohmann::json
		const nlohmann::json data = nlohmann::json::parse(
			Ginja::gs2s(gs_variables)
		);

		// Now we have everything we need to ask Inja to render
		std::string s_return = environment.render(s_template, data);
    
		// FIXME: don't we need to free() `data` ?
        
		//fprintf(stdout, "[ginja] Rendered:\n%s\n", s_return.c_str());

		return Ginja::s2gs(s_return);

	} catch (inja::InjaError error) {
		//fprintf(stderr, "[ginja] Failed to render:\n%s\n%s\n", s_template.c_str(), error.message.c_str());

		char message [2048]; // ¡Bad!  If Inja decides to be more verbose, we'll overflow and crash.
		sprintf(message, "Ginja failed to render(): %s", error.message.c_str());
		ERR_PRINT(message);
		
		// FIXME: don't we need to free() `message`?

		return String(""); // or perhaps the error message?
		
	} catch (nlohmann::detail::exception e) {
		ERR_PRINT("Ginja failed to parse the render() variables");
		
		return String("");
	}

}


/// Sets the opener and closer for template statements
void Ginja::set_statement(const String& open, const String& close)
{
	environment.set_statement(Ginja::gs2s(open), Ginja::gs2s(close));
}

/// Sets the opener for template line statements
void Ginja::set_line_statement(const String& open)
{
	environment.set_line_statement(Ginja::gs2s(open));
}

/// Sets the opener and closer for template expressions
void Ginja::set_expression(const String& open, const String& close)
{
	environment.set_expression(Ginja::gs2s(open), Ginja::gs2s(close));
}

/// Sets the opener and closer for template comments
void Ginja::set_comment(const String& open, const String& close)
{
	environment.set_comment(Ginja::gs2s(open), Ginja::gs2s(close));
}

/// Sets whether to remove the first newline after a block
void Ginja::set_trim_blocks(bool trim_blocks)
{
	environment.set_trim_blocks(trim_blocks);
}

/// Sets whether to strip the spaces and tabs from the start of a line to a block
void Ginja::set_lstrip_blocks(bool lstrip_blocks)
{
	environment.set_lstrip_blocks(lstrip_blocks);
}

/// Adds a function supporting a variable amount of arguments.
void Ginja::add_function_variadic(const String& name, const RID& object, const String& method)
{
	//Ginja::add_callback(name, 0, object, method); // meh : fix inja upstream
	Ginja::add_callback(name, -1, object, method);
}

/// Adds a function with a specific amount of arguments.
void Ginja::add_function(const String& name, const int args_count, const RID& object, const String& method)
{
	Ginja::add_callback(name, args_count, object, method);
}


// FuncRef appears absent from godot-cpp right now ; try again later
// void Ginja::add_callback(const String& name, const int args_count, FuncRef& fn)


/// `callback` is awkward, ambiguous.  But it's Inja's API, so we want to mirror it.  Prefer using add_function() and add_function_variadic()
/// 
void Ginja::add_callback(const String& name, const int args_count, const RID& object, const String& method)
{
	ERR_FAIL_NULL_MSG(object, "Callback object may not be null.  We need to call a method on it.");
	// Nope: ambiguous overload for ‘operator==’ (operand types are ‘const godot::String’ and ‘std::nullptr_t’)
// 	ERR_FAIL_NULL_MSG(method, "Callback method may not be null.  We need to call it.");
    
	// TODO: sanitize method with regex ^_*[a-zA-Z][_a-zA-Z0-9]*$
	const std::string s_name = Ginja::gs2s(name);

	environment.add_callback(s_name, args_count, [object, method, args_count](inja::Arguments& args) {
		
		// Triggers (but only once, and object_instance is returned anyway):
		// ERROR: Condition "_instance_bindings != nullptr" is true. at: set_instance_binding (core/object/object.cpp:1771)
		Object* object_instance = ObjectDB::get_instance(object.get_id());
		
		//UtilityFunctions::print(object_instance);
		
		auto actual_args_count = args.size();
		auto gd_args = Array();
		for (long i = 0 ; i < actual_args_count ; i++) {
			auto argument = args.at(i);
			// Instead of this fastidious type conversion,
			// we could try through JSON instead here as well.
			auto handled = false;
			if (argument->is_primitive()) {
				if (argument->is_null()) {
					gd_args.append(Variant());
					handled = true;
				}
				else if (argument->is_boolean()) {
					gd_args.append(argument->get<bool>());
					handled = true;
				}
				else if (argument->is_string()) {
					gd_args.append(String::utf8(argument->get<std::string>().c_str()));
					handled = true;
				}
				else if (argument->is_number_integer()) {
					gd_args.append(argument->get<int64_t>());
					handled = true;
				}
				else if (argument->is_number_float()) {
					gd_args.append(argument->get<double>());
					handled = true;
				}
// 				@ref is_binary() -- returns whether JSON value is a binary array
			} // TODO: else if is_structured() => is_object, is_array
			
			if ( ! handled) {
				UtilityFunctions::printerr("[Ginja] Unsupported type for function argument.");
				gd_args.append(Variant());
			}
		}
		
		Variant returned;
		if (-1 == args_count) {
			returned = object_instance->call(method, gd_args);
		} else {
			returned = object_instance->callv(method, gd_args);
		}
		
// 		UtilityFunctions::print("object->call(method, args) yields:");
// 		UtilityFunctions::print(returned);
		
		return returned.stringify().utf8().get_data();
// 		return String(returned.stringify()).utf8().get_data();
	});
}

void Ginja::set_templates_path(const String &templates_path)
{
	// We need to work upstream in Inja with the input_path first.
	// - https://github.com/pantor/inja/issues/193
	// - add a freakin' setter for input_path or figure out how to write a new environment to Ginja#environment
	ERR_PRINT("Ginja::set_templates_path() is not implemented.");
}

void Ginja::_bind_methods()
{
	ClassDB::bind_method(D_METHOD("render", "template", "variables"), &Ginja::render);
	ClassDB::bind_method(D_METHOD("set_statement", "open", "close"), &Ginja::set_statement);
	ClassDB::bind_method(D_METHOD("set_line_statement", "open"), &Ginja::set_line_statement);
	ClassDB::bind_method(D_METHOD("set_expression", "open", "close"), &Ginja::set_expression);
	ClassDB::bind_method(D_METHOD("set_comment", "open", "close"), &Ginja::set_comment);
	ClassDB::bind_method(D_METHOD("set_trim_blocks", "trim_blocks"), &Ginja::set_trim_blocks);
	ClassDB::bind_method(D_METHOD("set_lstrip_blocks", "lstrip_blocks"), &Ginja::set_lstrip_blocks);
	ClassDB::bind_method(D_METHOD("add_callback", "name", "args_count", "object", "method"), &Ginja::add_callback);
	ClassDB::bind_method(D_METHOD("add_function", "name", "args_count", "object", "method"), &Ginja::add_function);
	ClassDB::bind_method(D_METHOD("add_function_variadic", "name", "object", "method"), &Ginja::add_function_variadic);
	// WANTED BUT NOPE ; See https://github.com/pantor/inja/issues/193
	//ClassDB::bind_method(D_METHOD("set_templates_path", "templates_path"), &Ginja::set_templates_path);
}


Ginja::Ginja() {}

/* best not use constructor params, Godot does not like it
Ginja::Ginja(const String &templates_directory)
// : environment(Ginja::gs2s(templates_directory))
: environment("res://templates/")
{
	if (environment_ptr == nullptr) {
		environment_ptr = &environment;
	}
}
*/

Ginja::~Ginja() {}


} // end godot namespace
