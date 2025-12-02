# Описание

Тестовые задания для НПЦ ОРИ АО НПП "АМЭ".

## Состав проекта

* Задача 1: Алгоритмы сортировки

* Задача 2: Фильтрация дубликатов

* Задача 3: Построение вогнутой оболочки

## Сборка проекта

### Зависимости

* Компилятор с поддержкой C++20 (GCC 11+, Clang 12+).
* CMake (версия 3.16+).
* Git.
* Qt5 (версия 5.15+), включая:
- `qtbase5-dev`, `qtdeclarative5-dev`
- `qml-module-qtquick-controls2`
- `qml-module-qtquick-layouts`
- `qml-module-qt-labs-platform`

### Сборка с использованием Docker

Этот метод требует только установленного Docker.

Собрать образ.

```bash
docker build -t cpp-test-tasks .
```

Запустить контейнер (сборка + юнит тесты)

```bash
docker run --rm cpp-test-tasks
```

### Локальная сборка

Выполнить команды:

```bash
git clone https://github.com/jqnfxa/TestTask.git
cd TestTask

mkdir -p build && cd build
cmake -DCMAKE_BUILD_TYPE=Release ..
cmake --build .
```

## Запуск

Исполняемые файлы находятся в директории `build/bin/`.

* Тесты
```bash
./bin/unit_tests
```

* Тест производительности сортировок (Задача 1)
```bash
./bin/task1_benchmark
```

* Фильтр дубликатов (Задача 2):
```bash
./bin/task2_cli <входной_файл.txt> <выходной_файл.txt>
```

* CLI для вогнутой оболочки (Задача 3):
```bash
./bin/task3_cli <входной_файл.txt> <выходной_файл.txt> [гамма]
```

* GUI для вогнутой оболочки (Задача 3):
```bash
./bin/task3_gui
```
