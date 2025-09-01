add_rules("mode.debug", "mode.release")

package("e2fsprogs")
    set_sourcedir("e2fsprogs")
    on_install(function (package)
        local configs = {}
		import("package.tools.autoconf").install(package, configs)
    end)
package_end()

add_requires("e2fsprogs")

target("libext2fm")
	set_basename("ext2fm")
    set_kind("shared")
	add_packages("e2fsprogs")
	add_includedirs("include", "e2fsprogs/lib", "e2fsprogs/lib/ext2fs", "src")
    add_files("src/*.c")
target_end()

target("ext2fm")
	set_kind("binary")
	add_includedirs("include")
	add_deps("libext2fm")
	add_files("console/main.c")
target_end()
