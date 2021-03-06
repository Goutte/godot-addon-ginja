
**Ginja** is a **string template engine** for **Godot 4**.

Provides access to (some of) [`Inja`](https://github.com/pantor/inja)'s features.

This is a [`GdExtension`](https://godotengine.org/article/introducing-gd-extensions) addon for `Godot 4`.


Motivation
----------

- Generate tailored, efficient shaders.
- Generate gdscript during development.
- Generate dialogues, messages, and roleplays.
- …


Demo
----

If you have a bleeding edge Godot 4 (more recent than alpha9), you can download and run the demo in _Releases_ to see an example shader generation.

![A gooey cursor using a generated shader](demo/demo.gif)

> This source tree holds what's needed to build the shared libraries,
> not the shared libraries themselves since they are binary blobs.


Usage
-----

```gdscript
var tpl = "Hello, {{ name }} !"
var ginja = Ginja.new()
var msg = ginja.render(tpl, {
    'name': "Nathanaël",
})

print(msg)
```

> This is a very simple example, but you may use loops, conditions, comments, etc.
> See [Inja's documentation](https://pantor.github.io/inja/) for the available features.


You may also define your own functions for use in the templates:

```gdscript

func _ready():
	var ginja = Ginja.new()
	ginja.add_function("repeat", 2, self, "call_repeat")
	var msg = ginja.render("{{ repeat(msg, amount) }}", {
		'msg': "🎶!",
		'amount': 5,
	})
	assert(msg == "🎶!🎶!🎶!🎶!🎶!")


func call_repeat(msg: String, amount: int) -> String:
	return msg.repeat(times)

```

> You HAVE TO specify the amount of arguments your custom function uses.


You can also define and register variadic functions, like `sum` here:


```gdscript

func _ready():
	var ginja = Ginja.new()
	ginja.add_function_variadic("sum", self, "call_sum")
	var answer = ginja.render("{{ sum(a, b, c) }}", {
		'a': 41,
		'b': -5,
		'c': 6,
	})
	assert(answer == "42")


func call_sum(arguments: Array) -> int:
	var total := 0
	for argument in arguments:
		total += argument
	return total

```


Current Limitations
-------------------

- **LEAKING**  (see #1 — might not be us)
- GdNative **DOES NOT WORK IN EXPORTED HTML5 BUILDS**
- Single character unicode strings (like `🎶`) behave oddly and yield empty strings sometimes
- No personal plans for testing _Windows_ or _Mac_, contribs welcome
- Variadic user-defined functions need at least one parameter when called
- Limited to _Inja_ capabilities.  Eg: _Inja_ offers no filters
- `include` and `extend` _might_ not work in exported projects _(to check)_
- Error when fetching callback `Object` from its `RID`: _(but it works)_

```error
ERROR: Condition "_instance_bindings != nullptr" is true.
   at: set_instance_binding (core/object/object.cpp:1771)
```
> Can't figure out if it's our fault or not, for this one.
> Idea: try `Ref<Object>` instead of `RID&`.


Support
-------

Unsupported in HTML5 builds, beware !  See the [state of GdNative on HTML5](https://github.com/godotengine/godot-proposals/issues/147).

- [x] Linux x86_64
- [x] Linux x86_32 _(to test)_
- [x] Windows x86_64 _(to test)_
- [x] Windows x86_32 _(to test)_
- [x] Mac x86_64 _(to test)_
- [ ] …

> Shader and gdscript generation is still useful during development,
> even if we can't rely on it at runtime for now.


Install
-------

The installation is as usual, through the Assets Library. (todo)

Meanwhile, dowload the latest Release (on the right), or clone this repository and build the shared objects.

> The shared libs were not added to this repository, since they are build artifacts.  (CI is coming)

Then you can copy the `addons/` directory of this project into yours.

Finally, enable the plugin in `Scene > Project Settings > Plugins`.


Build
-----

Build the shared libraries:

    cd gdextension_generator
    CORES=4 TARGET=debug build.sh
    CORES=4 TARGET=release build.sh

> Want to do this using CI, but we need a custom action, or to find a docker image with everything we need.


Setup Dev Notes
---------------

> **Use homebuilt Godot 4 from master**, not alpha9.

### Submodules

Ideally:

    git submodule add -b 4.x https://github.com/godotengine/godot-cpp gdextension_generator/godot-cpp

Pragma 'til the `4.x` branch exists:
    
    git submodule add https://github.com/godotengine/godot-cpp gdextension_generator/godot-cpp

> Remember to init the submodules!


### Build

The first step is always to compile the `godot-cpp` library:
    
    cd gdextension_generator/godot-cpp
    scons target=debug

<!-- Maybe needed later on, maybe not -->
<!--     scons target=debug generate_bindings=yes -->


Then we can compile our shared library:
    
    cd gdextension_generator
    scons target=debug

It outputs in the `addons/goutte.template.inja/bin/` directory.

> The libs are voluminous because they are not `strip`ped, you may do so:

    strip addons/goutte.template.inja/bin/libgd*


### Demo

The demo uses a symlink to get as child its sibling `addons/` directory.
Might not hold through git across platforms, beware.


### Inja

Inja has been copied from `3.3`, and the `#include` of `json` have been changed to use the local file: `#include "json.hpp"`.  Both are warts, and enlightened suggestions are welcome.


Vanilla GdScript Implementation
-------------------------------

> Just thoughts on a vanilla gdscript fallback for Ginja.

### ANTLR

Needs `gdscript` templates and runtime first.  Those are `404` for now.

### CUSTOM

Would allow for easier customization of the enclosing markup tokens.
Much more work.  Faster code, though.  (if done well)


