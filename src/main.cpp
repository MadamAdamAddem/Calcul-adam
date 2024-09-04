#include "include/main.h"


std::string basePath;

SDL_Texture* background = NULL;
SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;
TTF_Font* font = NULL;

SDL_Color textColor = {0, 0, 0};
calcStorage calculator;

Mouse cursor = {0, 0, -1, false};


const int SCREENW = 600;
const int SCREENH = 800;



int main()
{
    SDL_Event e;
    bool quit = false;

    initAll(SCREENW, SCREENH, &window, &renderer, &background, &font);


    std::string buttonsPath;
    buttonsPath.assign(basePath);
    buttonPath(&buttonsPath);
    
    std::string buttonSpecific;
    buttonSpecific.assign(buttonsPath);


    adamTexture adjustmentLayer;
    adamTexture adjustmentTextures[4];

    //loads adjustmentLayer textures
    {
        buttonSpecific.append("buttonBlank.png");
        adjustmentTextures[0].loadFromFile(buttonSpecific, renderer);
        adjustmentTextures[0].setBlendMode(SDL_BLENDMODE_BLEND);
        adjustmentTextures[0].setAlphaLevel(100);
        buttonSpecific.assign(buttonsPath);

        buttonSpecific.append("buttonDark2.png");
        adjustmentTextures[1].loadFromFile(buttonSpecific, renderer);
        adjustmentTextures[1].setBlendMode(SDL_BLENDMODE_BLEND);
        adjustmentTextures[1].setAlphaLevel(100);
        buttonSpecific.assign(buttonsPath);

        buttonSpecific.append("buttonTopAdjBlank.png");
        adjustmentTextures[2].loadFromFile(buttonSpecific, renderer);
        adjustmentTextures[2].setBlendMode(SDL_BLENDMODE_BLEND);
        adjustmentTextures[2].setAlphaLevel(100);
        buttonSpecific.assign(buttonsPath);

        buttonSpecific.append("buttonTopAdjDark.png");
        adjustmentTextures[3].loadFromFile(buttonSpecific, renderer);
        adjustmentTextures[3].setBlendMode(SDL_BLENDMODE_BLEND);
        adjustmentTextures[3].setAlphaLevel(100);
    }


    
    calculator.loadText("0", textColor, renderer, font);
    calculator.loadText("0", textColor, renderer, font, false);

    //just checks if prevInput was divide
    int prevInput = 0;

    //used for button animation
    int frames = ANIMATION_FRAMES;


    //only 4 lines of code :0 how simple!
    while(handleEvents(&e, &prevInput)) {
        doRendering(&adjustmentLayer, adjustmentTextures, prevInput, &frames);
        sleepThread(16);
        SDL_RenderPresent(renderer);
    }





    aSDL_Close();
    return 0;
}
