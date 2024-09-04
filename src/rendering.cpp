#include "include/main.h"

//credit to GManNickG || ad absurdum on stackoverflow
//compiler was giving me some trouble with #include <thread>
#ifdef _WIN32
    #include <windows.h>

    void sleepThread(unsigned ms)
    {
        Sleep(ms);
    }

    // Windows having backslashes instead of forward will always curse me
    void buttonPath(std::string* buttonString)
    {
        buttonString->append("src\\assets\\Buttons\\");
    }
    void backgroundPath(std::string* backgroundString)
    {
        backgroundString->append("src\\assets\\CalcFull.png");
    }
    void fontPath(std::string* fontString)
    {
        fontString->append("src\\assets\\calculator.ttf");
    }

#else
    #include <unistd.h>
    
    //takes microseconds
    void sleepThread(unsigned ms)
    {
        usleep(ms * 1000); 
    }
    void buttonPath(std::string* buttonString)
    {
        buttonString->append("src/assets/Buttons/");
    }
    void backgroundPath(std::string* backgroundString)
    {
        backgroundString->append("src/assets/CalcFull.png");
    }
    void fontPath(std::string* fontString)
    {
        fontString->append("src/assets/calculator.ttf");
    }

#endif






void doRendering(adamTexture* adjustmentLayer, adamTexture* adjustmentTextures, int prevInput, int* frames)
{
        
    SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
    SDL_RenderClear(renderer);

    SDL_RenderCopy(renderer, background, NULL, NULL);

    calculator.renderText(585 - calculator.getTextWidth(), 95, renderer);
    calculator.renderText(585 - calculator.getTextWidth(false)*0.5, 55, renderer, 0.5, false);
        


    //Selects which adjustment to put over the button 
    if(cursor.isClicked)
    {
        if(cursor.onButton < 16)
            adjustmentLayer->replaceTexture(&adjustmentTextures[1]);
        else
            adjustmentLayer->replaceTexture(&adjustmentTextures[3]);
    }
    else
    {
        if(cursor.onButton < 16)
            adjustmentLayer->replaceTexture(&adjustmentTextures[0]);
        else
            adjustmentLayer->replaceTexture(&adjustmentTextures[2]);
    }
        

    //Animation of texture fading in
    if(cursor.onButton != -1)
    {
        if(*frames > 1)
            --*frames;
    }
    else
        *frames = ANIMATION_FRAMES;
        

    adjustmentLayer->setAlphaLevel(90/ *frames);

    //Renders the adjustment layer over select button
    switch(cursor.onButton)
    {
        case BUTTON0:
            adjustmentLayer->render(COL1, ROW5, renderer, 0.40);
            break;

        case BUTTON1:
            adjustmentLayer->render(COL1, ROW4, renderer, 0.4);
            break;

        case BUTTON2:
            adjustmentLayer->render(COL2, ROW4, renderer, 0.4);
            break;

        case BUTTON3:
            adjustmentLayer->render(COL3, ROW4, renderer, 0.4);
            break;

        case BUTTON4:
            adjustmentLayer->render(COL1, ROW3, renderer, 0.4);
            break;  

        case BUTTON5:
            adjustmentLayer->render(COL2, ROW3, renderer, 0.4);
            break;

        case BUTTON6:
            adjustmentLayer->render(COL3, ROW3, renderer, 0.4);
            break;  
            
        case BUTTON7:
            adjustmentLayer->render(COL1, ROW2, renderer, 0.4);
            break;
            
        case BUTTON8:
            adjustmentLayer->render(COL2, ROW2, renderer, 0.4);
            break;
            
        case BUTTON9:
            adjustmentLayer->render(COL3, ROW2, renderer, 0.4);
            break;

        case BUTTON_DIV:
            adjustmentLayer->render(COL4, ROW2, renderer, 0.4);
            break;

        case BUTTON_MUL:
            adjustmentLayer->render(COL4, ROW3, renderer, 0.4);
            break;
            
        case BUTTON_MIN:
            adjustmentLayer->render(COL4, ROW4, renderer, 0.4);
            break;

        case BUTTON_PLUS:
            adjustmentLayer->render(COL4, ROW5, renderer, 0.4);
            break;  

        case BUTTON_EQ:
            adjustmentLayer->render(COL3, ROW5, renderer, 0.4);
            break;
            
        case BUTTON_DOT:
            adjustmentLayer->render(COL2, ROW5, renderer, 0.4);
            break;

        case BUTTON_SQ:
            adjustmentLayer->render(44, ROW1, renderer);
            break;

        case BUTTON_C:
            adjustmentLayer->render(178, ROW1, renderer);
            break;
        
        case BUTTON_PLMN:
            adjustmentLayer->render(312, ROW1, renderer);
            break;
        
        case BUTTON_BACK:
            adjustmentLayer->render(444, ROW1, renderer);
            break;

        case -1:
            break;
        }

}














