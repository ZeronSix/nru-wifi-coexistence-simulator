# Симулятор совместного существования сетей 5G NR-U и Wi-Fi
### Ждановский Вячеслав Дмитриевич, М05-014б

### Описание

Симулятор сетей 5G NR-U и Wi-Fi, позволяющий моделировать их совместное существование для исследовательских целей.

Рассматривается сценарий плотного размещения сети, когда все устройства "слышат" друг друга.

Поддерживаются модели насыщенного трафика и трафика на основе On-Off процесса.

### Зависимости

Docker-контейнеры данному проекту не требуются в силу его специфики.
Единственные зависимости - это CMake и компилятор C++ с поддержкой C++ 17.

Установка на Debian-подобных дистрибутивах:
```console
$ sudo apt install cmake build-essential
```

### Сборка
```console
$ mkdir build
$ cd build
$ cmake .. -DCMAKE_BUILD_TYPE=Release
$ make
```

### Запуск
В директории `scenarios` лежат сценарии с различными экспериментами (например, только устройства Wi-Fi или Wi-Fi + 5G NR-U с варьированием числа каналов).

В директории `scenarios/scripts` лежат bash-скрипты для удобного их запуска с распараллеливанием выполнения и сбором результатов в единую таблицу.

### Научные публикации

1. Zhdanovskiy V. et al. A comparative study on multi-channel access methods in 5G NR-U networks //2020 IEEE International Black Sea Conference on Communications and Networking (BlackSeaCom). – IEEE, 2020. – С. 1-6.
2. Zhdanovskiy V. D., Loginov V. A., Lyakhov A. I. A Study on the Impact of Out-of-Band Emissions on Performance of 5G New Radio-Unlicensed (NR-U) Networks //Journal of Communications Technology and Electronics. – 2021. – Т. 66. – №. 6. – С. 784-795.