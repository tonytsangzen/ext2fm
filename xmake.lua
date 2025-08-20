add_rules("mode.debug", "mode.release")

rule("slint-compiler")
    set_extensions(".slint")

    on_config(function (target)
        local r = target:rule("c++.build")
        if r then
            r = r:clone()
            r:add("deps", "@slint/compiler", {order = true})
            target:rule_add(r)
        end

        local autogendir = path.join(target:autogendir(), "slint")
        os.mkdir(autogendir)
        target:add("includedirs", autogendir)

        import("lib.detect.find_program")
        local slint_compiler = find_program("slint-compiler", {envs = os.joinenvs(os.getenvs(), target:pkgenvs())})
        assert(slint_compiler, "could not find slint-compiler")
    end)

    on_buildcmd_file(function (target, batchcmds, sourcefile, opt)
        import("lib.detect.find_program")
        local slint_compiler = find_program("slint-compiler", {envs = os.joinenvs(os.getenvs(), target:pkgenvs())})

        local abs_path = path.join(target:scriptdir(), sourcefile)
        local autogendir = path.join(target:autogendir(), "slint")
        local output_path = path.join(autogendir, path.basename(abs_path) .. ".h")

        batchcmds:vrunv(slint_compiler, {abs_path, "-o", output_path})
        batchcmds:add_depfiles(sourcefile)
    end)
rule_end()

package("slint")
    set_description("Slint C++ SDK")
    set_homepage("https://slint.dev/")
    set_description("Slint is a declarative GUI toolkit to build native user interfaces for Rust, C++, or JavaScript apps. ")
    set_license("Slint License")

    add_urls("https://github.com/slint-ui/slint/releases/download/v$(version)/Slint-cpp-$(version)-Darwin-arm64.tar.gz")
    add_versions("1.12.1", "7ed3363f8ceeb31d813d0aa8c42704a643d1896cb2d5b705585308c2db9b9a68")

    add_bindirs("bin")
    add_includedirs("include/slint")

    on_install(function (package)
        os.cp("*", package:installdir())
        package:addenv("LD_LIBRARY_PATH", package:installdir("lib"))
    end)

    on_test(function (package)
        os.vrun("slint-compiler --version")
    end)
package_end()

package("e2fsprogs")
    set_sourcedir("e2fsprogs")
    on_install(function (package)
        local configs = {}
		import("package.tools.autoconf").install(package, configs)
    end)
package_end()

add_requires("e2fsprogs")
add_requires("slint")

target("libext2fm")
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

target("ext2fm-gui")
	set_languages("c++20")
	add_rules("slint-compiler")
	set_kind("binary")
	add_packages("slint")
	add_files("gui/src/*.cpp", "gui/ui/*.slint")
target_end()
