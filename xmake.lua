add_rules("mode.debug", "mode.release")

-- wave.c
if is_os("windows") then
    add_requires("vcpkg::sdl2")
else
    add_requires("sdl2")
end

target("wave")
    set_kind("binary")
    set_installdir(".")
    add_files("wave.c")

if is_os("windows") then
    add_syslinks("user32", "shell32", "gdi32", "advapi32", "version", "winmm", "ole32", "oleaut32", "imm32", "setupapi")
    add_packages("vcpkg::sdl2")
else
    add_packages("sdl2")
end

-- scene with camera, geometry shader
includes("scene_with_camera")

-- phony target to build all other targets
target("graphics_demos")
    set_kind("phony")
    set_installdir(".")
    add_deps("wave", "scene_with_camera")
