#include "game.h"
#include <cstdlib>
#include <iostream>

// ============================
// SAVE / LOAD SYSTEM
// ============================

void game::saveGame()
{
    // Zabezpieczenie: nie zapisuj, jeśli nick jest pusty
    if (gracz->getNick().empty()) return;

    // Tworzymy nazwę pliku na podstawie nicku, np. "Adam.txt"
    std::string fileName = gracz->getNick() + ".txt";

    std::ofstream file(fileName);
    if (file.is_open())
    {
        // Kolejność: Nick Punkty MaxHP SpeedLevel HPLevel ColorID BgID
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
    if (nickToLoad.empty()) nickToLoad = "Gracz"; // Fallback

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

// ============================
// INIT & KONSTRUKTOR
// ============================

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
    this->okno = new sf::RenderWindow(sf::VideoMode(800, 600), "Space Shooter - System Profili");
    this->okno->setFramerateLimit(60);
}

void game::initGracz()
{
    this->gracz = new Player(375.f, 500.f);
}

void game::initManagers()
{
    waveManager = new WaveManager(enemies);
    powerUpManager = new PowerUpManager(
        powerUps, doubleShot, shield, shootCooldown,
        doubleShotTimer, fastShotTimer, shieldTimer, POWERUP_DURATION
    );
}
void game::initHUD()
{
    // Ładowanie czcionki
    if (!font.loadFromFile("arial.ttf")) {
        std::cout << "ERROR: Nie znaleziono arial.ttf" << std::endl;
    }

    // Konfiguracja tekstu
    powerUpText.setFont(font);
    powerUpText.setCharacterSize(20);
    powerUpText.setFillColor(sf::Color::White);

    // Pasek powerupów
    powerUpBar.setSize({ 50.f, 5.f });
    powerUpBar.setFillColor(sf::Color::Green);

    // --- ŁADOWANIE LOGA ---
    // Upewnij się, że plik nazywa się logo.png (lub zmień nazwę tutaj)
    if (!logoTexture.loadFromFile("logo.png")) {
        std::cout << "ERROR: Nie znaleziono logo.png! Uzywam tekstu zastepczego." << std::endl;
    }
    else {
        logoSprite.setTexture(logoTexture);

        // Opcjonalnie: Skalowanie, jeśli logo jest za duże/za małe
        // np. logoSprite.setScale(0.5f, 0.5f); 
    }
}

void game::initButtons()
{
    // === PRZYCISK START ===
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


    // === PRZYCISK PROFIL ===
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

game::game()
{
    this->initZmienne();
    this->initOkno();
    this->initGracz();
    this->initManagers();
    this->initHUD();
    this->initButtons();

    // UWAGA: Usunąłem stąd loadGame()! 
    // Nie wiemy jeszcze kogo wczytać. Zrobimy to po wpisaniu nicku.
}

game::~game()
{
    this->saveGame(); // Zapisuje aktualnego gracza przy wyjściu
    delete this->okno;
    delete this->gracz;
    delete waveManager;
    delete powerUpManager;
}

const bool game::OknoOtwarte() const
{
    return this->okno->isOpen();
}

// ============================
// UPDATE EVENTS
// ============================

void game::updateEvent()
{
    while (this->okno->pollEvent(this->event))
    {
        // 1. ZAMKNIĘCIE OKNA
        if (this->event.type == sf::Event::Closed)
            this->okno->close();

        // 2. POBRANIE POZYCJI MYSZKI (potrzebne do przycisków)
        sf::Vector2i mousePosWindow = sf::Mouse::getPosition(*this->okno);
        sf::Vector2f mousePosView = this->okno->mapPixelToCoords(mousePosWindow);

        // --- STAN: MENU GŁÓWNE ---
        if (this->state == GameState::MENU)
        {
            // Sprawdzanie najechania myszką (Efekt Hover)
            if (btnStart.getGlobalBounds().contains(mousePosView))
                btnStart.setFillColor(sf::Color(0, 180, 0)); // Jasnozielony
            else
                btnStart.setFillColor(sf::Color(0, 100, 0)); // Ciemnozielony

            if (btnProfile.getGlobalBounds().contains(mousePosView))
                btnProfile.setFillColor(sf::Color(0, 0, 180)); // Jasnoniebieski
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

        // --- STAN: LOGIN (Wpisywanie Nicku) ---
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

            // ESC - Powrót do menu głównego
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

        // --- STAN: GAME OVER ---
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

// ============================
// UPDATE LOGIC
// ============================

void game::update()
{
    this->updateEvent();

    if (this->state == GameState::PLAYING)
    {
        float dt = 1.f / 60.f;
        this->gracz->update_Player();

        shootTimer += dt;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && shootTimer >= shootCooldown)
        {
            if (!doubleShot)
                bullets.emplace_back(gracz->getX() + 22, gracz->getY());
            else
            {
                bullets.emplace_back(gracz->getX() + 5, gracz->getY());
                bullets.emplace_back(gracz->getX() + 40, gracz->getY());
            }
            shootTimer = 0.f;
        }

        for (auto& b : bullets) b.update(dt);
        for (auto& eb : enemyBullets) eb.update(dt);
        for (auto& p : powerUps) p.update(dt);

        waveManager->update(dt, enemyBullets);
        powerUpManager->update(dt, gracz->getShape());

        // Kolizje pocisków gracza z wrogami
        for (size_t i = 0; i < bullets.size(); i++)
        {
            for (size_t j = 0; j < enemies.size(); j++)
            {
                if (intersects(bullets[i].shape.getGlobalBounds(), enemies[j].shape.getGlobalBounds()))
                {
                    enemies[j].hp--;
                    bullets.erase(bullets.begin() + i--);

                    if (enemies[j].hp <= 0)
                    {
                        this->gracz->dodajPunkty(100);
                        if (rand() % 4 == 0)
                        {
                            PowerUpType type = static_cast<PowerUpType>(rand() % 3 + 1);
                            powerUps.emplace_back(enemies[j].shape.getPosition().x, enemies[j].shape.getPosition().y, type);
                        }
                        enemies.erase(enemies.begin() + j);
                    }
                    break;
                }
            }
        }

        // Kolizje pocisków wroga z graczem
        for (size_t i = 0; i < enemyBullets.size(); i++)
        {
            if (intersects(enemyBullets[i].shape.getGlobalBounds(), gracz->getGlobalBounds()))
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

        // Kolizje powerupów
        for (size_t i = 0; i < powerUps.size(); i++)
        {
            if (intersects(powerUps[i].shape.getGlobalBounds(), gracz->getGlobalBounds()))
            {
                if (powerUps[i].type == 1) { shootCooldown = 0.15f; fastShotTimer = POWERUP_DURATION; }
                if (powerUps[i].type == 2) { doubleShot = true; doubleShotTimer = POWERUP_DURATION; }
                if (powerUps[i].type == 3) { shield = true; shieldTimer = POWERUP_DURATION; }
                powerUps.erase(powerUps.begin() + i);
                break;
            }
        }

        if (enemies.empty()) waveManager->nextWave();
    }
}

// ============================
// RENDERERS
// ============================
void game::renderMenu()
{
    // 1. RYSOWANIE LOGA (Na samej górze, czysto)
    if (logoTexture.getSize().x > 0)
    {
        float logoX = (800.f / 2.f) - (logoSprite.getGlobalBounds().width / 2.f);
        logoSprite.setPosition(logoX, 100.f);
        this->okno->draw(logoSprite);
    }

    // 2. PRZYCISKI (Wyśrodkowane pod logo)
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
    // Ciemna nakładka na tło (opcjonalnie)
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

    // Pole na Nick
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
    powerUpText.setCharacterSize(20);
    powerUpText.setPosition(50.f, 50.f);

    std::string info = "=== PROFIL GRACZA ===\n";
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
    preview.setPosition(500, 100);
    preview.setSize({ 100, 100 });
    this->okno->draw(preview);

    
        // ... reszta kodu renderowania ...
        std::string s = "PROFIL: " + gracz->getNick() + "\nPUNKTY: " + std::to_string(gracz->getPunkty());
        s += "\n\n[1-4] TLO  [F1-F4] WYBIERZ SKIN\n[B] HP (500pkt)  [N] SPEED (1000pkt)\n\nESC - POWROT";
        powerUpText.setString(s);
        this->okno->draw(powerUpText);
    
}

void game::renderHUD()
{
    sf::RectangleShape hudBg({ 800.f, 30.f });
    hudBg.setFillColor(sf::Color(0, 0, 0, 150));
    hudBg.setPosition(0, 0);
    this->okno->draw(hudBg);

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

    // Timery powerupów
    float yOffset = 40.f;
    sf::Vector2f barPos(10.f, yOffset);

    auto drawBar = [&](float timer, sf::Color col, std::string name) {
        if (timer > 0.f) {
            float width = 100.f * (timer / POWERUP_DURATION);
            powerUpBar.setSize({ width, 10.f });
            powerUpBar.setFillColor(col);
            powerUpBar.setPosition(barPos.x, barPos.y);
            okno->draw(powerUpBar);

            powerUpText.setCharacterSize(12);
            powerUpText.setString(name);
            powerUpText.setPosition(barPos.x + 110.f, barPos.y - 2.f);
            okno->draw(powerUpText);

            barPos.y += 15.f;
        }
        };

    drawBar(fastShotTimer, sf::Color::Cyan, "Szybki Strzal");
    drawBar(doubleShotTimer, sf::Color::Magenta, "Podwojny Strzal");
    drawBar(shieldTimer, sf::Color::Green, "Tarcza");
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
    // 1. Czyścimy okno kolorem tła wybranym w profilu
    this->okno->clear(this->backgroundColor);

    // 2. Wybieramy co narysować na podstawie aktualnego stanu
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
        // Rysowanie obiektów świata gry
        this->gracz->render_Player(*okno);

        for (auto& b : bullets) this->okno->draw(b.shape);
        for (auto& eb : enemyBullets) this->okno->draw(eb.shape);
        for (auto& e : enemies) this->okno->draw(e.shape);
        for (auto& p : powerUps) this->okno->draw(p.shape);

        // Elementy interfejsu w trakcie walki
        waveManager->drawBossHP(*okno);
        this->renderHUD();
    }
    else if (this->state == GameState::GAME_OVER)
    {
        this->renderEndScreen();
    }

    // 3. Wyświetlamy wszystko na ekranie
    this->okno->display();

}