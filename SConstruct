#!/usr/bin/env python
import os
import sys
import glob

# Get the environment variables
env = SConscript("godot-cpp/SConstruct")

# Find all Hunspell source files
def find_hunspell_sources():
    # Possible locations for Hunspell source files
    potential_paths = [
        "hunspell/src/hunspell/*.cxx",
        "hunspell/src/*.cxx",
        "hunspell/src/hunspell/*.cpp",
        "hunspell/src/*.cpp"
    ]
    
    all_sources = []
    for path in potential_paths:
        found_sources = glob.glob(path)
        all_sources.extend(found_sources)
    
    print("Found Hunspell sources:", all_sources)
    return all_sources

# Add Hunspell include directories
potential_include_paths = [
    "hunspell/src/hunspell",
    "hunspell/src"
]

for path in potential_include_paths:
    if os.path.exists(path):
        env.Append(CPPPATH=[path])
        print(f"Added include path: {path}")

# Adjust include and link flags for Windows
if env["platform"] == "windows":
    env.Append(CCFLAGS=["/EHsc", "/bigobj"])
    # Enable exceptions
    env.Append(CCFLAGS=["/DHUNSPELL_STATIC"])
    # Static linking for Hunspell
    env.Append(CPPDEFINES=["NOMINMAX"])
    # Define NOMINMAX to avoid issues with Windows.h

# Define our sources
sources = Glob("src/*.cpp")

# Add Hunspell sources
hunspell_sources = find_hunspell_sources()
if not hunspell_sources:
    print("ERROR: No Hunspell source files found!")
    Exit(1)

sources.extend(hunspell_sources)

# Create the library target
if env["platform"] == "macos":
    library = env.SharedLibrary(
        "addons/hunspell/bin/libhunspell{}{}".format(env["suffix"], env["SHLIBSUFFIX"]),
        source=sources,
    )
else:
    library = env.SharedLibrary(
        "addons/hunspell/bin/hunspell{}{}".format(env["suffix"], env["SHLIBSUFFIX"]),
        source=sources,
    )

Default(library)
