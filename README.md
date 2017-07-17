# snuffbox-mantis
## The mantis revision of Snuffbox, which is cross-platform on Windows and Linux.

### Important notes for building V8 for Windows x86_64

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

### Important notes for building V8 for Linux x86_64

#### Install the following dependencies:

```bash
-  (sudo apt-get install <name>)

-  libc++-dev
-  libc++abi-dev
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

Run the following commands in the directory where you want to save V8:

```bash
-  gclient
-  fetch v8
-  cd v8
-  gclient sync
```

#### Generation & compilation

Generate GN files

```bash
-  python "tools/dev/v8gen.py" x64.debug
-  python "tools/dev/v8gen.py" x64.release
```

Generate Ninja files
(A text editor will open, probably VIM, copy the arguments from 'v8_args.x64.conf.gn' in 'snuffbox-javascript')
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