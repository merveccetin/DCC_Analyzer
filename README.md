# DCC_Analyzer
---
*Host’ta çalışan (x86 Linux/Windows) bir Clang LibTooling tabanlı statik DCC analiz aracı*

Tek bir C++ aracıyla fonksiyonların içinde:

* Kontrol parametrelerini (koşullarda kullanılan parametreler)
* Veri kullanımlarını (parametre referansları)
* Guarded data uses (kontrol blokları içinde gerçekleşen veri kullanımları) tespit edip dDCC = guarded/all metriğini üretiyor.

## 1. MSYS2 kur: msys2.org

## 2.“MSYS2 MinGW x64” terminalini aç:

## 3.MSYS2 MinGW x64 terminalinde:

```
pacman -Syu
pacman -S --needed mingw-w64-x86_64-clang mingw-w64-x86_64-llvm \
               mingw-w64-x86_64-cmake mingw-w64-x86_64-toolchain
```

```
pacman -Syu
pacman -S --needed \
  mingw-w64-x86_64-clang \
  mingw-w64-x86_64-llvm \
  mingw-w64-x86_64-cmake \
  mingw-w64-x86_64-make \
  mingw-w64-x86_64-toolchain \
  mingw-w64-x86_64-bear
```

Yeniden kurulum adımı

```
MSYS2 MinGW64 terminalini açıp adım adım şu komutları gir:

pacman -Syu     # Sistemi güncelle
# Terminal kapanabilir, tekrar aç

pacman -S --needed mingw-w64-x86_64-toolchain
# Bu ekranda paket listesini göreceksin -> sadece Enter bas (hepsi kurulsun)

pacman -S --needed mingw-w64-x86_64-llvm mingw-w64-x86_64-clang mingw-w64-x86_64-cmake
```


```
pacman -S --needed mingw-w64-x86_64-llvm mingw-w64-x86_64-clang
```
```
Ek olarak CMake de lazım:

pacman -S --needed mingw-w64-x86_64-cmake
```
```
MSYS2 MinGW64 terminalini açıp şunu çalıştır:

pacman -S --needed mingw-w64-x86_64-clang-tools-extra
```

```
Kurulum bitince kontrol:

ls /mingw64/lib/cmake/llvm/LLVMConfig.cmake
ls /mingw64/lib/cmake/clang/ClangConfig.cmake
```

Not: PATH’e C:\msys64\mingw64\bin ekli olması faydalı

## 4. Qt Creator’da MinGW 64-bit kitini seç

## 5. Qt Creator → Tools → Options → Kits

 * Compilers: MinGW-w64 (g++/clang) görünmeli
 * CMake: MSYS2’nin CMake’i görünmeli
 * Kit: MinGW 64-bit bir kit seç (MSVC değil!)

Qt Creator → Projects (anahtar ikonu) → Build & Run → Kit olarak Desktop Qt MinGW 64-bit seç.

! MSVC kiti ile MSYS2 LLVM çalışmaz.

## 6. CMake’i yeniden yapılandır

 * Build → Clear CMake Configuration
 * Sonra Build → Run CMake
 * Ardından Build (Ctrl + b).

 ##7. MSY2 MINGW64 Terminalinde: 
 
 Aşağıdaki ya da debug yöntemini seçtiysen (çok yavaş oluyor) onun içerisinde yer alan gitmeli:

```
$  cd Desktop_Qt_6_9_0_MinGW_64_bit-Release/
```
ardından: 
```
$ ./dcc_tool.exe /c/Users/merve/Desktop/dcc_tool/test.cpp -- -std=gnu++17
```
! burada exe build içerisinde olduğundan dolayı dışaıda test dosyası öenmli olan build ile compile_commands.json dosyasının build içerisinde oluşmuş olması.

ve sonuç şöyle olmalı: 

```
$ ./dcc_tool.exe /c/Users/merve/Desktop/dcc_tool/test.cpp -- -std=gnu++17
file,function,has_control,all_data_uses,guarded_data_uses,dDCC
C:/Users/merve/Desktop/dcc_tool/test.cpp:1,f,true,3,1,0.333

```

