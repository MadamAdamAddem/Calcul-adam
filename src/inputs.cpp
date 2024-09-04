#include "include/main.h"


int easteregg = 0;



bool handleEvents(SDL_Event* e, int* prevInput)
{

    while(SDL_PollEvent(e) != 0)
    {
        //prevents a button being pressed multiple times with one input
        bool tmpClick = false;

        //big ol exit
        if(e->type == SDL_QUIT)
            return false;

        if(e->type == SDL_MOUSEMOTION)
        {   
            SDL_GetMouseState(&cursor.xPos, &cursor.yPos);
            cursor.onButton = -1;

            //Row 5 (Bottom)
            if(cursor.yPos >= 650 && cursor.yPos <= 770)
            {
                if(cursor.xPos >= 40 && cursor.xPos <= 160)
                    cursor.onButton = BUTTON0;
                else if(cursor.xPos >= 174 && cursor.xPos <= 294)
                    cursor.onButton = BUTTON_DOT;
                else if(cursor.xPos >= 308 && cursor.xPos <= 428)
                    cursor.onButton = BUTTON_EQ;
                else if(cursor.xPos >= 442 && cursor.xPos <= 562)
                    cursor.onButton = BUTTON_PLUS;
            }

            //Row 4
            else if(cursor.yPos >= 520 && cursor.yPos <= 640)
            {
                if(cursor.xPos >= 40 && cursor.xPos <= 160)
                    cursor.onButton = BUTTON1;
                else if(cursor.xPos >= 174 && cursor.xPos <= 294)
                    cursor.onButton = BUTTON2;
                else if(cursor.xPos >= 308 && cursor.xPos <= 428)
                    cursor.onButton = BUTTON3;
                else if(cursor.xPos >= 442 && cursor.xPos <= 562)
                    cursor.onButton = BUTTON_MIN;
            }

            //Row 3
            else if(cursor.yPos >= 390 && cursor.yPos <= 510)
            {
                if(cursor.xPos >= 40 && cursor.xPos <= 160)
                    cursor.onButton = BUTTON4;
                else if(cursor.xPos >= 174 && cursor.xPos <= 294)
                    cursor.onButton = BUTTON5;
                else if(cursor.xPos >= 308 && cursor.xPos <= 428)
                    cursor.onButton = BUTTON6;
                else if(cursor.xPos >= 442 && cursor.xPos <= 562)
                    cursor.onButton = BUTTON_MUL;
            }

            //Row 2
            else if(cursor.yPos >= 260 && cursor.yPos <= 380)
            {
                if(cursor.xPos >= 40 && cursor.xPos <= 160)
                    cursor.onButton = BUTTON7;
                else if(cursor.xPos >= 174 && cursor.xPos <= 294)
                    cursor.onButton = BUTTON8;
                else if(cursor.xPos >= 308 && cursor.xPos <= 428)
                    cursor.onButton = BUTTON9;
                else if(cursor.xPos >= 442 && cursor.xPos <= 562)
                    cursor.onButton = BUTTON_DIV;
            }

            //Row 1 (Top)
            else if(cursor.yPos >= 207 && cursor.yPos <= 245)
            {
                if(cursor.xPos >= 45 && cursor.xPos <= 157)
                    cursor.onButton = BUTTON_SQ;
                else if(cursor.xPos >= 179 && cursor.xPos <= 291)
                    cursor.onButton = BUTTON_C;
                else if(cursor.xPos >= 313 && cursor.xPos <= 425)
                    cursor.onButton = BUTTON_PLMN;
                else if(cursor.xPos >= 445 && cursor.xPos <= 557)
                    cursor.onButton = BUTTON_BACK;
            }

        }

        if(e->type == SDL_MOUSEBUTTONDOWN)
        {
            cursor.isClicked = true;
            tmpClick = true;
        }
            

        if(e->type == SDL_MOUSEBUTTONUP)
            cursor.isClicked = false;

        //translates cursor click to keyboard input
        if(cursor.onButton != -1 && tmpClick)
        {
            e->type = SDL_KEYDOWN;
            
            switch(cursor.onButton)
            {
                case BUTTON0:
                    e->key.keysym.sym = SDLK_0;
                    break;

                case BUTTON1:
                    e->key.keysym.sym = SDLK_1;
                    break;
                
                case BUTTON2:
                    e->key.keysym.sym = SDLK_2;
                    break;

                case BUTTON3:
                    e->key.keysym.sym = SDLK_3;
                    break;

                case BUTTON4:
                    e->key.keysym.sym = SDLK_4;
                    break;
                
                case BUTTON5:
                    e->key.keysym.sym = SDLK_5;
                    break;
                
                case BUTTON6:
                    e->key.keysym.sym = SDLK_6;
                    break;

                case BUTTON7:
                    e->key.keysym.sym = SDLK_7;
                    break;

                case BUTTON8:
                    e->key.keysym.sym = SDLK_8;
                    break;

                case BUTTON9:
                    e->key.keysym.sym = SDLK_9;
                    break;

                case BUTTON_DIV:
                    e->key.keysym.sym = SDLK_i;
                    break;
                
                case BUTTON_MUL:
                    e->key.keysym.sym = SDLK_u;
                    break;

                case BUTTON_MIN:
                    e->key.keysym.sym = SDLK_o;
                    break;

                case BUTTON_PLUS:
                    e->key.keysym.sym = SDLK_y;
                    break;

                case BUTTON_DOT:
                    e->key.keysym.sym = SDLK_PERIOD;
                    break;
                
                case BUTTON_EQ:
                    e->key.keysym.sym = SDLK_EQUALS;
                    break;

                case BUTTON_BACK:
                    e->key.keysym.sym = SDLK_BACKSPACE;
                    break;

                case BUTTON_C:
                    e->key.keysym.sym = SDLK_c;
                    break;

                case BUTTON_SQ:
                    e->key.keysym.sym = SDLK_p;
                    break;

                case BUTTON_PLMN:
                    e->key.keysym.sym = SDLK_MINUS;
                    break;
            }
        }


        //Accepts 0-9, Backspace, Del, c, y, u, i, o, p, =
        if(e->type == SDL_KEYDOWN)
        {   
            if(*prevInput == RESET)
            {
                calculator.initialize(true);
                *prevInput = 0;
            }

            switch(e->key.keysym.sym)
            {
                case SDLK_0:
                case SDLK_1:
                case SDLK_2:
                case SDLK_3:
                case SDLK_4:
                case SDLK_5:
                case SDLK_6:
                case SDLK_7:
                case SDLK_8:
                case SDLK_9:
                case SDLK_PERIOD:
                case SDLK_MINUS:

                    calculator.adjustDigits(e->key.keysym.sym);
                    break;

                case SDLK_BACKSPACE:
                case SDLK_DELETE:
                    
                    calculator.adjustDigits();
                    break;

                case SDLK_c:

                    calculator.initialize();
                    *prevInput = 0;
                    break;

                case SDLK_y:

                    calculator.doMathAndUpdate(ADD);
                    *prevInput = 0;
                    break;

                case SDLK_u:

                    calculator.doMathAndUpdate(MULTIPLY);
                    *prevInput = 0;
                    break;

                case SDLK_i:
 
                    calculator.doMathAndUpdate(DIVIDE);
                    *prevInput = DIVIDE;
                    break;
                    
                case SDLK_o:

                    calculator.doMathAndUpdate(SUBTRACT);
                    *prevInput = 0;
                    break;

                case SDLK_p:
                    
                    calculator.doMathAndUpdate(SQUARE);
                    *prevInput = 0;

                case SDLK_k:

                    //easter egg
                    if(cursor.onButton == BUTTON_DOT)
                        easteregg = 1;
                        

                case SDLK_EQUALS:

                    //prevent divide by zero
                    if(calculator.getLorR() == RIGHT && calculator.isZero() == true && *prevInput == DIVIDE)
                    { 
                        calculator.flipLorR();

                        if(!easteregg)
                            calculator.loadText("NaN", textColor, renderer, font);
                        else
                            calculator.loadText("DAMN.", textColor, renderer, font);
                            

                        *prevInput = RESET;
                        break;
                    }

                    calculator.doMathAndUpdate(EQUALS);
                    *prevInput = 0;
                    break;

            }
            
            if(*prevInput != RESET)
                calculator.loadText(calculator.returnActiveDigits(), textColor, renderer, font);
            
            calculator.loadText(calculator.returnLeftDigits(), textColor, renderer, font, false);
            
        }



        

        
    }
    
    return true;
}






















