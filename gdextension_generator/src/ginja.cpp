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

//     JSON jsonTool = new JSON(); // nope, constructor is not public
    // Not sure why this works ; are we fetching a singleton or something?
    JSON jsonTool;
    
    // First we serialize our Dictionary using Godot's JSON.
    const String gs_variables = jsonTool.stringify(variables);

    // print_line(variables);  // we'd need to import something first, but what?

    // Then we deserialize using nlohmann::json
    nlohmann::json data = nlohmann::json::parse(Ginja::gs2s(gs_variables));

    // Now we have everything we need to ask Inja to render
    std::string s_return = inja::render(s_template, data);

    //fprintf(stdout, "[ginja] Rendered:\n%s\n", ss_return.c_str());

    return Ginja::s2gs(s_return);
}


void Ginja::_bind_methods()
{
	ClassDB::bind_method(D_METHOD("render", "template", "variables"), &Ginja::render);
}


Ginja::Ginja() {}
Ginja::~Ginja() {}


} // end godot namespace
