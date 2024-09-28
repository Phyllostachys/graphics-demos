add_rules("mode.debug", "mode.release")

if is_os("windows") then
    add_requires("vcpkg::glad", "vcpkg::glfw3", "vcpkg::glm")
else
    add_requires("glad")
    add_requires("glm")
    add_requires("glfw3", {system = true})
end

target("scene_with_camera")
    set_kind("binary")
    add_files("src/*.cpp")

if is_os("windows") then
    add_packages("vcpkg::glad", "vcpkg::glfw3", "vcpkg::glm")
    add_syslinks("user32", "shell32", "gdi32")
else
    add_packages("glad", "glfw3", "glm")
end
