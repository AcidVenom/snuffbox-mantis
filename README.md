# snuffbox-mantis
## The mantis revision of Snuffbox, which is cross-platform on Windows and Linux.

### Important CMake variables

| Tables              | Optional      | Description                                                                    |
|:--------------------|:--------------|:-------------------------------------------------------------------------------|
| EASTL_ROOT_DIR      | No            | Points to the root directory of the EASTL git revision cloned to the machine   |
| V8_ROOT_DIR         | Yes           | Points to the V8 root directory from 'fetch v8' (containing 'out.gn')          |
| wxWidgets_ROOT_DIR  | No            | Windows only, as this should be found on Linux, wxWidgets-3.1.0 root directory |

### Building wxWidgets-3.1.0 on Windows

#### Download wxWidgets from [here](https://github.com/wxWidgets/wxWidgets/releases/download/v3.1.0/wxMSW-3.1.0-Setup.exe)

#### Compilation

Compilation is as simple as opening 'wxWidgetsRoot/buid/msw/wx_vc14.sln' in Visual Studio 2015.
After this change all projects to be **/MTd** in Debug and **/MT** in Release (under **C++/Code Generation** in the properties)

<dl><hr></hr></dl>

### Getting wxWidgets-3.0.0 on Linux

To put it simply, I went with the easy way and just preformed

```bash
-  sudo apt-get install libwxbase3.0-dev libwxgtk3.0-dev
```

<dl><hr></hr></dl>

### Important notes for building V8 for Windows x86_64

#### Install git
#### Install 'Debugging Tools for Windows' from [this](https://download.microsoft.com/download/3/6/3/36301F10-B142-46FA-BE8C-728ECFD62EA5/windowssdk/winsdksetup.exe) installer package
#### Download [depot_tools](https://storage.googleapis.com/chrome-infra/depot_tools.zip) and drag it somewhere to your liking

#### Set environment variables

Add the depot_tools to your PATH, make sure it's prepended so the right version of python is used.
You can check if we're using the python in depot_tools by running the command:

```bash
-  where python
```

Set the following user variables:

```
-  DEPOT_TOOLS_WIN_TOOLCHAIN=0
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

#### Now follow the same build principles as on Linux, under 'Generation & compilation'

<dl><hr></hr></dl>

### Important notes for building V8 for Linux x86_64

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
-  export PATH=`pwd`/path/to/depot_tools:"$PATH"
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