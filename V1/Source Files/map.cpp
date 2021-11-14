#include "mapData.h"
#include "perlin.hpp"
#include <fstream>
#include <cstring>
#include <random>

basic::texture tileTextures[map::textureAmount];

const std::string tileFile[map::textureAmount] ={
    "Soil.png",
    "Grass.png",
    "Tree.png"
};

bool map::isTileInWorld(basic::Vector2i Pos) {
    if (Pos.x >= 0 && Pos.x < map::sizeX && Pos.y >= 0 && Pos.y < map::sizeY)
        return true;
    else
        return false;
}

//This function needs to be fixed.
int getBitTile(int x, int y) {
    if (x == -1 && y == 0)
        return 0;
    else if (x == -1 && y == -1)
        return 1;
    else if (x == 0 && y == -1)
        return 2;
    else if (x == 1 && y == -1)
        return 3;
    else if (x == 1 && y ==  0)
        return 4;            
    else if (x == 1 && y ==  1)
        return 5;            
    else if (x == 0 && y ==  1)
        return 6;
    else if (x == -1 && y == 1)
        return 7;
}

void map::world::getSidesOfSides(basic::Vector2i Pos) {
    for (int ny = -1; ny <= 1; ny++)
    for (int nx = -1; nx <= 1; nx++) {
        basic::Vector2i normal(Pos.x+nx, Pos.y+ny);
        if (nx == 0 && ny == 0)
            continue;
        int i  = getBitTile(nx, ny);
        if (!isTileInWorld(normal) || map[normal.x][normal.y].Type != map[Pos.x][Pos.y].Type) {
            map[Pos.x][Pos.y].Sides[i] = true;
        }
        else {
            map[Pos.x][Pos.y].Sides[i] = false;
        }
    }
}

void map::world::save() {
    std::fstream f;
    f.open("data/world.dat", std::ios::out | std::ios::binary);

    if (f.is_open()) {
        f.write(reinterpret_cast<char*>(this), sizeof(map::world));
        f.close();
    }
    else {
        std::cout << "Could not save binary.";
    }

}

void map::world::load() {
    std::fstream f;
    f.open("data/world.dat", std::ios::in | std::ios::binary);

    if (f.is_open()) {
        f.read(reinterpret_cast<char*>(this), sizeof(map::world));
        f.close();
    }
    else {
        std::cout << "Could not load binary.";
    }
}

void map::world::getSides(basic::Vector2i Pos) {
    for (int y = -1; y <= 1; y++)
    for (int x = -1; x <= 1; x++) {
        basic::Vector2i normal(Pos.x + x, Pos.y + y);
        if (!isTileInWorld(normal))
            continue;
        getSidesOfSides(normal);
    }
}

//This FUnction needs to be fixed.
float map::world::getSmooth(basic::Vector2i Pos) {
    bool* b = map[Pos.x][Pos.y].Sides;
    tileSmooth b_smoothing = (tileSmooth)(b[0] + b[2]*2 + b[4]*4 + b[6]*8);
    switch (b_smoothing)
    {
    case tileSmooth::roundLT:
        if (b[5])
            b_smoothing = tileSmooth::roundLTE;
        break;
    case tileSmooth::roundTR:
        if (b[7])
            b_smoothing = tileSmooth::roundTRE;
        break;
    case tileSmooth::roundBR:
        if (b[1])
            b_smoothing = tileSmooth::roundBRE;
        break;
    case tileSmooth::roundBL:
        if (b[3])
            b_smoothing = tileSmooth::roundBLE;
        break;
    case tileSmooth::center:
        if (b[1]&&b[3]&&b[5]&&b[7]) {
            b_smoothing = tileSmooth::centerEdge;
            break;
        }
        if (b[7]&&b[1]) {
            if (b[5]) {
                b_smoothing = tileSmooth::IroundTR;
                break;
            }
            if (b[3]) {
                b_smoothing = tileSmooth::IroundBR;
                break;
            }
            b_smoothing = tileSmooth::centerBLTL;
            break;
        }
        if (b[1]&&b[3]) {
            if (b[5]) {
                b_smoothing = tileSmooth::IroundBL;
                break;
            }
            b_smoothing = tileSmooth::centerTLTR;
            break;
        }
        if (b[3]&&b[5]) {
            if (b[7]) {
                b_smoothing = tileSmooth::IroundTL;
                break;
            }
            b_smoothing = tileSmooth::centerTRBR;
            break;
        }
        if (b[5]&&b[7]) {
            b_smoothing = tileSmooth::centerBRBL;
            break;
        }
        if (b[1]&&b[5]) {
            b_smoothing = tileSmooth::centerTLBR;
            break;
        }
        if (b[3]&&b[7]) {
            b_smoothing = tileSmooth::centerTRBL;
            break;
        }
        if (b[1])
            b_smoothing = tileSmooth::centerTL;
        if (b[3])
            b_smoothing = tileSmooth::centerTR;
        if (b[5])
            b_smoothing = tileSmooth::centerBR;
        if (b[7])
            b_smoothing = tileSmooth::centerBL;
        break;
    case tileSmooth::left:
        if (b[3] && b[5]) {
            b_smoothing = tileSmooth::leftBRTR;
            break;
        }
        if (b[3])
            b_smoothing = tileSmooth::leftTR;
        if (b[5])
            b_smoothing = tileSmooth::leftBR;
        break;
    case tileSmooth::right:
        if (b[1]&&b[7]) {
            b_smoothing = tileSmooth::rightBLTL;
            break;
        }
        if (b[1])
            b_smoothing = tileSmooth::rightTL;
        if (b[7])
            b_smoothing = tileSmooth::rightBL;
        break;
    case tileSmooth::top:
        if (b[5]&&b[7]) {
            b_smoothing = tileSmooth::topBRBL;
            break;
        }
        if (b[5])
            b_smoothing = tileSmooth::topBR;
        if (b[7])
            b_smoothing = tileSmooth::topBL;
        break;
    case tileSmooth::bottom:
        if (b[1]&&b[3]) {
            b_smoothing = tileSmooth::bottomTLTR;
            break;
        }
        if (b[1])
            b_smoothing = tileSmooth::bottomTL;
        if (b[3])
            b_smoothing = tileSmooth::bottomTR;
        break;
    default:
        break;
    }
    map[Pos.x][Pos.y].Smooth = b_smoothing;
    return (float)b_smoothing;
}

basic::Vector2i getImageCoords(int width, int height, int coord) {
    float b_int = (float)coord/width;
    return basic::Vector2i((int)(((b_int-floor(b_int))*width))*height, (int)(floor(b_int))*height);
}

void map::world::draw(SDL_Renderer* renderer, basic::Vector2i Pos, ply::player* player) {
    SDL_Rect c ={ map[Pos.x][Pos.y].GamePosition.x, map[Pos.x][Pos.y].GamePosition.y, 80, 80 };
    switch (map[Pos.x][Pos.y].Type)
    {
    case map::tileType::DIRT: {
        float b_int = getSmooth(Pos);
        basic::Vector2i Coords = getImageCoords(4, 16, b_int);
        tileTextures[0].imageCoords ={ Coords.x,Coords.y,16,16 };
        tileTextures[0].screenCoords = c;
        tileTextures[0].drawTexture(renderer);
        break;
    }
    case map::tileType::GRASS: {
        basic::Vector2i Coords = getImageCoords(4, 16, map[Pos.x][Pos.y].randomNumber);
        tileTextures[1].imageCoords ={ Coords.x,Coords.y,16,16 };
        tileTextures[1].screenCoords = c;
        tileTextures[1].drawTexture(renderer);
        break;
    }
    default:
        break;
    }
    //---------Rend Any Trees that are to the left---------
    //if (map[Pos.x-1][Pos.y].Type == map::tileType::TREE) {
    //    basic::Vector2i Coords = getImageCoords(4, 16, 0);
    //    tileTextures[2].imageCoords ={ Coords.x, Coords.y,48,48 };
    //    tileTextures[2].screenCoords ={ map[Pos.x-1][Pos.y].GamePosition.x-80, map[Pos.x-1][Pos.y].GamePosition.y-160, 240,240 };
    //    tileTextures[2].drawTexture(renderer);
    //}

}

void setTextures(SDL_Renderer* renderer) {
    std::string path = "Images/";
    for (int i = 0; i < map::textureAmount; i++) {
        tileTextures[i].loadMedia(renderer, path + tileFile[i]);
    }
}

void map::init(world* world, SDL_Renderer* renderer) {
    srand(time(NULL));
    siv::PerlinNoise p(std::rand());
    setTextures(renderer);
    for (int y = 0; y < map::sizeY; y++)
    for (int x = 0; x < map::sizeX; x++) {
        //-------Get Sides--------
        world->getSides(basic::Vector2i(x, y));
        world->map[x][y].Position ={ x*80, y*80 };
        //-------Set Random--------
        if (rand()%6 == 0)
            world->map[x][y].randomNumber = rand()%3;
        else
            world->map[x][y].randomNumber = 15;
        //-------Set Perlin--------
        double c = p.accumulatedOctaveNoise2D_0_1((float)x/map::sizeX, (float)y/map::sizeY, 100);
        world->map[x][y].perlinNumber = c;
    }
}