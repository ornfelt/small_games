# Small & fun games

## BirdGame:
![BirdGame](./images/BirdGame.png?raw=true "BirdGame")

## CPP_FightingGame:
![CPP_FightingGame](./images/CPP_FightingGame.png?raw=true "CPP_FightingGame")

## Craft:
![Craft](./images/craft.png?raw=true "Craft")

## pacman:
![pacman](./images/pacman.png?raw=true "pacman")

## space-shooter:
![space-shooter](./images/space-shooter.png?raw=true "space-shooter")


## Compiling:

Changes made for linux:

For Pacman you might need to change:

target_link_options(pacman PRIVATE LINKER:-dead_strip)

->

target_link_options(pacman PRIVATE)

For BirdGame you might need to alter itoa and add something like:

#if !defined(_MSC_VER)

    auto strcpy_s = strcpy;
    
#endif

Then change the compile command:

g++ -Isrc/include -Lsrc/lib -std=c++17 -g *.cpp -o main.exe -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer

->

g++ -std=c++17 -g *.cpp -o main -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer

For FightingGame you need to add: std::size_t instead of size_t...

## Windows compiling notes:

space-shooter: use developer command prompt for VS and .\build.bat

pacman: cmake . and build in MS VS.

Craft:
Download and install [cURL](http://curl.haxx.se/download.html) (32bit required I think) so that
CURL/lib and CURL/include are in your Program Files directory. You can use the provided CURL dir.

Use the following commands in place of the ones described in the next section.

    cmake -G "MinGW Makefiles"
    mingw32-make

You also need libcurl.dll from the CURL dir (or have it in path). You can use other textures from the textures dir.
