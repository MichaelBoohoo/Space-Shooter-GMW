#include "game.h"

int main()
{
    // Tworzymy instancję gry
    game gra;

    // ============================
    // PĘTLA GRY
    // ============================
    while (gra.OknoOtwarte())
    {
        // Aktualizacja logiki gry
        gra.update();

        // Render wszystkich obiektów
        gra.render();
    }

    // Koniec programu
    return 0;
}

