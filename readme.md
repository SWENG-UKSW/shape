# Shape

Środowisko zadaniowe z metod numerycznych. Program pokazuje bryłę: podstawa to prostokąt W × D, ściany boczne są pionowe, a górna ściana jest zakrzywiona, z wysokością zależną od x. Na płaskie ściany bryły zaczepiasz ploter i wyklikujesz na nim punkty. Twoim zadaniem jest policzenie objętości bryły z tych punktów.

Program działa na Windows x64 oraz na Linuksie z sesją X11 (sekcja [Linux](#linux) na końcu). Gotowy program to `bin/shape.exe` (Windows) lub `bin/shape` (Linux), a Twój kod buduje się do biblioteki `student.dll` (Windows) lub `libstudent.so` (Linux), którą program wczytuje. Opisy poniżej dotyczą Windows; różnice dla Linuksa są w ostatniej sekcji.

## Jedyny plik do edycji: `src/student.cpp`

Zmieniasz wyłącznie `src/student.cpp`. Pozostałych plików, w tym `include/shape_api.h`, `src/plotter.h`, `src/plotter.cpp` i `bin/shape.exe`, nie wolno zmieniać.

W `src/student.cpp` są funkcje opisane w `include/shape_api.h`. Ich sygnatur nie zmieniaj.

| Funkcja | Kiedy jest wołana |
|:--|:--|
| `onPlotterAttached(face, area)` | Po zaczepieniu plotera na ścianie `face`. Szablon przepisuje `area` do `plot_data` i rysuje pusty wykres. |
| `onPointsChanged(face, pts, n)` | Po każdej zmianie punktów zaczepionej ściany. |
| `onRangeChanged(face, has, range_u)` | Po ustawieniu (`has = 1`) albo skasowaniu (`has = 0`) zakresu u klawiszem `X`. |
| `on_key_f_pressed()`, `on_key_g_pressed()` | Po klawiszach `F` i `G`, gdy ploter jest zaczepiony. |
| `computeVolume(faces, n_faces, width, depth)` | Pod klawiszem `T`. Zwraca objętość bryły. To jest Twoje zadanie. |

Funkcji `shape_api_version` i `shape_album_number` nie ruszaj. Wewnątrz `student.cpp` możesz używać biblioteki standardowej, na przykład `std::vector`, oraz funkcji wykresów z `src/plotter.h`.

Domyślnie `computeVolume` zwraca 0, dlatego po wpisaniu numeru albumu `T` daje status `ZLY WYNIK`.

### `computeVolume`

- `faces` ma zawsze 5 wpisów w kolejności ścian (`FACE_FRONT` ... `FACE_BOTTOM`). Wpis ściany zawiera jej punkty (`pts`, `n`, może być 0) i zakres u (`has_range_u`, `range_u`).
- `width` to szerokość W bryły (wzdłuż x), a `depth` to jej głębokość D (wzdłuż y).
- Pod `T` program woła `computeVolume` 7 razy: raz dla Twoich punktów i 6 razy dla tych samych punktów przeliczonych na bryłę przekształconą losowo, o innych wymiarach i innym kształcie. Kolejność wywołań jest losowa, a każde wywołanie dostaje `width` i `depth` swojej bryły.
- Dlatego korzystaj wyłącznie z parametrów `computeVolume` (`faces`, `width`, `depth`). Nie używaj wartości zapamiętanych wcześniej, na przykład w `onPointsChanged`, ani wymiarów wpisanych na sztywno, bo przy przekształconych danych będą nieprawdziwe.
- Zakres u z klawisza `X` jest opcjonalny. `range_u` ma wartość dopiero wtedy, gdy `has_range_u` wynosi 1; inaczej nie korzystaj z `range_u`.

### Wykresy: `F` i `G`

`F` i `G` służą do rysowania na ploterze, na przykład dopasowanej krzywej. W `on_key_f_pressed` i `on_key_g_pressed` zapisz wykres do `plot.png` funkcjami z `src/plotter.h` (na przykład `GenerateContinuousPlotFromFunc` albo `GeneratePlotFromPoints`). Po powrocie z funkcji program wczytuje `plot.png` i pokazuje go na ścianie.

Zakres osi wykresu jest stały: obejmuje ścianę i 10% marginesu z każdej strony, a ustawia go `onPlotterAttached` w `plot_data`. Funkcje wykresów nie dopasowują osi do danych, więc wykres zawsze pokrywa się ze ścianą. Kliknięte punkty i obrys ściany program rysuje sam.

## Krok po kroku

Potrzebne są Visual Studio z obsługą C++, CMake w wersji co najmniej 3.15 oraz git. Przy pierwszej konfiguracji potrzebny jest internet: CMake pobiera bibliotekę wykresów pbPlots.

**1. Konfiguracja.** W głównym katalogu projektu uruchom jedną linię:

```
cmake -S . -B build
```

Jeśli CMake zgłosi, że biblioteka musi być budowana dla x64, usuń katalog `build` i uruchom `cmake -S . -B build -A x64`.

**2. Otwarcie projektu.** Otwórz w Visual Studio plik `build\shape_student.sln`.

**3. Numer albumu.** Otwórz `src/student.cpp` i wpisz swój numer na górze pliku:

```cpp
const uint32_t ALBUM_NUMBER = 0; // wpisz swoj numer albumu
```

Numer wpisz bez zer wiodących, bo C++ czyta liczbę zaczynającą się od `0` jako ósemkową.

**4. Budowanie.** Naciśnij `Ctrl+Shift+B`. Gotowa biblioteka trafia do `bin/student.dll`.

**5. Uruchomienie.** Naciśnij `F5`. Visual Studio uruchamia `bin/shape.exe` z Twoją biblioteką. Tak uruchomiony program debugujesz normalnie, na przykład stawiając pułapki w `student.cpp`. W czasie zatrzymania na pułapce mysz działa w całym systemie bez przycinania. W menedżerze zadań widać wtedy dwa procesy `shape.exe`: drugi odbiera kliknięcia myszy.

**6. Przebudowa przy działającym programie.** Nie musisz zamykać programu po każdej zmianie:

1. Uruchom program przez `Ctrl+F5` (bez debugowania). Budowanie w Visual Studio kończy program uruchomiony przez `F5` razem z sesją debugowania.
2. Zmień kod w `src/student.cpp` i naciśnij `Ctrl+Shift+B`.
3. Kliknij okno programu i naciśnij `R`. Program wczyta nową bibliotekę i skasuje punkty, zakresy, wynik i ploter.

Jeśli wczytanie się nie uda, program dalej działa na poprzedniej wersji biblioteki, a przyczynę podaje w konsoli.

## Numer albumu i tryb testowy

- **`ALBUM_NUMBER = 0` (tryb testowy):** wszyscy dostają tę samą bryłę demonstracyjną. `T` liczy i pokazuje błąd, ale status to zawsze `TRYB TESTOWY`, a `SUKCES` jest niemożliwy.
- **Twój numer albumu (tryb właściwy):** numer wyznacza Twoją bryłę, a wynik jest oceniany.

Po zmianie numeru zbuduj bibliotekę i naciśnij `R`, a program wczyta właściwą bryłę.

## Ściany i współrzędne (u, v)

Punkty każdej ściany są we współrzędnych (u, v) tej ściany, w jednostkach świata. Początek leży w lewym dolnym narożniku ściany oglądanej z zewnątrz, u rośnie w prawo, v w górę.

| Ściana | Litera w tytule | Płaszczyzna | u | v |
|:--|:--|:--|:--|:--|
| `FACE_FRONT` | F | y = 0 | x | z |
| `FACE_BACK` | B | y = D | W − x | z |
| `FACE_LEFT` | L | x = 0 | D − y | z |
| `FACE_RIGHT` | R | x = W | y | z |
| `FACE_BOTTOM` | D | z = 0 | W − x | y (góra obrazu to +y) |

Górna ściana jest zakrzywiona i nie da się na niej zaczepić plotera.

## Klawisze i mysz

| Akcja | Efekt |
|:--|:--|
| Lewy przycisk na płaskiej ścianie | Zaczepia ploter na tej ścianie. Ploter jest jeden: kliknięcie innej ściany przenosi go tam, a punkty każdej ściany zostają zapamiętane. |
| Lewy przycisk na zaczepionym ploterze | Dodaje punkt (u, v). W trybie `X` ustawia granicę zakresu. |
| Prawy przycisk | Usuwa ostatni punkt zaczepionej ściany. |
| Środkowy przycisk + ruch, strzałki | Obracają kamerę. |
| Kółko | Przybliża i oddala. |
| `Home` | Przywraca widok początkowy. |
| `Tab` | Odczepia ploter. |
| `C` | Usuwa punkty i zakres zaczepionej ściany. |
| `X` | Ustawia zakres u zaczepionej ściany: pierwsze kliknięcie to u_min, drugie u_max. |
| `F`, `G` | Wołają `on_key_f_pressed` i `on_key_g_pressed` (Twoje wykresy). |
| `T` | Sprawdza `computeVolume`. |
| `R` | Wczytuje `bin/student.dll` od nowa, kasuje punkty i zakresy wszystkich ścian, wynik i ploter. Zmiana numeru albumu wczytuje też nową bryłę. |
| `ESC` | Zamyka program. |

Punkty klikasz fizyczną myszą lub touchpadem. Kliknięcia wygenerowane programowo są odrzucane, a ich liczbę pokazuje `ODRZUCONE` w tytule okna. Kliknięcie ploteru oglądanego zbyt ukośnie (więcej niż 80° od prostopadłej) jest ignorowane, a jedna ściana mieści najwyżej 500 punktów.

## Pasek tytułu

Wszystkie informacje są w tytule okna:

```
seed=<numer> ploter=<ściana lub -> u=<u> v=<v> pkt=F<n> B<n> L<n> R<n> D<n> <wynik>
```

`u` i `v` to pozycja kursora na zaczepionym ploterze (puste, gdy kursor jest poza nim). `pkt` podaje liczbę punktów na każdej ścianie. W trybie `X` pojawia się `X: u_min` albo `X: u_max`. Po `T` wynik ma postać `V=<objętość> blad=<błąd>% <STATUS>`.

## Statusy

`blad` to błąd względny objętości dla Twoich punktów. Próg dla Twoich punktów to 0,5%, dla przekształconych brył 2%. Po `T` w konsoli pojawia się dodatkowo największy błąd wśród przekształconych brył.

| Status | Znaczenie |
|:--|:--|
| `TRYB TESTOWY` | `ALBUM_NUMBER` wynosi 0, wynik nie jest oceniany. |
| `ZLY WYNIK` | Błąd dla Twoich punktów przekracza 0,5% (także NaN lub nieskończoność). |
| `NIESPOJNY` | Błąd dla Twoich punktów mieści się w 0,5%, ale dla co najmniej jednej przekształconej bryły przekracza 2%. Zwykle znaczy to, że `computeVolume` korzysta z czegoś poza swoimi parametrami. Granice całkowania bierz z `width` (albo z `faces[...].range_u`), nigdy z `plot_data.user_range_x`. Przy `NIESPOJNY` wypisz w `computeVolume` wartości, z których liczysz, i sprawdź, czy zmieniają się między wywołaniami. |
| `SUKCES` | Błąd jest w normie dla Twoich punktów i dla wszystkich przekształconych brył. |

## Oddanie

Rozwiązanie oddajesz jako Pull Request z gałęzi `rozwiazanie` do `master` we własnym repozytorium. Opis PR wypełnia się automatycznie z pliku `.github/pull_request_template.md` (katalog `.github` może być ukryty w Eksploratorze Windows). Uzupełnij w nim wszystkie pola: dane, zrzut ekranu, odpowiedzi na pytania i checklistę.

## Gdy program się nie uruchamia

Program pokazuje przyczynę w okienku i w konsoli, a potem się zamyka:

| Komunikat dotyczy | Co zrobić |
|:--|:--|
| Braku `student.dll` | Zbuduj bibliotekę (krok 4). |
| Złej architektury | Zbuduj bibliotekę dla x64 (krok 1). |
| Brakującej funkcji | Przywróć ją w `student.cpp` i zbuduj bibliotekę ponownie. |
| Złej wersji interfejsu | Zbuduj bibliotekę z dostarczonym `include/shape_api.h`. |
| Haka myszy | Uruchom program ponownie; jeśli to nie pomaga, zgłoś to prowadzącemu. |

## Linux

### Wymagania

- Linux x86-64 z biblioteką glibc 2.34 lub nowszą, sprawdzone na Ubuntu 22.04 i 24.04 (powinien działać też np. na Debianie 12).
- **Sesja X11.** Na Waylandzie program się nie uruchomi. Wyloguj się i na ekranie logowania wybierz sesję X11 (Xorg). Sprawdzisz to poleceniem `echo $XDG_SESSION_TYPE`, które ma wypisać `x11`.
- Kompilator C++ (g++), CMake w wersji co najmniej 3.15 i git. Przy pierwszej konfiguracji potrzebny jest internet: CMake pobiera bibliotekę wykresów pbPlots. Na Ubuntu i Debianie:

```
sudo apt install build-essential cmake git libx11-6 libxi6 libgl1
```

Inne systemy i Wayland nie są obsługiwane. Jeśli program nie działa, zgłoś to prowadzącemu z treścią komunikatu z konsoli.

### Krok po kroku

Wszystkie polecenia uruchamiasz w głównym katalogu projektu.

**1. Konfiguracja:**

```
cmake -S . -B build
```

**2. Numer albumu.** Otwórz `src/student.cpp` w dowolnym edytorze i wpisz swój numer na górze pliku:

```cpp
const uint32_t ALBUM_NUMBER = 0; // wpisz swoj numer albumu
```

Numer wpisz bez zer wiodących, bo C++ czyta liczbę zaczynającą się od `0` jako ósemkową.

**3. Budowanie:**

```
cmake --build build
```

Gotowa biblioteka trafia do `bin/libstudent.so`.

**4. Uruchomienie** (w drugim terminalu, żeby widzieć komunikaty programu):

```
./bin/shape
```

**5. Przebudowa przy działającym programie.** Nie musisz zamykać programu po każdej zmianie: zmień kod, uruchom `cmake --build build`, kliknij okno programu i naciśnij `R`. Program wczyta nową bibliotekę i skasuje punkty, zakresy, wynik i ploter. Jeśli wczytanie się nie uda, program dalej działa na poprzedniej wersji biblioteki, a przyczynę podaje w konsoli.

**Debugowanie:** `gdb ./bin/shape`, pułapki stawiasz w funkcjach z `student.cpp` (np. `break computeVolume`). Debugowanie działa normalnie, mysz w systemie nie przycina w czasie zatrzymania. Biblioteka jest budowana domyślnie w konfiguracji Debug.

Klawisze, statusy, tytuł okna i oddanie przez Pull Request są takie same jak na Windows. Katalog `.github` z szablonem PR widać w `ls -a`.

### Gdy program się nie uruchamia

Program wypisuje przyczynę w konsoli i się zamyka:

| Komunikat dotyczy | Co zrobić |
|:--|:--|
| Braku `libstudent.so` | Zbuduj bibliotekę (krok 3). |
| Brakującej funkcji | Przywróć ją w `student.cpp` i zbuduj bibliotekę ponownie. |
| Złej wersji interfejsu | Zbuduj bibliotekę z dostarczonym `include/shape_api.h`. |
| Sesji Wayland | Zaloguj się do sesji X11 (Xorg). |
| Braku biblioteki `libXi.so.6` lub `libGL` | Doinstaluj pakiety z sekcji Wymagania. |
