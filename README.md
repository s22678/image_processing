prosty program do przetwarzania obrazow

kompilacja ze wszystkimi plikami zordlowymi/naglowkowymi w jednym katalogu:
 - GNU/Linux
    - g++ -std=c++17 *.cpp
 - Windows (Visual Studio/MSVC)
    - sciagnac wszystkie pliki do pustego, dowolnego folderu
    - otworzyc folder w Visual Studio
    - stworzyc projekt z folderu: File -> New -> Project From Existing Code
    - zmienic opcje kompilacji na std=c++17 https://docs.microsoft.com/en-us/cpp/build/reference/std-specify-language-standard-version?view=msvc-170#to-set-this-compiler-option-in-the-visual-studio-development-environment
    - Build -> Build Solution

przyklad uzycia:
GNU/Linux
./a.out -i resources/sample.pgm -n -b gauss -dn -dn -dn  -n -b average -r 30 -g 2 -o ./obraz_obrocony.pgm

Windows (Visual Studio/MSVC)
image_processing.exe -i sample.pgm -n -b gauss -dn -dn -dn  -n -b average -r 30 -g 2 -o new_sample.pgm