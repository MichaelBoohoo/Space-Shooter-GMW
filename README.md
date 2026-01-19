Jak ustawić projekt?

1.Wypakuj pobraną zawartość projektu

Sprawdź folder "SpaceShooter"

2.Kliknij dwukrotnie ten folder w Visual Studio.

Poszukaj w środku pliku z rozszerzeniem .sln (ikona Visual Studio) lub .vcxproj.

Jeśli go tam znajdziesz – kliknij go dwukrotnie. To przeładuje Visual Studio w tryb projektu i przycisk startu powinien zadziałać.

!jeśli go nie ma



1.Utwórz nowy projekt:

Kliknij Utwórz nowy projekt (Create a new project).

Wybierz Pusty projekt (Empty Project) dla języka C++. Kliknij Dalej.

Nazwij go np. "SpaceShooter" i utwórz.

2.Dodaj istniejące pliki:

Teraz w Eksploratorze rozwiązań zobaczysz strukturę drzewka (Pliki źródłowe, Pliki nagłówkowe).

Kliknij Prawym Przyciskiem Myszy na nazwę projektu. Otwórz folder w Eksploratorze plików (Open Folder in File Explorer).

Skopiuj wszystkie pliki, które masz na zrzucie ekranu (takie jaki Bullet.cpp, Enemy.h, Zrodlo.cpp itd.) do tego folderu, który właśnie otworzyłeś.

3. Podłącz pliki w Visual Studio:


Kliknij Prawym na folder "Pliki nagłówkowe" (Header Files) -> Dodaj -> Istniejący element. Zaznacz wszystkie pliki z końcówką .h.

Kliknij Prawym na folder "Pliki źródłowe" (Source Files) -> Dodaj -> Istniejący element. Zaznacz wszystkie pliki z końcówką .cpp.

-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

Instrukcja konfiguracji SFML w Visual Studio 

Założenie

Masz już pobraną bibliotekę SFML na dysku (np. na dysku C:\SFML). Jeśli nie, musisz ją pobrać ze strony sfml-dev.org (wersja Visual C++).

1: Otwórz właściwości projektu
W Solution Explorer (Eksplorator rozwiązań) kliknij Prawym Przyciskiem Myszy na nazwę projektu SpaceShooter 

Z menu wybierz Właściwości (Properties) – zazwyczaj jest na samym dole.

2: Wskaż ścieżki (Header i Lib)
W oknie, które się pojawiło, upewnij się, że na górze w "Configuration" (Konfiguracja) masz wybrane All Configurations (Wszystkie konfiguracje) lub Active(Debug).

Nagłówki (Include):

Po lewej wybierz: C/C++ -> General (Ogólne).

Po prawej znajdź: Additional Include Directories (Dodatkowe katalogi plików nagłówkowych).

Kliknij tam, wybierz <Edytuj...> i wskaż folder include z Twojego SFML (np. C:..\SFML\include). lub $(SolutionDir)SFML\include

Biblioteki (Library):

Po lewej wybierz: Linker (Konsolidator) -> General (Ogólne).

Po prawej znajdź: Additional Library Directories (Dodatkowe katalogi biblioteki).

Kliknij tam, wybierz <Edytuj...> i wskaż folder lib z Twojego SFML (np. C:..\SFML\lib). lub $(SolutionDir)SFML\include (tylko trzba uważać czy się ścierzka wyznaczyła poniżej)

3: Podaj nazwy bibliotek (Input)


Po lewej wybierz: Linker -> Input (Dane wejściowe).

Po prawej znajdź: Additional Dependencies (Dodatkowe zależności).

Kliknij, wybierz <Edytuj...> i wklej w polu tekstowym poniższą listę (dla trybu Debug, którego używasz):


sfml-graphics-d.lib
sfml-window-d.lib
sfml-system-d.lib
sfml-audio-d.lib
sfml-network-d.lib

!PAMIĘTAJ O -d NA KOŃCU!

Kliknij OK i Zastosuj.

4: Skopiuj pliki DLL 

Wejdź do folderu, gdzie masz pobrany SFML -> folder bin.

Skopiuj wszystkie pliki .dll z tego folderu.

Wklej je do folderu ze swoim projektem (tam, gdzie masz pliki Zrodlo.cpp, game.cpp itd.).
