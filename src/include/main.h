#pragma once
#include <iostream>
#include <stdio.h>
#include <stdlib.h>

#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <mpfr.h>

#define FLOAT_PRECISION 256
#define ANIMATION_FRAMES 7
#define ROUNDING_MODE MPFR_RNDN


//i'd go on a date with extern if i could
extern SDL_Color textColor;
extern SDL_Renderer* renderer;
extern SDL_Window* window;
extern TTF_Font* font;
extern SDL_Texture* background;
extern std::string basePath;


enum leftOrRight
{
    LEFT = false,
    RIGHT = true
};

enum mathMode
{
    ADD = '+',
    SUBTRACT = '-',
    DIVIDE = '/',
    MULTIPLY = 'x',
    EQUALS = '=',
    SQUARE = '^',
    INACTIVE = 0,
    RESET = -1
};

enum screenButtons
{
    BUTTON0 = 0, BUTTON1, BUTTON2, BUTTON3, BUTTON4, BUTTON5,
    BUTTON6, BUTTON7, BUTTON8, BUTTON9, BUTTON_DIV, BUTTON_MUL,
    BUTTON_MIN, BUTTON_PLUS, BUTTON_DOT, BUTTON_EQ, BUTTON_SQ,
    BUTTON_C, BUTTON_PLMN, BUTTON_BACK
};

//Used for rendering
enum buttonsPos
{
    ROW5 = 648, ROW4 = 518, ROW3 = 388, ROW2 = 258, ROW1 = 207,
    COL1 = 38, COL2 = 172, COL3 = 306, COL4 = 440
};










class adamTexture
{
public:

    adamTexture()
    {
        texture = NULL;
        tWidth = 0;
        tHeight = 0;
    }
    
    ~adamTexture()
    {
        free();
    }

    void free()
    {
        if(texture == NULL) return;

        SDL_DestroyTexture(texture);
        tWidth = 0;
        tHeight = 0;
    }
    
    bool loadFromFile(std::string path, SDL_Renderer* renderer)
    {
        free();

        SDL_Texture* tmpTexture = NULL;

        SDL_Surface* tmpSurface = IMG_Load(path.c_str());
        if(tmpSurface == NULL) 
        {
            std::cout << "Failed to load surface for texture, " << IMG_GetError() << std::endl;
            return false;
        }

        SDL_SetColorKey(tmpSurface, SDL_TRUE, SDL_MapRGB(tmpSurface->format, 0, 0xFF, 0xFF));

        tmpTexture = SDL_CreateTextureFromSurface(renderer, tmpSurface);
        if(tmpTexture == NULL)
        {
            std::cout << "Failed to create texture from surface" << std::endl;
            SDL_FreeSurface(tmpSurface);
            return false;
        }

        tWidth = tmpSurface->w;
        tHeight = tmpSurface->h;
        texture = tmpTexture;

        SDL_FreeSurface(tmpSurface);
        SDL_SetTextureScaleMode(tmpTexture, SDL_ScaleModeBest);
        return true;

    }

    bool loadFromText(char* text, SDL_Color textColor, SDL_Renderer* renderer, TTF_Font* font)
    {
        free();

        //SDL_Color bgColor = {0,0,0,0};
        
        SDL_Surface* tmpSurface = TTF_RenderUTF8_Blended(font, text, textColor);

        if(tmpSurface == NULL)
        {
            std::cout << "Text failure\n" << std::endl;
            return false;
        }

        texture = SDL_CreateTextureFromSurface(renderer, tmpSurface);
        if(texture == NULL)
        {
            std::cout << "Text texture failure\n" << std::endl;
            return false;
        }

        tWidth = tmpSurface->w;
        tHeight = tmpSurface->h;

        SDL_FreeSurface(tmpSurface);
        SDL_SetTextureScaleMode(texture, SDL_ScaleModeBest);
        
        return true;
    }

    void render(int x, int y, SDL_Renderer* renderer, float stretchFactor = 1, SDL_Color colorMod = {0,0,0,0}, SDL_Rect* clip = NULL)
    {
        //area to render
        SDL_Rect renderSpace = {x, y, tWidth*stretchFactor, tHeight*stretchFactor};
        if(clip != NULL)
        {
            renderSpace.w = clip->w;
            renderSpace.h = clip->h;
        }

        if(colorMod.a != 0)
            SDL_SetTextureColorMod(texture, colorMod.r, colorMod.g, colorMod.b);
        
        SDL_RenderCopy(renderer, texture, clip, &renderSpace);
    }

    void setBlendMode(SDL_BlendMode blendMode)
    {
        SDL_SetTextureBlendMode(texture, blendMode);
    }

    void setAlphaLevel(Uint8 alpha)
    {
        SDL_SetTextureAlphaMod(texture, alpha);
    }

    void replaceTexture(adamTexture* newTexture)
    {
        texture = newTexture->texture;
        tWidth = newTexture->tWidth;
        tHeight = newTexture->tHeight;
    }

    int getWidth() {return tWidth;}
    int getHeight() {return tHeight;}


private:

    SDL_Texture* texture;
    int tWidth;
    int tHeight;

};

class calcNumber
{
public:

    friend class calcStorage;

    calcNumber()
    {
        mpfr_set_default_prec(FLOAT_PRECISION);
        mpfr_init_set_si(internalNumber, 0, MPFR_RNDU);
        reset();
    }

    ~calcNumber()
    {
        mpfr_clear(internalNumber);
    }

    void reset()
    {
        char reset[14] = {' ', '0', '\0', '\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0'};
        strcpy(stringDigits, reset);

        mpfr_set_ui(internalNumber, 0, MPFR_RNDN);
    }

    char* returnString()
    {
        return stringDigits;
    }

    void removeDigit()
    {
        //deletes the last character,
        //and period if it is left hanging
        int i;
        for(i=1; stringDigits[i] != '\0'; ++i)
        {
            if(stringDigits[i+1] == '\0')
            {
                stringDigits[i] = '\0';
                if(stringDigits[i-1] == '.')
                    stringDigits[i-1] = '\0';

                break;
            }
        }

        if(stringDigits[1] == '\0')
            reset();
    }

    void addDigit(char digit)
    {
        bool isZero = (stringDigits[1] == '0' && stringDigits[2] == '\0');

        //if the number is 0 and the digit being added is also 0 or -, exit
        if((digit == '0' || digit == '-' ) && isZero)
            return;
        if(isZero && digit != '.' && digit != '-')
        {
            stringDigits[1] = digit;
            return;
        }

        if(isZero && digit == '.')
        {
            stringDigits[2] = '.';
            stringDigits[3] = '\0';
            return;
        }

        //toggle negative
        if(!isZero && digit == '-')
        {
            if(stringDigits[0] == '-')
                stringDigits[0] = ' ';
            else
                stringDigits[0] = '-';

            return;
        }


        int i;
        bool hasDecimal = false;
        for(i=1; (stringDigits[i-1] != '\0') && (i<(hasDecimal ? 13 : 12)); ++i)
        {

            if(stringDigits[i] == '.')
                hasDecimal = true;

            //makes sure two .s don't get added
            if(digit == '.' && stringDigits[i] == '.')
                break;

            if(stringDigits[i] == '\0')
            {
                stringDigits[i] = digit;
                stringDigits[i+1] = '\0';
                break;
            }
        }


    }

    //update string to reflect internalNumber
    void stringify()
    {

        //if number can't be represented by 11 digits put in scientific notation
        if((mpfr_cmp_ui(internalNumber, 99999999999)) < 1)
            mpfr_snprintf(stringDigits, 14, "% .*RNf", 10, internalNumber);
        else
            mpfr_snprintf(stringDigits, 14, "% .5Re", internalNumber); //supports numbers up to 10^999, any more is probably not necessary but possible by lowering .5


        formatString();

    }

    void formatString()
    {
        
        //removes lonely period
        if(stringDigits[12] == '.')
            stringDigits[12] = '\0';

        //removes trailing zeroes
        int i;
        int lastNum = 0;
        int pIndex = 2;
        bool isScientific = 0;

        for(i=0; stringDigits[i] != '.' && stringDigits[i] != '\0'; ++i) {}

        pIndex = i;


        //removes trailing zeroes
        for( ; stringDigits[i] != '\0'; ++i)
        {
            if(stringDigits[i] == 'e')
                isScientific = true;

            if(stringDigits[i] != '.' && stringDigits[i] != '0')
                lastNum = i;
        }

        //only formates if not scientific, and only removes the zeroes past the decimal point
        if(!isScientific)
        {
            if(lastNum+1 > pIndex)
                stringDigits[lastNum+1] = '\0';
            else
                stringDigits[pIndex] = '\0';
        }
            

    }

    //update internalNumber to reflect string
    void nummify()
    {
        mpfr_set_str(internalNumber, stringDigits, 10, ROUNDING_MODE);
    }

private:

    //11 digits,
    //one sign, one decimal, and one \0, 
    char stringDigits[14];

    mpfr_t internalNumber;
};

class calcStorage
{
public:

    

    calcStorage()
    {
        initialize();
    }

    ~calcStorage()
    {

    }

    void initialize(bool reset = false)
    {
        leftOrRight = LEFT;
        prevOperation = INACTIVE;
        midEquation = false;

        if(reset)
            calcText.loadFromText("0", textColor, renderer, font);


        numbers[0].reset();
        numbers[1].reset();
        oldNum.reset();
    }

    char* returnActiveDigits()
    {
        return numbers[leftOrRight].returnString();
    }

    char* returnLeftDigits()
    {
        return numbers[LEFT].returnString();
    }

    bool isZero()
    {
        if(numbers[leftOrRight].stringDigits[1] == '0' && numbers[leftOrRight].stringDigits[2] == '\0')
            return true;

        return false;
    }

    void adjustDigits(int input = 0)
    {
        //if input is backspace or delete (0)
        if(!input)
        {
            numbers[leftOrRight].removeDigit();
            numbers[leftOrRight].nummify();
        }
            
        else
        {
            numbers[leftOrRight].addDigit(input);
        }

        return;
    }

    bool getLorR()
    {
        return leftOrRight;
    }

    void flipLorR()
    {
        leftOrRight = !leftOrRight;
    }
    
    //All the funny variables and logic is there to make the calculations more fluid
    //Allows for chain commands (Ex: 1 + 3 / 2 * 5 = 10)
    //Saves answer after = (Ex: 2 + 2 = 4 * 3 = 12)
    void doMathAndUpdate(int mathMode)
    {
        

        bool doChainCommand = (prevOperation != INACTIVE);
        int newCommand = mathMode;

        //only write to left number on first operation, or after clear (c)
        if(!midEquation)
            numbers[LEFT].nummify();

        //bug fix to ensure number flips its sign when '-' is input
        if(numbers[leftOrRight].stringDigits[0] == '-')
            mpfr_setsign(numbers[leftOrRight].internalNumber, numbers[leftOrRight].internalNumber, 1, ROUNDING_MODE);
        else
            mpfr_setsign(numbers[leftOrRight].internalNumber, numbers[leftOrRight].internalNumber, 0, ROUNDING_MODE);
            

        numbers[RIGHT].nummify();


        if(doChainCommand)
            mathMode = prevOperation;

        


        switch(mathMode)
        {
            case INACTIVE:
                break;
            
            case ADD:

                mpfr_add(numbers[LEFT].internalNumber, numbers[LEFT].internalNumber, numbers[RIGHT].internalNumber, ROUNDING_MODE);
                prevOperation = newCommand;
                break;

            case MULTIPLY:

                //prevents accidentally multiplying by zero before wanted
                if(prevOperation == MULTIPLY)
                    mpfr_mul(numbers[LEFT].internalNumber, numbers[LEFT].internalNumber, numbers[RIGHT].internalNumber, ROUNDING_MODE);

                prevOperation = newCommand;
                break;

            case DIVIDE:

                //prevents dividing by zero when definitely wanted
                if(mpfr_zero_p(numbers[RIGHT].internalNumber) && prevOperation == DIVIDE)
                {
                    numbers[LEFT].reset();
                    numbers[RIGHT].reset();
                }

                //prevents accidentally dividing by zero before wanted (lol?)
                if(prevOperation == DIVIDE)
                    mpfr_div(numbers[LEFT].internalNumber, numbers[LEFT].internalNumber, numbers[RIGHT].internalNumber, ROUNDING_MODE);

                    
                prevOperation = newCommand;
                break;

            case SUBTRACT:

                mpfr_sub(numbers[LEFT].internalNumber, numbers[LEFT].internalNumber, numbers[RIGHT].internalNumber, ROUNDING_MODE);
                prevOperation = newCommand;
                break;

            case SQUARE:

                mpfr_sqr(numbers[LEFT].internalNumber, numbers[LEFT].internalNumber, ROUNDING_MODE);
                break;
        }

        numbers[LEFT].stringify();
        numbers[RIGHT].reset();
        leftOrRight = RIGHT;

        if((!midEquation && mathMode == EQUALS))
            midEquation = false;
        else
            midEquation = true;

        //mpfr_printf("Left: %.16Rf\n", numbers[LEFT].internalNumber);

        mpfr_set(oldNum.internalNumber, numbers[LEFT].internalNumber, ROUNDING_MODE);
        oldNum.stringify();

        if(newCommand == EQUALS)
        {
            prevOperation = INACTIVE;
            leftOrRight = LEFT;
        }
        
    }

    void loadText(char* text, SDL_Color textColor, SDL_Renderer* renderer, TTF_Font* font, bool loadMain = true)
    {
        if(loadMain)
            calcText.loadFromText(text, textColor, renderer, font);
        else
            oldNumText.loadFromText(text, textColor, renderer, font);
    }

    void renderText(int x, int y, SDL_Renderer* renderer, float stretchFactor = 1, bool renderMain = true, SDL_Color colorMod = {0,0,0,0}, SDL_Rect* clip = NULL)
    {
        if(renderMain)
            calcText.render(x, y, renderer, stretchFactor, colorMod, clip);
        else
            oldNumText.render(x, y, renderer, stretchFactor, colorMod, clip);

    }

    int getTextWidth(bool useMainValue = true)
    {
        if(useMainValue)
            return calcText.getWidth();
        else
            return oldNumText.getWidth();
        
    }

    int getTextHeight()
    {
        return calcText.getHeight();
    }

private:


    calcNumber numbers[2];
    adamTexture calcText;

    adamTexture oldNumText;
    calcNumber  oldNum;

    bool leftOrRight;
    bool midEquation;
    int prevOperation;
};

typedef struct Mouse
{
    int xPos;
    int yPos;
    
    char onButton;
    bool isClicked;

}Mouse;





//James William Bottomtooth the Third if he was made out of function declarations:


void aSDL_Close();
SDL_Renderer* ainitRenderer(SDL_Window* window, bool vsync = false);
SDL_Window* ainitWindow(std::string windowName, int SCREEN_WIDTH, int SCREEN_HEIGHT);
bool handleEvents(SDL_Event* e, int* prevInput);
SDL_Texture* loadBackground();
void sleepThread(unsigned ms);
void buttonPath(std::string* buttonString);
void fontPath(std::string* fontString);
void backgroundPath(std::string* backgroundString);
void initAll(int SCREENW, int SCREENH, SDL_Window** window, SDL_Renderer** renderer, SDL_Texture** texture, TTF_Font** font);
void doRendering(adamTexture* adjustmentLayer, adamTexture* adjustmentTextures, int prevInput, int* frames);




extern Mouse cursor;
extern calcStorage calculator;


