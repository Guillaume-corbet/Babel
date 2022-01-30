# BABEL DOCUMENTATION
\
by Maxime Barbier and Samuel Parayre
<p>&nbsp;</p>

## Summary
1. [Remotes](#REMOTES)
2. [Install Dependencies](#install-dependencies)
    1. [Fedora](#Fedora)
    2. [Ubuntu](#Ubuntu)
3. [Create Conan build folder](#create-conan-build-folder)
4. [Cmake](#cmake)
5. [.gitignore](#.gitignore)
6. [Alsa](#Alsa)
<p>&nbsp;</p>

## REMOTES

to add remotes to conan you need to do :
conan remote add <NAME> <URL>

and the only remote to add is :
https://api.bintray.com/conan/conan-community/conan
https://api.bintray.com/conan/conan-center
https://api.bintray.com/conan/bincrafters/public-conan

name it randomly ;)
<p>&nbsp;</p>

## INSTALL DEPENDENCIES

You would need some packages to compile and run conan and your libs. Here is the list:

### Fedora

>sudo dnf install libfontenc-devel libXaw-devel libXcomposite-devel libXdmcp-devel libXtst-devel libXres-devel xorg-x11-xkb-utils-devel libXScrnSaver-devel libXvMC-devel xorg-x11-xtrans-devel xcb-util-wm-devel xcb-util-keysyms-devel xcb-util-renderutil-devel libXdamage-devel xkeyboard-config-devel

### Ubuntu

>sudo apt install libfontenc-dev libxaw7-dev libxcomposite-dev libxdmcp-dev libxtst-dev libxres-dev x11-xkb-utils libxss1 libxvmc-dev xtrans-dev libxcb-ewmh-dev libxcb-keysyms1-dev libxcb-render-util0 libxdamage-dev xkb-data libx11-xcb-dev libxcb-render0-dev libxcb-render-util0-dev libxcb-xkb-dev libxcb-icccm4-dev libxcb-image0-dev libxcb-randr0-dev libxcb-shape0-dev libxcb-sync-dev libxcb-xfixes0-dev libxcb-xinerama0-dev

Make sure all of these are fully installed before going on the next step !
<p>&nbsp;</p>

## CREATE CONAN BUILD FOLDER

In order to create conan build architecture, you may need this command:

>mkdir build && cd build && conan install .. --build missing && cmake .. -G “UnixMakefiles” && cmake –build

This command will download and install all the libs contain in "conanfile.txt".
Two files will be created "conanbuildinfo.cmake" and "conanbuildinfo.txt" and they are necessary to compile so copy them to the root of the repo.
<p>&nbsp;</p>

## CMAKE

Then you are ready to compile !

>cmake CMakeLists.txt

>make
<p>&nbsp;</p>
<p>&nbsp;</p>

## .gitignore

Some files are undesirable on github ! Don't forget to add all of these files to your ```.gitignore``` file.

- babel_server
- babel_client
- a.out
- .vscode
- CMakeCache.txt
- cmake_install.cmake
- conanbuildinfo.cmake
- conanbuildinfo.txt
- conaninfo.txt
- CMakeFiles
- README.pdf
- babel_client_autogen
- babel_server_autogen
<p>&nbsp;</p>
<p>&nbsp;</p>
<p>&nbsp;</p>


## Alsa

Alsa have a problem with his own repo in conan so you need to help alsa to find your files with this command on linux : 

export ALSA_CONFIG_DIR=/usr/share/alsa