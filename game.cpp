#include "game.h"
#include <cstdlib>
#include <iostream>

// ============================
// SAVE/LOAD SYSTEM
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
        // Kolejność: Nick Punkty MaxHP SpeedLevel HPLevel ColorID BgID Skin1Owned Skin2Owned Skin3Owned
        file << gracz->getNick() << " "
            << gracz->getPunkty() << " "
            << gracz->getHPMax() << " "
            << gracz->getSpeedLevel() << " "
            << gracz->getHpLevel() << " "
            << gracz->getSkinID() << " "
            << this->bgID << " "
            // Zapisujemy, czy gracz ma skiny (0 lub 1). Skin 0 zawsze jest 1, więc zapiszmy 1, 2, 3
            << (gracz->czyPosiadaSkin(1) ? 1 : 0) << " "
            << (gracz->czyPosiadaSkin(2) ? 1 : 0) << " "
            << (gracz->czyPosiadaSkin(3) ? 1 : 0);

    }
    file.close();
    std::cout << "Zapisano gre do pliku: " << fileName << std::endl;
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
        int s1, s2, s3; // Zmienne na status skórek

        // Czytamy dane
        // Próba wczytania podstawowych danych
        if (file >> n >> pts >> hpMx >> spdLvl >> hpLvl >> skinID >> bID)
        {
            gracz->setNick(n);
            gracz->setPunkty(pts);
            gracz->setHpMax(hpMx);
            gracz->reset(); // Leczy HP do nowego Maxa

            // Resetujemy prędkość i nakładamy ulepszenia
            gracz->setSpeed(5.f + (spdLvl - 1) * 1.f);

            // Tło
            this->setBackground(bID);

            // -- WCZYTYWANIE SKÓREK --
            // Próbujemy wczytać statusy skórek. Jeśli plik jest stary i ich nie ma, 
            // strumień wejdzie w stan błędu, ale podstawowe dane już są wczytane.
            if (file >> s1 >> s2 >> s3) {
                if (s1 == 1) gracz->odblokujSkin(1);
                if (s2 == 1) gracz->odblokujSkin(2);
                if (s3 == 1) gracz->odblokujSkin(3);
            }
            // ------------------------

            // Ustawiamy skórkę na końcu (musi być odblokowana wcześniej)
            gracz->zmienSkorke(skinID);

            std::cout << "Wczytano profil: " << n << std::endl;
        }
        file.close();
    }
    else
    {
        // JEŚLI PLIK NIE ISTNIEJE -> NOWY GRACZ
        std::cout << "Brak zapisu dla gracza " << nickToLoad << ". Tworze nowy profil." << std::endl;

        // Ważne: Resetujemy gracza do zera
        gracz->resetAllStats();
        gracz->setNick(nickToLoad);
        // Resetujemy też tło na domyślne
        this->setBackground(0);
    }
}

void game::setBackground(int id)
{
    this->bgID = id;
    std::string fileName = "bg" + std::to_string(id) + ".png";

    // Próba załadowania wybranego tła
    if (!this->backgroundTexture.loadFromFile(fileName))
    {
        // JEŚLI SIĘ NIE UDA (np. brak bg1.png), SPRÓBUJ ZAŁADOWAĆ bg0.png
        std::cout << "Nie znaleziono " << fileName << ", laduje bg0.png" << std::endl;
        if (!this->backgroundTexture.loadFromFile("bg0.png"))
        {
            std::cout << "ERROR: KRYTYCZNY BLAD - BRAK bg0.png!" << std::endl;
            return;
        }
    }

    // Ustawienie tekstury (musi być smooth dla płynnego ruchu)
    this->backgroundTexture.setSmooth(true);
    this->backgroundSprite1.setTexture(backgroundTexture);
    this->backgroundSprite2.setTexture(backgroundTexture);

    // Pozycje startowe
    this->backgroundSprite1.setPosition(0, 0);
    this->backgroundSprite2.setPosition(0, -600.f);
}

// ============
// INIT & KONSTRUKTOR
// ============

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
    if (!logoTexture.loadFromFile("logo.png")) {
        std::cout << "ERROR: Nie znaleziono logo.png! Uzywam tekstu zastepczego." << std::endl;
    }
    else {
        logoSprite.setTexture(logoTexture);
    }
}

void game::initButtons()
{
    // === PRZYCISK START ===
    this->btnStart.setSize(sf::Vector2f(200.f, 60.f));
    this->btnStart.setFillColor(sf::Color(0, 100, 0)); // Ciemny zielony
    this->btnStart.setOutlineThickness(2.f);
    this->btnStart.setOutlineColor(sf::Color::White);
    this->btnStart.setPosition(290.f, 400.f);

    this->textStart.setFont(font);
    this->textStart.setString("START");
    this->textStart.setCharacterSize(30);
    this->textStart.setFillColor(sf::Color::White);

    sf::FloatRect textRect = textStart.getLocalBounds();
    textStart.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
    textStart.setPosition(btnStart.getPosition().x + btnStart.getSize().x / 2.f,
        btnStart.getPosition().y + btnStart.getSize().y / 2.f);

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
    textProfile.setPosition(btnProfile.getPosition().x + btnProfile.getSize().x / 2.f,
        btnProfile.getPosition().y + btnProfile.getSize().y / 2.f);
}

game::game()
{
    this->initZmienne();
    this->initOkno();
    this->initGracz();
    this->initManagers();
    this->initHUD();
    this->initButtons();
    this->setBackground(0);
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

        // 2. POBRANIE POZYCJI MYSZKI
        sf::Vector2i mousePosWindow = sf::Mouse::getPosition(*this->okno);
        sf::Vector2f mousePosView = this->okno->mapPixelToCoords(mousePosWindow);

        // --- STAN: MENU GŁÓWNE ---
        if (this->state == GameState::MENU)
        {
            // Efekt Hover
            if (btnStart.getGlobalBounds().contains(mousePosView))
                btnStart.setFillColor(sf::Color(0, 180, 0));
            else
                btnStart.setFillColor(sf::Color(0, 100, 0));

            if (btnProfile.getGlobalBounds().contains(mousePosView))
                btnProfile.setFillColor(sf::Color(0, 0, 180));
            else
                btnProfile.setFillColor(sf::Color(0, 0, 100));

            // Kliknięcie
            if (this->event.type == sf::Event::MouseButtonPressed && this->event.mouseButton.button == sf::Mouse::Left)
            {
                if (btnStart.getGlobalBounds().contains(mousePosView))
                {
                    this->nextStateAfterLogin = GameState::PLAYING;
                    this->state = GameState::LOGIN;
                }
                else if (btnProfile.getGlobalBounds().contains(mousePosView))
                {
                    this->nextStateAfterLogin = GameState::PROFILE;
                    this->state = GameState::LOGIN;
                }
            }
        }

        // --- STAN: LOGIN (Wpisywanie Nicku) ---
        else if (this->state == GameState::LOGIN)
        {
            if (this->event.type == sf::Event::TextEntered)
            {
                // Enter
                if (this->event.text.unicode == 13 && !inputNick.empty())
                {
                    this->loadGame();
                    this->state = this->nextStateAfterLogin;
                }
                // Backspace
                else if (this->event.text.unicode == 8 && !inputNick.empty())
                {
                    inputNick.pop_back();
                }
                // Litery i cyfry
                else if (this->event.text.unicode < 128 && this->event.text.unicode > 31 && inputNick.length() < 12)
                {
                    inputNick += static_cast<char>(this->event.text.unicode);
                }
            }
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
                // Wyjście do menu
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

                // --- OBSŁUGA KUPOWANIA SKÓREK (F1 - F4) ---

                // F1: Skin 0 (Zawsze darmowy)
                if (this->event.key.code == sf::Keyboard::F1) {
                    this->gracz->zmienSkorke(0);
                }

                // F2: Skin 1 (Cena: 1000)
                if (this->event.key.code == sf::Keyboard::F2) {
                    if (gracz->czyPosiadaSkin(1)) {
                        gracz->zmienSkorke(1);
                    }
                    else if (gracz->getPunkty() >= 1000) {
                        gracz->wydajPunkty(1000);
                        gracz->odblokujSkin(1);
                        gracz->zmienSkorke(1);
                    }
                }

                // F3: Skin 2 (Cena: 3000)
                if (this->event.key.code == sf::Keyboard::F3) {
                    if (gracz->czyPosiadaSkin(2)) {
                        gracz->zmienSkorke(2);
                    }
                    else if (gracz->getPunkty() >= 3000) {
                        gracz->wydajPunkty(3000);
                        gracz->odblokujSkin(2);
                        gracz->zmienSkorke(2);
                    }
                }

                // F4: Skin 3 (Cena: 5000)
                if (this->event.key.code == sf::Keyboard::F4) {
                    if (gracz->czyPosiadaSkin(3)) {
                        gracz->zmienSkorke(3);
                    }
                    else if (gracz->getPunkty() >= 5000) {
                        gracz->wydajPunkty(5000);
                        gracz->odblokujSkin(3);
                        gracz->zmienSkorke(3);
                    }
                }
                // ------------------------------------------

                // Zakupy statystyk
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
                this->saveGame();
                this->gracz->reset();
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

        // Ruch tła
        this->backgroundSprite1.move(0.f, backgroundSpeed * dt);
        this->backgroundSprite2.move(0.f, backgroundSpeed * dt);

        if (this->backgroundSprite1.getPosition().y >= 600.f)
            this->backgroundSprite1.setPosition(0.f, -600.f);
        if (this->backgroundSprite2.getPosition().y >= 600.f)
            this->backgroundSprite2.setPosition(0.f, -600.f);

        // Gracz
        this->gracz->update_Player();

        // Strzelanie
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

        // Aktualizacje obiektów
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
    if (logoTexture.getSize().x > 0)
    {
        float logoX = (800.f / 2.f) - (logoSprite.getGlobalBounds().width / 2.f);
        logoSprite.setPosition(logoX, 100.f);
        this->okno->draw(logoSprite);
    }

    btnStart.setPosition(400.f - 210.f, 400.f);
    this->okno->draw(btnStart);
    textStart.setPosition(btnStart.getPosition().x + btnStart.getSize().x / 2.f,
        btnStart.getPosition().y + btnStart.getSize().y / 2.f);
    this->okno->draw(textStart);

    btnProfile.setPosition(400.f + 10.f, 400.f);
    this->okno->draw(btnProfile);
    textProfile.setPosition(btnProfile.getPosition().x + btnProfile.getSize().x / 2.f,
        btnProfile.getPosition().y + btnProfile.getSize().y / 2.f);
    this->okno->draw(textProfile);
}

void game::renderLogin()
{
    sf::RectangleShape overlay(sf::Vector2f(800.f, 600.f));
    overlay.setFillColor(sf::Color(0, 0, 0, 200));
    this->okno->draw(overlay);

    powerUpText.setCharacterSize(30);
    powerUpText.setFillColor(sf::Color::White);
    powerUpText.setString("LOGOWANIE PROFILU");

    sf::FloatRect tr = powerUpText.getLocalBounds();
    powerUpText.setOrigin(tr.width / 2.f, tr.height / 2.f);
    powerUpText.setPosition(400.f, 200.f);
    this->okno->draw(powerUpText);

    powerUpText.setCharacterSize(20);
    powerUpText.setString("Wpisz swoj nick:");
    powerUpText.setPosition(400.f, 260.f);
    this->okno->draw(powerUpText);

    sf::RectangleShape inputField(sf::Vector2f(300.f, 50.f));
    inputField.setFillColor(sf::Color(50, 50, 50));
    inputField.setOutlineThickness(2.f);
    inputField.setOutlineColor(sf::Color::Yellow);
    inputField.setOrigin(150.f, 25.f);
    inputField.setPosition(400.f, 320.f);
    this->okno->draw(inputField);

    powerUpText.setCharacterSize(30);
    powerUpText.setFillColor(sf::Color::Yellow);
    powerUpText.setString(inputNick + (((int)(shootTimer * 4) % 2 == 0) ? "_" : ""));
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
    powerUpText.setCharacterSize(22);
    powerUpText.setPosition(30.f, 30.f);

    std::string info = "=== PROFIL GRACZA: " + gracz->getNick() + " ===\n";
    info += "Punkty: " + std::to_string(gracz->getPunkty()) + "\n\n";

    info += "=== SKLEP SKOREK ===\n";

    // Helper do wyświetlania cen/statusu
    auto getSkinStatus = [&](int id, int price) -> std::string {
        if (gracz->getSkinID() == id) return " [WYBRANY]";
        if (gracz->czyPosiadaSkin(id)) return " [POSIADANE]";
        return " [Cena: " + std::to_string(price) + "]";
        };

    info += "[F1] Default" + getSkinStatus(0, 0) + "\n";
    info += "[F2] Skin 1 " + getSkinStatus(1, 1000) + "\n";
    info += "[F3] Skin 2 " + getSkinStatus(2, 3000) + "\n";
    info += "[F4] Skin 3 " + getSkinStatus(3, 5000) + "\n\n";

    info += "=== WYGLAD TLA ===\n";
    info += "[1-4] Tlo (ID: " + std::to_string(bgID) + ")\n\n";

    info += "=== SKLEP STATYSTYK ===\n";
    info += "[B] HP (+5) - 500 pkt (Lvl: " + std::to_string(gracz->getHpLevel()) + ")\n";
    info += "[N] Speed (+1) - 1000 pkt (Lvl: " + std::to_string(gracz->getSpeedLevel()) + ")\n\n";

    info += "[ESC] Powrot i Zapis";

    powerUpText.setCharacterSize(20);
    powerUpText.setPosition(20.f, 20.f);
    powerUpText.setString(info);
    this->okno->draw(powerUpText);

    // Podgląd gracza
    sf::RectangleShape preview = gracz->getShape();
    preview.setPosition(500.f, 150.f);
    preview.setSize({ 150.f, 150.f });
    this->okno->draw(preview);
}

void game::renderHUD()
{
    powerUpText.setOrigin(0.f, 0.f);

    sf::RectangleShape hudBg({ 800.f, 30.f });
    hudBg.setFillColor(sf::Color(0, 0, 0, 150));
    hudBg.setPosition(0, 0);
    this->okno->draw(hudBg);

    powerUpText.setCharacterSize(20);
    powerUpText.setFillColor(sf::Color::White);
    powerUpText.setString("GRACZ: " + gracz->getNick() + " | PUNKTY: " + std::to_string(gracz->getPunkty()));
    powerUpText.setPosition(10.f, 5.f);
    this->okno->draw(powerUpText);

    // Pasek HP
    sf::RectangleShape hpBack({ 200.f, 20.f });
    hpBack.setFillColor(sf::Color(50, 50, 50));
    hpBack.setPosition(10.f, 570.f);
    this->okno->draw(hpBack);

    float hpPercent = (float)gracz->getHP() / (float)gracz->getHPMax();
    if (hpPercent < 0) hpPercent = 0;

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
    powerUpText.setOrigin(0, 0);
}

void game::render()
{
    this->okno->clear();

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
        this->powerUpText.setOrigin(0.f, 0.f);
        this->renderProfile();
    }
    else if (this->state == GameState::PLAYING)
    {
        this->okno->draw(this->backgroundSprite1);
        this->okno->draw(this->backgroundSprite2);

        this->gracz->render_Player(*okno);
        for (auto& b : bullets) this->okno->draw(b.shape);
        for (auto& eb : enemyBullets) this->okno->draw(eb.shape);
        for (auto& e : enemies) this->okno->draw(e.shape);
        for (auto& p : powerUps) this->okno->draw(p.shape);

        waveManager->drawBossHP(*okno);
        this->renderHUD();
    }
    else if (this->state == GameState::GAME_OVER)
    {
        this->renderEndScreen();
    }

    this->okno->display();
}