# snuffbox-mantis
## The mantis revision of Snuffbox, which is cross-platform on Windows and Linux (Ubuntu).

#### Note: all Linux code was compiled and tested with GCC under Ubuntu 16.04.2 LTS in the QtCreator IDE

<dl><hr></hr></dl>

### Important CMake variables

| Name                | Optional      | Description                                                                      |
|:--------------------|:--------------|:---------------------------------------------------------------------------------|
| EASTL_ROOT_DIR      | No            | Points to the root directory of the EASTL git revision cloned to the machine     |
| V8_ROOT_DIR         | Yes           | Points to the V8 root directory from 'fetch v8' (containing 'out.gn')            |
| wxWidgets_ROOT_DIR  | No            | Windows only, as this should be found on Ubuntu, wxWidgets-3.1.0 root directory  |

#### Note: in QtCreator the dialog for variables doesn't update until you set the EASTL_ROOT_DIR to a valid location, the same applies to GLFW_ROOT_DIR. I'm not sure how to resolve this issue

<dl><hr></hr></dl>

**Experimental:** Use **gen_\<platform\>** to fetch and compile all dependencies for snuffbox-mantis. **'depot_tools'** for V8 still has to be downloaded and setup manually. Don't forget to still read
the important notes under **'Important notes for building V8..'** for each platform

<dl><hr></hr></dl>

### Vulkan with the LunarG SDK
Vulkan can simply be downloaded through the official LunarG SDK. This is not included in the generation tool. After installation of the SDK all paths should be setup correctly and found through
the FindVulkan module in CMake. Download the LunarG SDK [here](https://vulkan.lunarg.com/)

If Vulkan was still not found, specify Vulkan_LIBRARY and Vulkan_INCLUDE_DIR in CMake manually to the paths where Vulkan is installed. (The library is found under **<Vulkan root>/Lib/vulkan-1**)

As for shader compilation, **python3** is required to be installed on the system, else the CMake generation will fail. A stable release of python3 can be found [here](https://www.python.org/downloads/release/python-354/).

<dl><hr></hr></dl>

### Get EASTL from the official repo
You can clone the repository like so:

```bash
-  git clone https://github.com/electronicarts/EASTL
```

Place this in a folder and let **EASTL_ROOT_DIR** point to it

<dl><hr></hr></dl>

### Get GLFW from the official repo
You can clone the repository and switch to the correct branch like so:

```bash
-  git clone https://github.com/glfw/glfw.git
-  git checkout latest
```

It is however not necessary to switch to the 'latest' branch, 'master' should work as well.
However, please note 'latest' is the latest stable *release* build, unlike master.

Place this in a folder and let **GLFW_ROOT_DIR** point to it

<dl><hr></hr></dl>

### Building wxWidgets-3.1.0 on Windows

#### Download wxWidgets from [here](https://github.com/wxWidgets/wxWidgets/releases/download/v3.1.0/wxMSW-3.1.0-Setup.exe)

#### Compilation

Compilation is as simple as opening 'wxWidgetsRoot/buid/msw/wx_vc14.sln' in Visual Studio 2015.
After this change all projects to be **/MTd** in Debug and **/MT** in Release (under **C++/Code Generation** in the properties)

<dl><hr></hr></dl>

### Getting wxWidgets-3.0.0 on Ubuntu

To put it simply, I went with the easy way and just preformed

```bash
-  sudo apt-get install libwxbase3.0-dev libwxgtk3.0-dev
```

<dl><hr></hr></dl>

### Important notes for building V8 for Windows x86_64

#### Download & install [git](https://git-scm.com/download/win)
#### Download & install 'Debugging Tools for Windows' from [this](https://download.microsoft.com/download/3/6/3/36301F10-B142-46FA-BE8C-728ECFD62EA5/windowssdk/winsdksetup.exe) installer package
#### Download [depot_tools](https://storage.googleapis.com/chrome-infra/depot_tools.zip) and drag it somewhere to your liking

#### Set environment variables

Add the depot_tools to your PATH, make sure it's prepended so the right version of python is used.
You can check if we're using the python in depot_tools by running the command:

```bash
-  where python
```

Set the following user variables (these are set automatically in gen_win32.cmd):

```
-  DEPOT_TOOLS_WIN_TOOLCHAIN = 0
-  GYP_MSVS_VERSION = 2015
```

#### Visual Studio

Make sure to only have Visual Studio 2015 installed. 2017 seems to conflict a lot with the V8 library setup.
Also; apparently it matters in what order VS dependencies are installed. Make sure to deinstall any older versions
of libraries like VS Build Tools.

Make sure you have the following folder in "DriveLetter:/Program Files (x86)/Microsoft Visual Studio 14.0/"

```
- DIA SDK
```

..and the following file..

```
- VC/vcvarsall.bat
```

If you reinstall any Visual Studio dependencies, don't forget to reinstall the Debug Tools mentioned above as well.

#### Now follow the same build principles as on Ubuntu, under 'Generation & compilation'

<dl><hr></hr></dl>

### Important notes for building V8 for Ubuntu x86_64

#### Install the following dependencies (assuming you have git and python installed)

```bash
-  sudo apt-get install libc++-dev libc++abi-dev  
```

#### Setup environment

Get depot_tools from the chromium git

```bash
-  git clone https://chromium.googlesource.com/chromium/tools/depot_tools.git
```

Prepend depot_tools to $PATH, preferably in .bashrc

```bash
-  export PATH=/path/to/depot_tools:"$PATH"
```

#### Generation & compilation

Run the following commands in the directory where you want to save V8:

```bash
-  gclient
-  fetch v8
-  cd v8
-  gclient sync
```

Generate GN files

```bash
-  python "tools/dev/v8gen.py" x64.debug
-  python "tools/dev/v8gen.py" x64.release
```

Generate Ninja files

*A text editor will open, probably VIM, copy the arguments from either of the following*

Debug: [x64.debug](snuffbox-javascript/v8_args.x64.debug.gn)

Release: [x64.release](snuffbox-javascript/v8_args.x64.release.gn)

```bash
-  gn args "out.gn/x64.debug"
-  gn args "out.gn/x64.release"
```

Compile with Ninja (-v for verbose logging, make sure it's compiling with g++)

```bash
-  ninja [-v] -C "out.gn/x64.debug"
-  ninja [-v] -C "out.gn/x64.release"
```

#### Linking in [FindV8](cmake/FindV8.cmake)

As for linking; link order is important here. This configuration seems to work without problems

```
-  libv8_libplatform 
-  libv8_base 
-  libv8_libbase 
-  libv8_snapshot 
-  libv8_libsampler 
-  libv8_nosnapshot 
-  libicui18n 
-  libicuuc
-  libc++ 
-  libc++abi
```

<dl><hr></hr></dl>

#### A list of available console variables

Booleans are set by 'true' and 'false'

|Name              |Type         |Description                                     |Default                                |
|:-----------------|:------------|:-----------------------------------------------|:--------------------------------------|
|console           |Boolean      |Should the console be enabled?                  |false                                  |
|console_ip        |String       |The IP of the external console to connect with  |127.0.0.1                              |
|console_port      |Number       |The port of the external console to connect on  |**SNUFF_DEFAULT_PORT** in CMake        |
|reload            |Boolean      |Should files be hot-reloaded?                   |false                                  |
|reload_freq       |Number       |The milliseconds to wait for a reload check     |**SNUFF_RELOAD_AFTER** in CMake        |
|src_directory     |String       |The working directory to load content from      |No value, the target root will be used |

