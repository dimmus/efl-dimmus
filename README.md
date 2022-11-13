<img src="https://raw.githubusercontent.com/Dimmus/efl/master/data/readme/efl.png" align="right"/>

# Enlightenment Founation Library

[![Packaging status](https://repology.org/badge/tiny-repos/efl.svg)](https://repology.org/project/efl/versions)
[![CII Best Practices](https://bestpractices.coreinfrastructure.org/projects/6640/badge)](https://bestpractices.coreinfrastructure.org/projects/6640)

[API/ABI changes review for EFL](https://abi-laboratory.pro/index.php?view=timeline&l=efl)

*Enlightenment Founation Library (EFL)* is a collection of libraries for handling many common tasks a
developer may have such as data structures, communication, rendering,
widgets and more.

There are many components inside EFL. They also build various things
like shared libraries, loadable plug-in modules and also binary
executables. Different elements will end up being licensed differently
too. Below is a rundown of the components, what they do and their
licensing.

All library source is in the src/lib/ directory. All binaries are in
src/bin/. All loadable modules are in src/modules/. All data files are
in the data/ directory. Licensing details are listed in COPYING and
the licenses/ directory. The pc/ directory holds all the Package
Config files installed to help developers build against EFL.

**Attention:** EFL is under active development.  Feel free to report bugs [here](https://github.com/dimmus/efl/issues) or to the official [repo](https://git.enlightenment.org/enlightenment/efl/issues), instabilities or feature wishes.

-----

<div align="center"> <a href="http://www.enlightenment.org/">Homepage</a> | <a href="https://www.enlightenment.org/download/">Downloads</a> | <a href="https://www.enlightenment.org/news">News</a> | <a href="https://www.enlightenment.org/develop/start.md">Developer's Manual</a> | <a href="https://www.enlightenment.org/contact/">Contact</a> | <a href="https://kiwiirc.com/nextclient/irc.libera.chat/e">Chat</a> </div>

-----

For more documentation please see: [www.enlightenment.org/doc](https://www.enlightenment.org/docs)

## Platforms

EFL is primarily developed on Linux (GNU/Linux) and should work on
most distributions as long as dependencies are provided. It has been
compiled and run also on Windows (using MSYS2 + mingw-w64) - please see
[windows docs](https://www.enlightenment.org/docs/distros/windows-start.md),
Mac OS X, FreeBSD and NetBSD.

## Requirements

EFL requires a C and C++ compiler by default. C++ exists mostly to interface
to C++ libraries like Bullet and our C++ bindings.

Required by default:

* libpng
* libjpeg
* openjpeg2
* gstreamer (Ensure all codecs you want are installed.)
* zlib
* luajit (lua 5.1 or 5.2 support optional)
* libtiff
* openssl
* curl
* dbus
* libc
* fontconfig
* freetype2
* fribidi
* harfbuzz
* libpulse
* libsndfile
* libx11
* libxau
* libxcomposite
* libxdamage
* libxdmcp
* libxext
* libxfixes
* libxinerama
* libxrandr
* libxrender
* libxss
* libxtst
* libxcursor
* libxi (2.2 or newer)
* opengl(mesa etc.) (opengl/glx/full or opengl-es2/egl. full opengl only on osx - must be explicitly specified to be full to have support)
* giflib/libgif
* util-linux (limbount + libblkid)
* systemd / libudev
* poppler / poppler-cpp
* libraw
* libspectre
* librsvg
* openmp (clang needs libomp, while gcc uses libgomp)
* libwebp


### Wayland support

You may also want wayland support when on Linux. This enables support
for EFL to target wayland support for client applications. To do this
supply:
``` sh
-Dwl=true
```

### Framebuffer support

For more modern framebuffer support you may want drm/kms rendering
support so enable this. This is what you also want for wayland
compositor support in enlightenment as it will want to be able to
render to a modern framebuffer target with atomic buffer swapping. To
do this provide:
``` sh
-Ddrm=true
```

Legacy fbcon support also exists, but you probably no longer want to
use this as it is not maintained anymore. This supports basic frame-buffers
like /dev/fb as well as input via /dev/input for keyboards and mice in a
basic way. Enable this with:
``` sh
-Dfb=true
```

You may want to change the install prefix for EFL with:
``` sh
--prefix=/path/to/prefix
```

The default prefix if not given is "/usr/local". Many people like to
use prefixes like /opt/e or /opt/efl or /home/USERNAME/software etc.

### Compiler flags

You can affect compilation optimization, debugging and other factors
by setting your `CFLAGS` environment variable (and `CXXFLAGS`). Be aware
that to ensure ABI stability you should use the exact same `CFLAGS` /
`CXXFLAGS` for all the build of EFL and any applications/libraries that
depend on them.

There are many other configure options that can be used, but in
general it is not a good idea to go enabling or disabling things
unless you wish to break things. The defaults are well tested, with
the above recommended options also being well tested. Go much further
and your mileage may vary wildly. Disabling features is a good way of
breaking EFL functionality, so it is not recommended to mess with
these without understanding the implications. The defaults
have been carefully considered to provide full functionality so users
will not be missing anything.

-----

## Compiling and Installing

Meson is the build system used for this project. For more information
please see [mesonbuild.com](https://mesonbuild.com)

You will need normal build tooling installed such as a compiler (gcc
or clang for example), pkg-config, ninja, any relevant package-dev or
package-devel packages if your distribution splits out development
headers (e.g. libc6-dev) etc.

Depending on where dependencies,  you might have to set your
`PKG_CONFIG_PATH` environment variable like:
```sh
export PKG_CONFIG_PATH=/usr/local/lib/pkgconfig:/usr/lib/pkgconfig
```

Also note that some distributions like to add extra arch directories
to your library locations so you might have to have more like:
```sh
export PKG_CONFIG_PATH=/usr/local/lib/pkgconfig:/usr/local/lib64/pkgconfig:/usr/local/lib/x86_64-linux-gnu/pkgconfig:/usr/lib/pkgconfig:/usr/lib64/pkgconfig:/usr/lib/x86_64-linux-gnu/pkgconfig
```

You will need to ensure that the destination library directory (e.g.
`/usr/local/lib` is in your /etc/ld.so.conf or /etc/ld.so.conf.d/
files and after installing anything that installs libraries you
re-run `ldconfig`. Please see relevant documentation on ldconfig and
ld.so.conf for your distribution.

You might also want to add the destination bin dir to your environment
variable PATH (see documentation on your shell PATH variable) such as:
```sh
export PATH=/usr/local/bin:/usr/bin:/bin
```

Normal compilation in /usr/local:
```sh
meson . build
ninja -C build
sudo ninja -C build install
```

For meson build generic options:
```sh
meson --help
```

For a list of project specific options supported:
```sh
cat meson_options.txt
```

To set 1 or more project specific options:
```sh
meson --prefix=/path/to -Doption=value [-Dother=value2] [...] . build
```

To display current configuration:
```sh
meson configure build
```

The above will only work after at least the following is done:
```sh
meson . build
```

### Quick build help

How to clean out the build and config and start fresh:
```sh
rm -rf build
```

How to make a dist tarball and check its build:
(must do it from git tree clone and commit all changes to git first)
```sh
ninja -C build dist
```

How to change prefix:
```sh
meson --prefix=/path/to/prefix . build
```

How to install in a specific destination directory for packaging:
```sh
DESTDIR=/path/to/destdir ninja -C build install
```

How to build with verbose output (full commands run):
```sh
ninja -C build -v
```

-----

**NOTE:** Nvidia OpenGL-ES/EGL seems to have bugs and performance issues.
All other drivers work just fine except Nvidia with OpenGL-ES/EGL. Users have
reported stuttering and graphical glitches and sometimes windows not
updating. If you want the best results and are stuck on Nvidia and unable to
change that, then:

  * Make sure you do not enable **Wayland** i.e. Meson option `-Dwl=false` (this is default anyway if not provided)
  * Make sure you do not enable **DRM** i.e. Meson option `-Ddrm=false` (this is default anyway if not provided)
  * Make sure **OpenGL** is **full** i.e. Meson option `-Dopengl=full`
  
[![Packaging status](https://repology.org/badge/vertical-allrepos/efl.svg)](https://repology.org/project/efl/versions)

## Discussions

[GitHub Discussions](https://docs.github.com/discussions/quickstart) is a feature to enable fluid and open discussions within the community
for topics that are not related to code, unlike issues.

This is an experiment we are trying in our repositories to see if it helps move discussions out of issues so that issues remain actionable by the team or members of the community.
There should be no expectation that EFL development team members are regular participants in the discussions.
Individual EFL development team members may choose to participate in discussions, but the expectation is that community members help drive discussions so that team members
can focus on issues.

Create or join a [discussion](https://github.com/dimmus/efl/discussions).
