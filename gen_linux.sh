echo -e "\e[92m\nsnuffbox-mantis generation tool, please make sure 'git' is installed and accessible from the command line\e[0m\n"

if [ ! -d "deps" ]; then
	mkdir deps
	echo "Created 'deps' folder"
fi

cd deps

echo -n "Clone EASTL? (y/n)"
read option
if echo "$option" | grep -iq "^y" ;then
	if [ -d "eastl" ]; then
		rm -r -f eastl
		echo "Removing old 'eastl' folder as we're regenerating"
	fi
	mkdir eastl
	echo -e "\e[32mMade the 'eastl' folder\e[0m"
	echo -e "\e[32mCloning EASTL from git\e[0m"
	git clone https://github.com/electronicarts/EASTL.git eastl
fi

echo -e ""

echo -n "Clone GLFW? (y/n)"
read option
if echo "$option" | grep -iq "^y" ;then
	if [ -d "glfw" ]; then
		rm -r -f glfw
		echo "Removing old 'glfw' folder as we're regenerating"
	fi
	mkdir glfw
	echo -e "\e[32mMade the 'glfw' folder\e[0m"
	echo -e "\e[32mCloning GLFW from git\e[0m"
	git clone https://github.com/glfw/glfw.git glfw
	cd glfw
	git checkout latest
	cd ../
fi

echo -e ""

echo "To download and build V8, depot_tools is required in your system PATH"
echo -e "\e[91mBuilding V8 takes quite some time and is only required with SNUFF_JAVASCRIPT\e[0m\n"
echo "libc++ and lib++abi are also required on Linux builds"

echo -n "Download libc++-dev and libc++abi-dev? (y/n)"
read option
if echo "$option" | grep -iq "^y" ;then
	sudo apt-get install libc++-dev libc++abi-dev
fi

echo ""

echo -n "Download & build V8? (y/n)"
read option
if echo "$option" | grep -iq "^y" ;then
	echo -n "Fetch V8? (y/n)"
	read option
	if echo "$option" | grep -iq "^y" ;then
		if [ -d "v8" ]; then
			rm -r -f v8
			rm -r -f .gclient
			rm -r -f .gclient_entries
			echo "Removing old 'v8' folder as we're regenerating"
		fi
		gclient
		fetch v8
		cd v8
		gclient sync
		cd ../
	fi

	echo -n "Build V8? (y/n)"
	read option
	if echo "$option" | grep -iq "^y" ;then
		cd v8
		python "tools/dev/v8gen.py" x64.debug
		python "tools/dev/v8gen.py" x64.release
		cd ../
		yes | \cp -rf "../snuffbox-javascript/gn/x64.debug.linux.gn" "v8/out.gn/x64.debug/args.gn"
		yes | \cp -rf "../snuffbox-javascript/gn/x64.release.linux.gn" "v8/out.gn/x64.release/args.gn"
		cd v8
		gn gen "out.gn/x64.debug"
		gn gen "out.gn/x64.release"

		echo -e "\n"

		echo -n "Compile V8? (y/n)"
		read option
		if echo "$option" | grep -iq "^y" ;then
			ninja -C "out.gn/x64.debug"
			ninja -C "out.gn/x64.release"
		fi
	fi
fi

echo ""

echo -n "Download wxWidgets-3.0.0? (y/n)"
read option
if echo "$option" | grep -iq "^y" ;then
	sudo apt-get install libwxbase3.0-dev libwxgtk3.0-dev
fi

echo -e "\n\e[92mAll done generating\e[0m"

read -n 1 -s -r -p "Press any key to continue.."

echo -e "\n"
