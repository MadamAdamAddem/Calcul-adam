#include "include/main.h"





//Initiates SDL, SDL_image, and SDL_TTF with a window
SDL_Window* ainitWindow(std::string windowName, int SCREEN_WIDTH, int SCREEN_HEIGHT)
{
    SDL_Window* tempWindow = NULL;
    if(SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        printf("SDL initialization failed\n");
        return NULL;
    }
    

    //Set texture filtering to linear
	if( !SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1" ) )
	{
		printf( "Linear texture filtering not enabled!\n" );
		return NULL;
	}

    tempWindow = SDL_CreateWindow(windowName.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);

    if(tempWindow == NULL)
    {
        printf("Window initialization failed\n");
        return NULL;
    } 
    int imgFlags = IMG_INIT_PNG;

    if(!(IMG_Init(imgFlags) & imgFlags))
    {
        printf("SDL_image initialization failed\n");
        return NULL;
    }

    //Initialize SDL_ttf
    if(TTF_Init() == -1)
    {
        printf( "SDL_ttf initialization failed\n");
        return NULL;
    }
    

    return tempWindow;
}

//Initiates a renderer with vsync optional
SDL_Renderer* ainitRenderer(SDL_Window* window, bool vsync)
{
    SDL_Renderer* tempRenderer = NULL;
    if(vsync)
        tempRenderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    else
        tempRenderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    if(tempRenderer == NULL)
    {
        std::cout << "Renderer Error" << SDL_GetError() << std::endl;
        return NULL;
    }

    SDL_SetRenderDrawColor(tempRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
    return tempRenderer;
}


SDL_Texture* loadBackground()
{
    std::string tmpAppend;
    tmpAppend.assign(basePath);
    backgroundPath(&tmpAppend);


    SDL_Surface* tmpSurface = IMG_Load(tmpAppend.c_str());
    if(tmpSurface == NULL)
    {
        std::cout << "Failed to load CalcBG" << std::endl;
        return NULL;
    }


    SDL_Texture* background = SDL_CreateTextureFromSurface(renderer, tmpSurface);
    if(background == NULL)
    {
        std::cout << "Failed to create BG from surface" << std::endl;
        return NULL;
    }

    SDL_FreeSurface(tmpSurface);
    return background;
}

void initAll(int SCREENW, int SCREENH, SDL_Window** window, SDL_Renderer** renderer, SDL_Texture** texture, TTF_Font** font)
{
    std::string tmpAppend;
    basePath = SDL_GetBasePath();
    tmpAppend.assign(basePath);
    tmpAppend.append("src/assets/calculator.ttf");

    *window = ainitWindow("Calcul-adam", SCREENW, SCREENH);
    if(window == NULL)
        exit(1);

    *renderer = ainitRenderer(*window);
    if(renderer == NULL)
        exit(1);

    *texture = loadBackground();
    if(background == NULL)
        exit(1);

    TTF_SetFontHinting(*font, TTF_HINTING_LIGHT_SUBPIXEL);
    *font = TTF_OpenFont(tmpAppend.c_str(), 100);
    if(font == NULL)
        exit(1);

}

void aSDL_Close()
{
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_DestroyTexture(background);
    
    SDL_Quit();
    IMG_Quit();
    TTF_Quit();
}





