
Provides access to `Inja`.

This is a gdnative addon.

NO.

This is a [GdExtension](https://godotengine.org/article/introducing-gd-extensions) addon.


Install
-------

The installation is as usual, through the Assets Library.
You can also simply copy the files of this project into yours, it should work.

Then, enable the plugin in `Scene > Project Settings > Plugins`.




Setup Dev Notes
---------------

### Submodules

Ideally:

    git submodule add -b 4.x https://github.com/godotengine/godot-cpp gdextension_generator/godot-cpp

Pragma 'til the `4.x` branch exists:
    
    git submodule add https://github.com/godotengine/godot-cpp gdextension_generator/godot-cpp


### Build

The first step is always to compile the godot-cpp library:
    
    cd gdextension_generator/godot-cpp
    scons target=debug generate_bindings=yes


Then we can compile our shared library:
    
    cd gdextension_generator
    scons target=debug

