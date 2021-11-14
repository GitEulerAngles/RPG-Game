#ifndef MAP
#define MAP

#include "SDL.h"
#include "playerData.h"


namespace map {
    const unsigned int sizeX = 100;
    const unsigned int sizeY = 100;
    const unsigned int textureAmount = 3;
    enum struct tileType {
        DIRT = 0,
        GRASS = 1,
        TREE = 2,
    };
    enum struct tileSmooth {
        center    = 0,
        left      = 1,
        top       = 2,
        roundLT   = 3,
        right     = 4,
        middleV   = 5,
        roundTR   = 6,
        roundLTR  = 7,
        bottom    = 8,
        roundBL   = 9,
        middleH   = 10,
        roundBLT  = 11,
        roundBR   = 12,
        roundLBR  = 13,
        roundTRB  = 14,
        round     = 15,
        leftTR    = 16,
        leftBR    = 17,
        leftBRTR  = 18,
        rightTL   = 19,
        rightBL   = 20,
        rightBLTL = 21,
        topBR     = 22,
        topBL     = 23,
        topBRBL   = 24,
        bottomTL  = 25,
        bottomTR  = 26,
        bottomTLTR= 27,
        centerEdge= 28,
        centerBLTL= 29,
        centerTLTR= 30,
        centerTRBR= 31,
        centerBRBL= 32,
        centerTL  = 33,
        centerTR  = 34,
        centerBL  = 35,
        centerBR  = 36,
        centerTLBR= 37,
        centerTRBL= 38,
        roundLTE  = 39,
        roundTRE  = 40,
        roundBRE  = 41,
        roundBLE  = 42,
        IroundTL  = 43,
        IroundTR  = 44,
        IroundBR  = 45,
        IroundBL  = 46
    };
    struct tile {
        int randomNumber;
        int perlinNumber;
        basic::Vector2f Offset;
        basic::Vector2i Dimensions;
        basic::Vector2i Position;
        basic::Vector2i GamePosition;
        bool     Sides[8];
        tileSmooth Smooth = tileSmooth::center;
        tileType Type = tileType::GRASS;
    };
    class world {
    public:
        tile map[sizeX][sizeY];
        void getSides(basic::Vector2i Pos);
        float getSmooth(basic::Vector2i Pos);
        void draw(SDL_Renderer* renderer, basic::Vector2i Pos, ply::player* player);
        void save();
        void load();
    private:
        void getSidesOfSides(basic::Vector2i Pos);
    };
    bool isTileInWorld(basic::Vector2i Pos);
    void init(world* World, SDL_Renderer* renderer);
}

#endif //!MAP
