#include "game.h"
#include <cstdlib>  // dla rand()


// Save i load


void game::saveGame()
{
    // Zabezpieczenie: nie zapisuj, jeśli nick jest pusty
    if (gracz->getNick().empty()) return;

    // Tworzymy nazwę pliku na podstawie nicku, np. "Adam.txt"
    std::string fileName = gracz->getNick() + ".txt";

    std::ofstream file(fileName);
    if (file.is_open())
    {
        // Okreslenie kolejnosci wczytywania: Nick Punkty MaxHP SpeedLevel HPLevel ColorID BgID
        file << gracz->getNick() << " "
            << gracz->getPunkty() << " "
            << gracz->getHPMax() << " "
            << gracz->getSpeedLevel() << " "
            << gracz->getHpLevel() << " "
            << gracz->getSkinID() << " "
            << this->bgID;

        file.close();
        std::cout << "Zapisano gre do pliku: " << fileName << std::endl;
    }
}

void game::loadGame()
{
    // Pobieramy nick, który wpisał użytkownik w menu
    std::string nickToLoad = this->inputNick;
    if (nickToLoad.empty()) nickToLoad = "Gracz"; // jakby nie dzialalo

    std::string fileName = nickToLoad + ".txt";
    std::ifstream file(fileName);

    if (file.is_open())
    {
        std::string n;
        int pts, hpMx, spdLvl, hpLvl, skinID, bID;

        // Czytamy dane w tej samej kolejności
        if (file >> n >> pts >> hpMx >> spdLvl >> hpLvl >> skinID >> bID)
        {
            gracz->setNick(n);
            gracz->setPunkty(pts);
            gracz->setHpMax(hpMx);
            gracz->reset(); // Leczy HP do nowego Maxa

            // Resetujemy prędkość i nakładamy ulepszenia
            gracz->setSpeed(5.f + (spdLvl - 1) * 1.f);

            // Kolor i tło
            gracz->zmienSkorke(skinID);
            this->setBackground(bID);

            std::cout << "Wczytano profil: " << n << std::endl;
        }
        file.close();
    }
    else
    {
        // JEŚLI PLIK NIE ISTNIEJE -> NOWY GRACZ
        std::cout << "Brak zapisu dla gracza " << nickToLoad << ". Tworze nowy profil." << std::endl;

        // Ważne: Resetujemy gracza do zera, żeby nie miał statystyk z poprzedniej sesji
        gracz->resetAllStats();
        gracz->setNick(nickToLoad);

        // Resetujemy też tło na domyślne
        this->setBackground(0);
    }
}

void game::setBackground(int id)
{
    this->bgID = id;
    switch (id)
    {
    case 0: backgroundColor = sf::Color::Black; break;
    case 1: backgroundColor = sf::Color(50, 0, 50); break; // Ciemny fiolet
    case 2: backgroundColor = sf::Color(0, 0, 50); break;  // Ciemny granat
    case 3: backgroundColor = sf::Color(50, 20, 0); break; // Ciemny brąz
    default: backgroundColor = sf::Color::Black; break;
    }
}

void game::startDialog(std::vector<std::string> lines, std::string name)
{
    this->iloscDialog = lines;      // Kopiujemy teksty
    this->dialogName.setString(name);// Ustawiamy imię
    this->ktoryDialog = 0;      // Zaczynamy od początku

    // Ustawiamy tekst pierwszej linii
    if (!iloscDialog.empty()) {
        this->dialogText.setString(iloscDialog[0]);
        this->isDialogOn = true; // Włączamy tryb dialogu!
    }
}

// FUNKCJE PRYWATNE


void game::initZmienne()
{
    this->okno = nullptr;
    this->state = GameState::MENU;
    this->inputNick = "";
    this->bgID = 0;
    this->backgroundColor = sf::Color::Black;
    
}

void game::initOkno()
{
    this->okno = new sf::RenderWindow(sf::VideoMode(800, 600), "Space Shooter");
    this->okno->setFramerateLimit(80);
}

void game::initGracz()
{
    this->gracz = new Player(375.f, 520.f);
}


void game::initManagers()
{
    // Menedżery fal i power-upów
    waveManager = new WaveManager(enemies);
    powerUpManager = new PowerUpManager(
        powerUps,
        doubleShot,
        shield,
        shootCooldown,
        doubleShotTimer,
        fastShotTimer,
        shieldTimer,
        POWERUP_DURATION
    );
}


// HUD


void game::initHUD()
{
    // Ładowanie czcionki
    if (!font.loadFromFile("../Assets/arial.ttf")) {
        std::cout << "ERROR: Nie znaleziono arial.ttf" << std::endl;
    }

    powerUpText.setFont(font);
    powerUpText.setCharacterSize(14);
    powerUpText.setFillColor(sf::Color::White);

    powerUpBar.setSize({ 50.f, 5.f });
    powerUpBar.setFillColor(sf::Color::Green);

    if (!logoTexture.loadFromFile("../Assets/logo.png")) { // uważać na nazwę pliku dla logo
        std::cout << "ERROR: Nie znaleziono logo.png! Uzywam tekstu zastepczego." << std::endl;
    }

else {
    logoSprite.setTexture(logoTexture);
    }
}

void game::initButtons()
{
    // Przycisk start
    this->btnStart.setSize(sf::Vector2f(200.f, 60.f));
    this->btnStart.setFillColor(sf::Color(0, 100, 0)); // Ciemny zielony
    this->btnStart.setOutlineThickness(2.f);
    this->btnStart.setOutlineColor(sf::Color::White);
    // Ustawimy pozycję później w render, ale warto dać domyślną
    this->btnStart.setPosition(290.f, 400.f);

    this->textStart.setFont(font);
    this->textStart.setString("START");
    this->textStart.setCharacterSize(30);
    this->textStart.setFillColor(sf::Color::White);
    // Centrowanie napisu na przycisku
    sf::FloatRect textRect = textStart.getLocalBounds();
    textStart.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
    textStart.setPosition(btnStart.getPosition().x + btnStart.getSize().x / 2.f, btnStart.getPosition().y + btnStart.getSize().y / 2.f);


    // PRZYCISK PROFIL 
    this->btnProfile.setSize(sf::Vector2f(200.f, 60.f));
    this->btnProfile.setFillColor(sf::Color(0, 0, 100)); // Ciemny niebieski
    this->btnProfile.setOutlineThickness(2.f);
    this->btnProfile.setOutlineColor(sf::Color::White);
    this->btnProfile.setPosition(510.f, 400.f); // Obok przycisku start

    this->textProfile.setFont(font);
    this->textProfile.setString("PROFIL");
    this->textProfile.setCharacterSize(30);
    this->textProfile.setFillColor(sf::Color::White);

    textRect = textProfile.getLocalBounds();
    textProfile.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
    textProfile.setPosition(btnProfile.getPosition().x + btnProfile.getSize().x / 2.f, btnProfile.getPosition().y + btnProfile.getSize().y / 2.f);
}

void game::initDialogs()
{
    // Okno
    this->dialogBox.setSize(sf::Vector2f(760.f, 150.f));
    this->dialogBox.setFillColor(sf::Color(0, 0, 0, 200)); 
    this->dialogBox.setOutlineColor(sf::Color::White);
    this->dialogBox.setOutlineThickness(2.f);
    this->dialogBox.setPosition(20.f, 430.f); 

    // Tekst
    this->dialogText.setFont(this->font);
    this->dialogText.setCharacterSize(20);
    this->dialogText.setFillColor(sf::Color::White);
    this->dialogText.setPosition(40.f, 470.f);

    // Nazwa
    this->dialogName.setFont(this->font);
    this->dialogName.setCharacterSize(24);
    this->dialogName.setFillColor(sf::Color::Yellow);
    this->dialogName.setPosition(40.f, 440.f);
    this->dialogName.setStyle(sf::Text::Bold);
}

// KONSTRUKTOR I DESTRUKTOR


game::game()
{
    this->initZmienne();
    this->initOkno();
    this->initGracz();
    this->initManagers();
    this->initHUD();  // inicjalizacja HUD (paski i font)
    this->initButtons();
    this->initDialogs();
}

game::~game()
{
    delete this->okno;
    delete this->gracz;
    delete this->waveManager;
    delete this->powerUpManager;
}


// DOSTĘP


const bool game::OknoOtwarte() const
{
    return this->okno->isOpen();
}


// OBSŁUGA EVENTÓW


void game::updateEvent()
{
    while (this->okno->pollEvent(this->event))
    {
        // Zamkniecie okna
        if (this->event.type == sf::Event::Closed)
            this->okno->close();

        // Pobieranie pozycji myszki (potrzebne do przycisków)
        sf::Vector2i mousePosWindow = sf::Mouse::getPosition(*this->okno);
        sf::Vector2f mousePosView = this->okno->mapPixelToCoords(mousePosWindow);
        if (this->isDialogOn && this->event.type == sf::Event::KeyReleased)
        {
            // Jeśli puszczono SPACJĘ -> następna linia
            if (this->event.key.code == sf::Keyboard::Space)
            {
                this->ktoryDialog++; // Idź dalej

                // Czy są jeszcze linie?
                if (this->ktoryDialog < this->iloscDialog.size())
                {
                    this->dialogText.setString(this->iloscDialog[this->ktoryDialog]);
                }
                else
                {
                    // Koniec dialogu -> wracamy do gry
                    this->isDialogOn = false;
                }
            }
            continue;
        }
        // STAN: MENU
        if (this->state == GameState::MENU)
        {
            // Sprawdzanie najechania myszką (Efekt Hover)
            if (btnStart.getGlobalBounds().contains(mousePosView))
            {
                btnStart.setFillColor(sf::Color(0, 180, 0)); // Jasnozielony
                this->audio.playSound("CLICK");
            }
                
            else
                btnStart.setFillColor(sf::Color(0, 100, 0)); // Ciemnozielony

            if (btnProfile.getGlobalBounds().contains(mousePosView))
            {
                btnProfile.setFillColor(sf::Color(0, 0, 180)); // Jasnoniebieski
                this->audio.playSound("CLICK");
            }
            else
                btnProfile.setFillColor(sf::Color(0, 0, 100)); // Ciemnoniebieski

            // Obsługa kliknięć w przyciski
            if (this->event.type == sf::Event::MouseButtonPressed && this->event.mouseButton.button == sf::Mouse::Left)
            {
                if (btnStart.getGlobalBounds().contains(mousePosView))
                {
                    this->nextStateAfterLogin = GameState::PLAYING; // Po loginie idź do gry
                    this->state = GameState::LOGIN;
                }
                else if (btnProfile.getGlobalBounds().contains(mousePosView))
                {
                    this->nextStateAfterLogin = GameState::PROFILE; // Po loginie idź do profilu
                    this->state = GameState::LOGIN;
                }
            }
        }

        // STAN: LOGIN 
        else if (this->state == GameState::LOGIN)
        {
            // Obsługa wpisywania znaków
            if (this->event.type == sf::Event::TextEntered)
            {
                // Enter - Zatwierdzenie
                if (this->event.text.unicode == 13 && !inputNick.empty())
                {
                    this->loadGame(); // Wczytaj postęp dla tego nicku
                    this->state = this->nextStateAfterLogin; // Przejdź tam gdzie wybrano w menu
                }
                // Backspace - Usuwanie
                else if (this->event.text.unicode == 8 && !inputNick.empty())
                {
                    inputNick.pop_back();
                }
                // Litery i cyfry (limit do 12 znaków)
                else if (this->event.text.unicode < 128 && this->event.text.unicode > 31 && inputNick.length() < 12)
                {
                    inputNick += static_cast<char>(this->event.text.unicode);
                }
            }

            // ESCAPE - Powrót do menu głównego
            if (this->event.type == sf::Event::KeyPressed && this->event.key.code == sf::Keyboard::Escape)
            {
                this->state = GameState::MENU;
            }
        }

        // --- STAN: PROFILE (Sklep i Ulepszenia) ---
        else if (this->state == GameState::PROFILE)
        {
            if (this->event.type == sf::Event::KeyPressed)
            {
                // Wyjście do menu z zapisem
                if (this->event.key.code == sf::Keyboard::Escape)
                {
                    this->saveGame();
                    this->state = GameState::MENU;
                }

                // Zmiana tła (klawisze 1-4)
                if (this->event.key.code == sf::Keyboard::Num1) this->setBackground(0);
                if (this->event.key.code == sf::Keyboard::Num2) this->setBackground(1);
                if (this->event.key.code == sf::Keyboard::Num3) this->setBackground(2);
                if (this->event.key.code == sf::Keyboard::Num4) this->setBackground(3);

                // Zmiana koloru gracza (klawisze F1-F4)
                // Wewnątrz updateEvent() -> state == GameState::PROFILE
                if (this->event.key.code == sf::Keyboard::F1) this->gracz->zmienSkorke(0);
                if (this->event.key.code == sf::Keyboard::F2) this->gracz->zmienSkorke(1);
                if (this->event.key.code == sf::Keyboard::F3) this->gracz->zmienSkorke(2);
                if (this->event.key.code == sf::Keyboard::F4) this->gracz->zmienSkorke(3);

                // Zakupy w sklepie
                // HP (Koszt 500)
                if (this->event.key.code == sf::Keyboard::B && gracz->getPunkty() >= 500)
                {
                    gracz->wydajPunkty(500);
                    gracz->ulepszHP();
                }
                // Speed (Koszt 1000)
                if (this->event.key.code == sf::Keyboard::N && gracz->getPunkty() >= 1000)
                {
                    gracz->wydajPunkty(1000);
                    gracz->ulepszSpeed();
                }
            }
        }

        // STAN: GAME OVER 
        else if (this->state == GameState::GAME_OVER)
        {
            if (this->event.type == sf::Event::KeyPressed && this->event.key.code == sf::Keyboard::Escape)
            {
                this->saveGame();     // Zapisz wynik przed wyjściem
                this->gracz->reset(); // Przywróć HP do max na następny raz
                this->state = GameState::MENU;
            }
        }
    }
}


// Update


void game::update()
{

    this->updateEvent();

    if (this->isDialogOn) return;

    if (this->state == GameState::PLAYING)
    {

        float dt = 1.f / 60.f; // przybliżony delta time

        this->updateEvent();

        // update gracza
        this->gracz->update_Player();

        sf::RectangleShape& shapePlayer = this->gracz->getShape();
        sf::Vector2u windowSize = this->okno->getSize();//Vector2u bo debugger tak powiedzial -_-
        sf::FloatRect playerSize = shapePlayer.getGlobalBounds();


        if (shapePlayer.getPosition().x < 0.f) shapePlayer.setPosition(0.f, shapePlayer.getPosition().y);//lewa granica

        else if (shapePlayer.getPosition().y < 0.f) shapePlayer.setPosition(shapePlayer.getPosition().x, 0.f);//gorna granica

        else if (shapePlayer.getPosition().x + playerSize.width > windowSize.x) shapePlayer.setPosition(windowSize.x - playerSize.width, shapePlayer.getPosition().y);//prawa granica

        else if (shapePlayer.getPosition().y + playerSize.height > windowSize.y) shapePlayer.setPosition(shapePlayer.getPosition().x, windowSize.y - playerSize.height);//dolna granica
        // STRZAŁ GRACZA

        shootTimer += dt;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && shootTimer >= shootCooldown)
        {

            this->audio.playSound("STRZAL");//uruchamia funkcje do odtwarzania dzwieku

            if (!doubleShot)
                bullets.emplace_back(gracz->getX() + 22, gracz->getY());
            else
            {
                bullets.emplace_back(gracz->getX() + 5, gracz->getY());
                bullets.emplace_back(gracz->getX() + 40, gracz->getY());
            }
            shootTimer = 0.f;
        }


        // UPDATE OBIEKTÓW

        for (auto& b : bullets) b.update(dt);
        for (auto& eb : enemyBullets) eb.update(dt);
        for (auto& p : powerUps) p.update(dt);

        // update menedżerów
        waveManager->update(dt, enemyBullets);
        powerUpManager->update(dt, gracz->getShape());


        


        // KOLIZJE


       // Bullet -> Enemy
        for (size_t i = 0; i < bullets.size(); i++)
        {
            for (size_t j = 0; j < enemies.size(); j++)
            {
                if (bullets[i].shape.getGlobalBounds().intersects(enemies[j].getHitBox()))
                {
                    enemies[j].hp--;
                    bullets.erase(bullets.begin() + i--);
                    if (enemies[j].hp <= 0)
                    {
                        sound_rand = rand() % 4;
                        switch (sound_rand) {
                        case 0:
                            this->audio.playSound("ENEMY-1");
                            break;
                        case 1:
                            this->audio.playSound("ENEMY-2");
                            break;
                        case 2:
                            this->audio.playSound("ENEMY-3");
                            break;
                        case 3:
                            this->audio.playSound("ENEMY-4");
                            break;
                        }
                        this->gracz->dodajPunkty(100);

                        // spawn power-up z enumem
                        if (rand() % 4 == 0)
                        {
                            PowerUpType type = static_cast<PowerUpType>(rand() % 3 + 1);
                            powerUps.emplace_back(
                                enemies[j].shape.getPosition().x,
                                enemies[j].shape.getPosition().y,
                                type
                            );
                        }
                        enemies.erase(enemies.begin() + j);
                    }
                    break;
                }
            }
        }


        // EnemyBullet -> Player
        
        // Kolizje pocisków wroga z graczem
        for (size_t i = 0; i < enemyBullets.size(); i++)
        {
            if (enemyBullets[i].shape.getGlobalBounds().intersects (this->gracz->getGlobalBounds()))
            {
                if (!shield)
                {
                    this->gracz->otrzymajObrazenia(1);
                    if (this->gracz->getHP() <= 0)
                    {
                        this->state = GameState::GAME_OVER;
                    }
                }
                enemyBullets.erase(enemyBullets.begin() + i--);
            }
        }
        // Enemy ->Player
        for (size_t i = 0; i < enemies.size(); i++)
        {
            // Sprawdzamy, czy prostokąt Gracza nachodzi na prostokąt Wroga
            if (this->gracz->getGlobalBounds().intersects(this->enemies[i].getHitBox()))
            {
                if (!shield)
                {
                    this->gracz->otrzymajObrazenia(1);
                    if (this->gracz->getHP() <= 0)
                    {
                        this->state = GameState::GAME_OVER;
                    }
                }
                enemies.erase(enemies.begin() + i--);
            }
        }

        // PowerUp -> Player
        for (size_t i = 0; i < powerUps.size(); i++)
        {
            if (powerUps[i].shape.getGlobalBounds().intersects(gracz->getGlobalBounds()))
            {
                if (powerUps[i].type == FAST_SHOT) { shootCooldown = 0.15f; fastShotTimer = POWERUP_DURATION; }
                if (powerUps[i].type == DOUBLE_SHOT) { doubleShot = true; doubleShotTimer = POWERUP_DURATION; }
                if (powerUps[i].type == SHIELD) { shield = true; shieldTimer = POWERUP_DURATION; }
                powerUps.erase(powerUps.begin() + i);
                break;
            }
        }


        // SPAWNY FALI (i tresc dialogow)

        if (enemies.empty())
        {
            waveManager->nextWave();
        }
        if (this->waveManager->bossDialog == true)
        {
            this->audio.playSound("ZEUS");
            std::vector<std::string> lines;

            lines.push_back("Syzyfie...");
            losowyDialog = rand() % 3;
            switch (losowyDialog) {
            case 0:
            lines.push_back("Czy malo ci umierania?! ");
            break;
            case 1:
            lines.push_back("Ukaram cie wlasnorecznie za twoja pyche!");
            break;
            case 2:
            lines.push_back("Nie wiem w jakie gierki pogrywas dz, ale mi wystarczy!");
            break;
            }
        this->startDialog(lines, "ZEUS");

        this->waveManager->bossDialog = false; 

        }
        if (this->waveManager->soloDialog == true)
        {
            std::vector<std::string> lines;
            lines.push_back("WASD lub STRZALKI by sie przemiszczac - kliknij SPACE");
            lines.push_back("W trakcie rozgrywki przytrzymaj SPACE by strzelac - kliknij SPACE");
            lines.push_back("Zbieraj POWER-UPY i zdobywaj punkty! Powodzenia");

            this->startDialog(lines, "SAMOUCZEK");

            // Bardzo ważne: resetujemy flagę!

            this->waveManager->soloDialog = false;
        }
    }
}


// RENDER

void game::renderMenu()
{
    // Rysowanie loga
    if (logoTexture.getSize().x > 0)
    {
        float logoX = (800.f / 2.f) - (logoSprite.getGlobalBounds().width / 2.f);
        logoSprite.setPosition(logoX, 100.f);
        this->okno->draw(logoSprite);
    }

    // Przyciski wyśrodkowanie pod logiem
    btnStart.setPosition(400.f - 210.f, 400.f);
    this->okno->draw(btnStart);
    textStart.setPosition(btnStart.getPosition().x + btnStart.getSize().x / 2.f, btnStart.getPosition().y + btnStart.getSize().y / 2.f);
    this->okno->draw(textStart);

    btnProfile.setPosition(400.f + 10.f, 400.f);
    this->okno->draw(btnProfile);
    textProfile.setPosition(btnProfile.getPosition().x + btnProfile.getSize().x / 2.f, btnProfile.getPosition().y + btnProfile.getSize().y / 2.f);
    this->okno->draw(textProfile);
}

void game::renderLogin()
{
    // Ciemna nakładka na tło 
    sf::RectangleShape overlay(sf::Vector2f(800.f, 600.f));
    overlay.setFillColor(sf::Color(0, 0, 0, 200));
    this->okno->draw(overlay);

    powerUpText.setCharacterSize(30);
    powerUpText.setFillColor(sf::Color::White);
    powerUpText.setString("LOGOWANIE PROFILU");

    // Centrowanie tekstu
    sf::FloatRect tr = powerUpText.getLocalBounds();
    powerUpText.setOrigin(tr.width / 2.f, tr.height / 2.f);
    powerUpText.setPosition(400.f, 200.f);
    this->okno->draw(powerUpText);

    powerUpText.setCharacterSize(20);
    powerUpText.setString("Wpisz swoj nick:");
    powerUpText.setPosition(400.f, 260.f);
    this->okno->draw(powerUpText);

    // Pole na nick
    sf::RectangleShape inputField(sf::Vector2f(300.f, 50.f));
    inputField.setFillColor(sf::Color(50, 50, 50));
    inputField.setOutlineThickness(2.f);
    inputField.setOutlineColor(sf::Color::Yellow);
    inputField.setOrigin(150.f, 25.f);
    inputField.setPosition(400.f, 320.f);
    this->okno->draw(inputField);

    // Wpisany tekst
    powerUpText.setCharacterSize(30);
    powerUpText.setFillColor(sf::Color::Yellow);
    powerUpText.setString(inputNick + (((int)(shootTimer * 4) % 2 == 0) ? "_" : "")); // Prosty migający kursor
    tr = powerUpText.getLocalBounds();
    powerUpText.setOrigin(tr.width / 2.f, tr.height / 2.f);
    powerUpText.setPosition(400.f, 315.f);
    this->okno->draw(powerUpText);

    powerUpText.setCharacterSize(15);
    powerUpText.setFillColor(sf::Color::White);
    powerUpText.setString("Nacisnij ENTER aby zatwierdzic\nESC aby wrocic");
    tr = powerUpText.getLocalBounds();
    powerUpText.setOrigin(tr.width / 2.f, tr.height / 2.f);
    powerUpText.setPosition(400.f, 450.f);
    this->okno->draw(powerUpText);
}

void game::renderProfile()
{
    powerUpText.setOrigin(0.f, 0.f);
    powerUpText.setCharacterSize(20);
    powerUpText.setPosition(25.f, 25.f);

    std::string info = "";
    info += "=== PROFIL GRACZA ===\n";
    info += "Nick: " + gracz->getNick() + "\n";
    info += "Dostepne Punkty: " + std::to_string(gracz->getPunkty()) + "\n\n";

    info += "=== WYGLAD ===\n";
    info += "[1-4] Zmien Tlo (Aktualne ID: " + std::to_string(bgID) + ")\n";
    info += "[F1-F4] Zmien Kolor Gracza (Aktualne ID: " + std::to_string(gracz->getSkinID()) + ")\n\n";

    info += "=== SKLEP (ULEPSZENIA) ===\n";
    info += "[B] Kup wiecej HP (+5 Max HP) - Koszt: 500 pkt\n";
    info += "    Aktualny poziom HP: " + std::to_string(gracz->getHpLevel()) + " (Max HP: " + std::to_string(gracz->getHPMax()) + ")\n";
    info += "[N] Kup Szybkosc (+1 Speed) - Koszt: 1000 pkt\n";
    info += "    Aktualny poziom Speed: " + std::to_string(gracz->getSpeedLevel()) + "\n\n";

    info += "[ESC] Wroc do MENU (Gra zostanie zapisana)";

    powerUpText.setString(info);
    this->okno->draw(powerUpText);

    // Rysowanie podglądu gracza w profilu
    sf::RectangleShape preview = gracz->getShape();
    preview.setPosition(600, 100);
    preview.setSize({ 100, 100 });
    this->okno->draw(preview);

 

}

void game::renderHUD()
{
    sf::RectangleShape hudBg({ 800.f, 30.f });
    hudBg.setFillColor(sf::Color(0, 0, 0, 150));
    hudBg.setPosition(0, 0);
    this->okno->draw(hudBg);

    powerUpText.setOrigin(0.f, 0.f);
    powerUpText.setCharacterSize(20);
    powerUpText.setString("GRACZ: " + gracz->getNick() + " | PUNKTY: " + std::to_string(gracz->getPunkty()));
    powerUpText.setPosition(10.f, 5.f);
    this->okno->draw(powerUpText);

    // Pasek HP
    sf::RectangleShape hpBack({ 200.f, 20.f });
    hpBack.setFillColor(sf::Color(50, 50, 50));
    hpBack.setPosition(10.f, 570.f);
    this->okno->draw(hpBack);

    float hpPercent = (float)gracz->getHP() / (float)gracz->getHPMax();
    if (hpPercent < 0) hpPercent = 0; // zabezpieczenie graficzne

    sf::RectangleShape hpFront({ 200.f * hpPercent, 20.f });
    hpFront.setFillColor(sf::Color::Red);
    hpFront.setPosition(10.f, 570.f);
    this->okno->draw(hpFront);

    powerUpText.setString(std::to_string(gracz->getHP()) + "/" + std::to_string(gracz->getHPMax()) + " HP");
    powerUpText.setPosition(220.f, 568.f);
    this->okno->draw(powerUpText);

    
}

void game::renderEndScreen()
{
    powerUpText.setCharacterSize(40);
    std::string msg = "KONIEC GRY!\n\n" + gracz->getNick() + "\nZdobyles lacznie: " + std::to_string(gracz->getPunkty()) + " pkt\n\nNacisnij ESC aby wrocic do menu";

    sf::FloatRect textRect = powerUpText.getLocalBounds();
    powerUpText.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
    powerUpText.setPosition(400, 300);

    powerUpText.setString(msg);
    this->okno->draw(powerUpText);

    // Reset origin po rysowaniu, żeby nie psuć innych tekstów
    powerUpText.setOrigin(0, 0);
}



void game::render()
{
    // Czyścimy okno kolorem tła wybranym w profilu
    this->okno->clear(this->backgroundColor);

    // Wybieramy co narysować na podstawie aktualnego stanu
    if (this->state == GameState::MENU)
    {
        this->renderMenu();
    }
    else if (this->state == GameState::LOGIN)
    {
        this->renderLogin();
    }
    else if (this->state == GameState::PROFILE)
    {
        this->renderProfile();
    }
    else if (this->state == GameState::PLAYING)
    {
        this->okno->clear(sf::Color::Black);

        // render gracza
        this->gracz->render_Player(*okno);

        // render obiektów 
        for (auto& b : bullets) this->okno->draw(b.shape);
        for (auto& eb : enemyBullets) this->okno->draw(eb.shape);
        for (auto& e : enemies) this->okno->draw(e.shape);
        for (auto& p : powerUps) this->okno->draw(p.shape);

        // BOSS HP
        waveManager->drawBossHP(*okno);
        this->renderHUD();
        sf::Vector2f barPos(gracz->getX() - 25.f, gracz->getY() - 20.f); // start lewo od środka gracza
        float yOffset = 0.f;

        if (fastShotTimer > 0.f)
        {
            float width = 50.f * (fastShotTimer / POWERUP_DURATION);
            powerUpBar.setSize({ width, 5.f });
            powerUpBar.setFillColor(sf::Color::Cyan);
            powerUpBar.setPosition(barPos.x, barPos.y - yOffset);
            okno->draw(powerUpBar);

            powerUpText.setString(std::to_string((int)fastShotTimer) + "s");
            powerUpText.setPosition(barPos.x + width + 5.f, barPos.y - yOffset - 2.f);
            okno->draw(powerUpText);

            yOffset += 10.f;
        }

        if (doubleShotTimer > 0.f)
        {
            float width = 50.f * (doubleShotTimer / POWERUP_DURATION);
            powerUpBar.setSize({ width, 5.f });
            powerUpBar.setFillColor(sf::Color::Magenta);
            powerUpBar.setPosition(barPos.x, barPos.y - yOffset);
            okno->draw(powerUpBar);

            powerUpText.setString(std::to_string((int)doubleShotTimer) + "s");
            powerUpText.setPosition(barPos.x + width + 5.f, barPos.y - yOffset - 2.f);
            okno->draw(powerUpText);

            yOffset += 10.f;
        }

        if (shieldTimer > 0.f)
        {
            float width = 50.f * (shieldTimer / POWERUP_DURATION);
            powerUpBar.setSize({ width, 5.f });
            powerUpBar.setFillColor(sf::Color::Green);
            powerUpBar.setPosition(barPos.x, barPos.y - yOffset);
            okno->draw(powerUpBar);

            powerUpText.setString(std::to_string((int)shieldTimer) + "s");
            powerUpText.setPosition(barPos.x + width + 5.f, barPos.y - yOffset - 2.f);
            okno->draw(powerUpText);

            yOffset += 10.f;
        }
    }
    
    else if (this->state == GameState::GAME_OVER)
    {
        this->renderEndScreen();
    }
    // Power-up bars nad graczem




// HUD LEWY GÓRNY RÓG

sf::Vector2f hudPos(10.f, 40.f); // lewy górny róg
float hudYOffset = 0.f; // odstęp w pionie
float radius = 8.f;     // rozmiar kółka

auto drawHUDPowerUp = [&](PowerUpType type, const std::string& name, float timer)
{
    if (timer > 0.f)
    {
        sf::CircleShape circle(radius);
        circle.setFillColor(PowerUp(0,0,type).getColor());
        circle.setPosition(hudPos.x, hudPos.y + hudYOffset);
        okno->draw(circle);

        powerUpText.setString(name);
        powerUpText.setCharacterSize(14);
        powerUpText.setPosition(hudPos.x + 20.f, hudPos.y + hudYOffset - 2.f);
        okno->draw(powerUpText);

        hudYOffset += 20.f;
    }
};

// Fast shot
drawHUDPowerUp(FAST_SHOT, "Fast Shot", fastShotTimer);

// Double shot
drawHUDPowerUp(DOUBLE_SHOT, "Double Shot", doubleShotTimer);

// Shield
drawHUDPowerUp(SHIELD, "Shield", shieldTimer);
    if (this->isDialogOn)
    {
    this->okno->draw(this->dialogBox);
    this->okno->draw(this->dialogName);
    this->okno->draw(this->dialogText);

   
    }

    this->okno->display();
}
