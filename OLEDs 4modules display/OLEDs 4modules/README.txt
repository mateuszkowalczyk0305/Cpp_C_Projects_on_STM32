# Projekt Wyświetlacza OLED na STM32

Projekt realizowany jest w oparciu o mikrokontroler STM32 i polega na obsłudze wyświetlacza OLED. Wykorzystuje on zmodyfikowane oraz autorskie biblioteki do zarządzania wyświetlaczem i obsługi komunikacji przez UART. Poniżej przedstawiono opis kluczowych plików projektu.

## Struktura projektu

### Pliki

- **bitmaps.c/h**
  - Przechowuje bitmapy w formie tablic znakowych, dostosowanych do wymagań wyświetlacza.
  - Bitmapy zostały wygenerowane za pomocą narzędzia: [Images to Byte Array Converter](https://mischianti.org/images-to-byte-array-online-converter-cpp-arduino/#google_vignette).
  - Grafiki powstały przy użyciu strony [Textcraft](https://textcraft.net), korzystając z czcionki K-ARCADE.

- **oled.cpp/hpp**
  - Pliki te stanowią bibliotekę bazującą na sterowniku wyświetlacza `ssd1306.cpp`, dostosowaną do potrzeb projektu.
  - Obsługują jedynie wyświetlanie bitmap, bez funkcji tekstowych.

- **ssd1306.cpp/h**
  - Zmodyfikowana biblioteka, oryginalnie napisana w języku C, znaleziona na [GitHubie](https://github.com/afiskon/stm32-ssd1306).
  - Istotna uwaga: każda instancja OLED wymaga oddzielnego przypisania pinów dla sygnałów RESET oraz DC.

- **uart_handler.cpp/hpp**
  - Odbiera dane znakowe pojedynczo z wykorzystaniem mechanizmu przerwań.
  - Zapisuje znaki do tablicy do momentu napotkania znaku końca linii (`\r\n`), po czym dokonuje parsingu i realizuje odpowiednie komendy.
  - Szczegółowy opis komend znajduje się w pliku "Command structures for displays.pdf".

## Źródła

- Narzędzie do konwersji obrazów na tablice bajtów: [Images to Byte Array Converter](https://mischianti.org/images-to-byte-array-online-converter-cpp-arduino/#google_vignette)
- Generator grafik: [Textcraft](https://textcraft.net)
- Biblioteka SSD1306: [GitHub - afiskon/stm32-ssd1306](https://github.com/afiskon/stm32-ssd1306)

---

> **Note:** Aby projekt działał poprawnie, każda instancja wyświetlacza OLED musi mieć osobno przypisane piny dla sygnałów RESET oraz DC.

> **Note:** Dodany został również plik uart_lib.zip w którym zrealizowana została już obsługa komend UART z poziomu mastera. Pliki display_handler.cpp/hpp posiadają te funkcjonalności


