Wypakuj pobran¹ zawartoœæ projektu

SprawdŸ folder "SpaceShooter"

Kliknij dwukrotnie ten folder w Visual Studio.

Poszukaj w œrodku pliku z rozszerzeniem .sln (ikona Visual Studio) lub .vcxproj.

Jeœli go tam znajdziesz – kliknij go dwukrotnie. To prze³aduje Visual Studio w tryb projektu i przycisk startu powinien zadzia³aæ.




Instrukcja konfiguracji SFML w Visual Studio krok po kroku.

Za³o¿enie
Masz ju¿ pobran¹ bibliotekê SFML na dysku (np. na dysku C:\SFML). Jeœli nie, musisz j¹ pobraæ ze strony sfml-dev.org (wersja Visual C++).

1: Otwórz w³aœciwoœci projektu
W Solution Explorer (Eksplorator rozwi¹zañ) kliknij Prawym Przyciskiem Myszy na nazwê projektu SpaceShooter 

Z menu wybierz W³aœciwoœci (Properties) – zazwyczaj jest na samym dole.

2: Wska¿ œcie¿ki (Header i Lib)
W oknie, które siê pojawi³o, upewnij siê, ¿e na górze w "Configuration" (Konfiguracja) masz wybrane All Configurations (Wszystkie konfiguracje) lub Active(Debug).

Nag³ówki (Include):

Po lewej wybierz: C/C++ -> General (Ogólne).

Po prawej znajdŸ: Additional Include Directories (Dodatkowe katalogi plików nag³ówkowych).

Kliknij tam, wybierz <Edytuj...> i wska¿ folder include z Twojego SFML (np. C:..\SFML\include). lub $(SolutionDir)SFML\include

Biblioteki (Library):

Po lewej wybierz: Linker (Konsolidator) -> General (Ogólne).

Po prawej znajdŸ: Additional Library Directories (Dodatkowe katalogi biblioteki).

Kliknij tam, wybierz <Edytuj...> i wska¿ folder lib z Twojego SFML (np. C:..\SFML\lib). lub $(SolutionDir)SFML\include (tylko trzba uwa¿aæ czy siê œcierzka wyznaczy³a poni¿ej)

3: Podaj nazwy bibliotek (Input)


Po lewej wybierz: Linker -> Input (Dane wejœciowe).

Po prawej znajdŸ: Additional Dependencies (Dodatkowe zale¿noœci).

Kliknij, wybierz <Edytuj...> i wklej w polu tekstowym poni¿sz¹ listê (dla trybu Debug, którego u¿ywasz):


sfml-graphics-d.lib
sfml-window-d.lib
sfml-system-d.lib
sfml-audio-d.lib
sfml-network-d.lib

!PAMIÊTAJ O -d NA KOÑCU!

Kliknij OK i Zastosuj.