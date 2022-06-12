#include "ginja.h"

namespace godot {


std::string Ginja::gs2s(const String &gstr)
{
	return gstr.utf8().get_data();
}


String Ginja::s2gs(const std::string &str)
{
	return String::utf8(str.c_str());
}


// Remember: `template` is a reserved word
String Ginja::render(const String &string_template, const Dictionary &variables)
{

	const std::string s_template = Ginja::gs2s(string_template);

	// To pivot our Dictionary of Variants to a nlohmann::json object,
	// we're using serialization and then deserialization. It's cheap.
	// There may be drawbacks, though. Slower, bigger memory footprint, type loss, etc.
	// This has NOT been extensively tested. Contributions are welcome :3

	// Is C++ handling the malloc()/free() for us?  Looks like it.  Neat!
	JSON jsonTool;
	
	// First we serialize our Dictionary using Godot's JSON.
	const String gs_variables = jsonTool.stringify(variables);

	// print_line(variables);  // we'd need to import something first, but what?

	// Then we deserialize using nlohmann::json
	const nlohmann::json data = nlohmann::json::parse(Ginja::gs2s(gs_variables));


	try {

		// Input template path is ignored when parsing strings, see
		// https://github.com/pantor/inja/issues/193
		//inja::Environment environment_yolo { "./templates/", "/tmp/" };
		
		// Now we have everything we need to ask Inja to render
		std::string s_return = environment.render(s_template, data);
    
		// The old, quick way ; now we want to customize env
		//std::string s_return = inja::render(s_template, data);
        
		//fprintf(stdout, "[ginja] Rendered:\n%s\n", s_return.c_str());

		return Ginja::s2gs(s_return);

	} catch (inja::FileError error) {
		//fprintf(stderr, "[ginja] Failed to render:\n%s\n%s\n", s_template.c_str(), error.message.c_str());

		char message [2048]; // ¡Bad!  If Inja decides to be more verbose, we'll overflow and crash.
		sprintf(message, "Ginja failed to render(): %s", error.message.c_str());
		ERR_PRINT(message);

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
