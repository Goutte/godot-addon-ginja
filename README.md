
This is a string template engine for `gdscript`.

Provides access to (some of) [`Inja`](https://github.com/pantor/inja)'s features.

This is a [`GdExtension`](https://godotengine.org/article/introducing-gd-extensions) addon for `Godot 4`.


Support
-------

- [x] Linux x86_64
- [ ] Linux x86_32
- [ ] Windows x86_64
- [ ] Windows x86_32
- [ ] Mac x86_64
- [ ] Mac x86_32
- [ ] …


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

> This is a very simple example, but you may use loops, conditions, filters, etc.


Current Limitations
-------------------

- No User-defined filters and functions.


Install
-------

The installation is as usual, through the Assets Library. (todo)
You can also simply copy the `addons/` directory of this project into yours, it should work.

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
Might not hold through git, beware.   Just remake the symlink, for now.


### Inja

Inja has been copied from `3.3`, and the `#include` of `json` have been changed to use the local file: `#include "json.hpp"`.  Both are warts, and enlightened suggestions are welcome.

