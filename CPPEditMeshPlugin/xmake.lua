add_rules("mode.debug", "mode.release")
add_requires("cgal")
target("CPPEDITMESHPLUGIN")
    set_kind("shared")
    add_rules("utils.symbols.export_all",{export_classes = true})
    add_files("src/*.cpp")
    add_headerfiles("include/*.h")
    add_packages("cgal")
     
    set_targetdir("../UnityEditMeshWithCGAL/Assets/Plugins") -- output to target 