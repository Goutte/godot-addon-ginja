
This is a string template engine for `gdscript`.

Provides access to (some of) [`Inja`](https://github.com/pantor/inja)'s features.

This is a [`GdExtension`](https://godotengine.org/article/introducing-gd-extensions) addon for `Godot 4`.


Motivation
----------

- Generate tailored, efficient shaders.
- Generate dialogues, messages, and roleplays.
- …


Usage
-----

```gdscript
var tpl = "Hello, {{ name }} !"
var engine = Ginja.new()
var msg = engine.render(tpl, {
    'name': "Nathanaël",
})

print(msg)
```

> This is a very simple example, but you may use loops, conditions, etc.
> See Inja's documentation for the available features.


Current Limitations
-------------------

- No plans for Windows or Mac, either feed me or help out.
- Rough user-defined functions. (Strings only, awkward API that will probably change)
- Limited to Inja capabilities.  Eg: Inja offers no filters.
- `include` and `extend` _might_ not work in exported projects _(to check)_
- Error when fetching callback `Object` from its `RID`: _(but it works)_

```error
ERROR: Condition "_instance_bindings != nullptr" is true.
   at: set_instance_binding (core/object/object.cpp:1771)
```
> Can't figure out if it's our fault or not, for this one.


Support
-------

- [x] Linux x86_64
- [ ] Linux x86_32
- [ ] Windows x86_64
- [ ] Windows x86_32
- [ ] Mac x86_64
- [ ] Mac x86_32
- [ ] …


Install
-------

The installation is as usual, through the Assets Library. (todo)
You can also simply copy the `addons/` directory of this project into yours, it should work.

> Nope, you need to build the shared libs first.  Should we add those to git?

Then, enable the plugin in `Scene > Project Settings > Plugins`.




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


### Demo

The demo uses a symlink to get as child its sibling `addons/` directory.
Does not hold through git, beware.   Just remake the symlink, for now.


### Inja

Inja has been copied from `3.3`, and the `#include` of `json` have been changed to use the local file: `#include "json.hpp"`.  Both are warts, and enlightened suggestions are welcome.

