#include <iostream>
#include "game.h"

int main()
{
    game gra;
   
    //game-loop
    while (gra.OknoOtwarte())
    {

        //update
        gra.update();

        //render
        gra.render();
    }
    //koniec progamu
	return 0;
}



