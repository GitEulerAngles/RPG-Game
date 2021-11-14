#include "window.hpp"
#include "SDL_image.h"
#include "SDL_ttf.h"
#include "mapData.h"
#include "playerData.h"


ply::player* player = new ply::player;
map::world* world = new map::world;
basic::texture background;

bool left;
bool right;
bool middle;

void window::input() {
    SDL_Event e;
    while (SDL_PollEvent(&e)) {
        if (e.type == SDL_WINDOWEVENT) {
            switch (e.window.event) {
            case SDL_WINDOWEVENT_CLOSE:
                running = false;
                break;
            }
        }
        if (e.type == SDL_MOUSEBUTTONDOWN) {
            switch (e.button.button) {
            case SDL_BUTTON_MIDDLE:
                middle = true;
                break;
            case SDL_BUTTON_LEFT:
                left = true;
                break;
            case SDL_BUTTON_RIGHT:
                right = true;
                break;
            }
        }
        if (e.type == SDL_MOUSEBUTTONUP) {
            switch (e.button.button) {
            case SDL_BUTTON_MIDDLE:
                middle = false;
                break;
            case SDL_BUTTON_LEFT:
                left = false;
                break;
            case SDL_BUTTON_RIGHT:
                right = false;
                break;
            }
        }
    }

    const Uint8* keystates = SDL_GetKeyboardState(NULL);
    if (keystates[SDL_SCANCODE_W]) {
        player->Position.y -= ply::speed;
    }
    if (keystates[SDL_SCANCODE_S]) {
        player->Position.y += ply::speed;
    }
    if (keystates[SDL_SCANCODE_A]) {
        player->Position.x -= ply::speed;
    }
    if (keystates[SDL_SCANCODE_D]) {
        player->Position.x += ply::speed;
    }
    if (keystates[SDL_SCANCODE_O]) {
        world->save();
    }
    if (keystates[SDL_SCANCODE_L]) {
        world->load();
    }
}
void window::update() {
    for (int y = 0; y < map::sizeY; y++)
    for (int x = 0; x < map::sizeX; x++) {
        world->map[x][y].GamePosition = subVector2i(world->map[x][y].Position, player->Position);
    }

    basic::Vector2i mouse_pos;
    basic::Vector2i mouse_togame;

    SDL_GetMouseState(&mouse_pos.x, &mouse_pos.y);

    mouse_togame.x = floor((mouse_pos.x + player->Position.x)/80);
    mouse_togame.y = floor((mouse_pos.y + player->Position.y)/80);

    if (map::isTileInWorld(mouse_togame)) {
        if (left) {
            world->map[mouse_togame.x][mouse_togame.y].randomNumber = 15;
            world->map[mouse_togame.x][mouse_togame.y].Type = map::tileType::GRASS;
            world->getSides(mouse_togame);
        }
        if (right) {
            world->map[mouse_togame.x][mouse_togame.y].Type = map::tileType::TREE;
            world->getSides(mouse_togame);
        }
        if (middle) {
            world->map[mouse_togame.x][mouse_togame.y].Type = map::tileType::DIRT;
            world->getSides(mouse_togame);
        }
    }
}
void window::render() {
    SDL_SetRenderDrawColor(renderer, 144, 210, 114, 255);
    SDL_RenderClear(renderer);

    int windowPosX = floor((player->Position.x)/80);
    int windowPosY = floor((player->Position.y)/80);

    int windowMaxX = windowPosX + ceil(1920/80) + 2;
    int windowMaxY = windowPosY + ceil(1080/80) + 2;

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    for (int y = windowPosY; y < windowMaxY; y++)
    for (int x = windowPosX; x < windowMaxX; x++) {
        if (map::isTileInWorld(basic::Vector2i(x, y))) {
            world->draw(renderer, basic::Vector2i(x, y), player);
            //---------Render Lines Between Tiles---------
            basic::Vector2i point = world->map[x][y].GamePosition;
            SDL_RenderDrawLine(renderer, point.x, point.y, point.x + 80, point.y);
            SDL_RenderDrawLine(renderer, point.x, point.y, point.x, point.y + 80);
        }
    }

    SDL_RenderPresent(renderer);
    SDL_UpdateWindowSurface(screen);
}
void window::setup() {
    if (IMG_Init(IMG_INIT_PNG) == -1) {
        std::cout << "SDL_image has failed to load!";
    }
    if (TTF_Init() == -1) {
        std::cout << "SDL_ttf has failed to load!";
    }

    background.loadMedia(renderer, "Images/Grass.png");
    background.imageCoords ={ 0,0,16,16 };
    map::init(world, renderer);
}
void window::clean() {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(screen);

    IMG_Quit();
    SDL_Quit();
    TTF_Quit();
}