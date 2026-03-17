set_project("Lys-TiledMapLoading")
set_version("0.0.0")

add_repositories("TigoleRepo https://github.com/Tigole/xmake-repo.git")

add_requires("gtest", {system = false, configs = {main = true, gmock = false, shared = false}})
add_requires("lys-types")
add_requires("tinyxml-boosted")

target("lys-tiledmaploading")
    set_kind("headeronly")
    add_includedirs("includes/Lys-TiledMapLoading", {public = true})
    add_packages("lys-types", {public = true})
    add_packages("tinyxml-boosted", {public = true})

for _, testfile in ipairs(os.files("tests/**.cpp")) do 
    local name = path.basename(testfile)
    target(name)
        set_kind("binary")
        add_files(testfile)
        add_packages("gtest", {main = true, gmok = false})
        set_group("test")
        set_default(false)
        add_tests("maploading")
        add_deps("lys-tiledmaploading")
        add_packages("lys-types", {public = true})
end