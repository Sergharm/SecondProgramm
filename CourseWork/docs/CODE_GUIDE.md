# Разбор исходного кода LinuxSandbox v0.1

Руководство относится к исходному коду первого чекпоинта. В примерах можно проследить путь от выбора файла до сетевого события и записи TXT. Номера указывают на строки исходных файлов на момент подготовки материала.

## Структура проекта

```text
CourseWork/
├── .gitignore, AGENTS.md, CMakeLists.txt, PROGRESS.md, README.md
├── cmake/EmbedTests.cmake
├── include/
│   ├── ConsoleUI.h, Event.h, FileDescriptor.h, IsolationManager.h
│   └── NetworkMonitor.h, ProcessManager.h, ReportManager.h, Sandbox.h
├── src/
│   ├── main.cpp, ConsoleUI.cpp, Sandbox.cpp, IsolationManager.cpp
│   └── ProcessManager.cpp, NetworkMonitor.cpp, ReportManager.cpp
├── tests/test_tcp.cpp, tests/test_udp.cpp
├── docs/architecture.md, checkpoint1.md, roadmap.md и материалы курсовой
├── reports/.gitkeep и локальные TXT-отчёты
├── scripts/ — генераторы документации
└── build/ и build-docs/ — локальная сборка и результаты проверок
```

`src/` содержит реализации, `include/` — интерфейсы. В `tests/` находятся собственные исполняемые примеры. `cmake/` содержит вспомогательное встраивание их содержимого. `CMakeLists.txt` задаёт цели, зависимости, C++17 и предупреждения. В `build/` образуются бинарники, заголовок и технические файлы CMake. `docs/` хранит архитектуру, описание чекпоинта, дорожную карту и документы. `reports/` относится к текущему каталогу запуска и хранит сделанные программой отчёты. `README.md` объясняет сборку, `AGENTS.md` задаёт правила изменений и проверок, `PROGRESS.md` фиксирует проверенное. `.gitignore` исключает сборку, рабочие отчёты и ряд чувствительных файлов.

## Заголовочные файлы

### `include/ConsoleUI.h`

Отдельный заголовок объявляет интерфейс терминального слоя. Стандартных заголовков нет. Предварительное `class Sandbox;` достаточно для хранения ссылки. Поле `Sandbox& sandbox_` указывает на ранее созданное ядро и требует, чтобы оно жило дольше интерфейса. `explicit ConsoleUI(Sandbox&)` получает эту ссылку. `void run()` ведёт сеанс и завершает работу по команде или EOF. Деструктор не объявлен вручную. Вопрос: зачем ссылка? Ответ: консоль обращается к тому же состоянию `Sandbox` и не создаёт копию.

### `include/Event.h`

Независимая структура `Event` держит общую модель для парсера, ядра и интерфейса. `<string>` даёт `std::string`. `pid` — идентификатор из трассы, при отсутствии используется 0. `call` — имя вызова. `protocol` — TCP или UDP при распознавании. `address` и `port` — доступные поля адреса, `result` — числовой код и при наличии пояснение, `success` — признак неотрицательного результата. Структура агрегатная, собственных функций нет. Вопрос: почему порт — строка? Ответ: хранится представление данных из трассы, а пустая строка передаёт отсутствие значения.

### `include/FileDescriptor.h`

`<unistd.h>` объявляет `close`. `FileDescriptor` владеет числом `fd_`. Конструктор `explicit FileDescriptor(int)` принимает результат открытия. Деструктор закрывает только неотрицательное значение. `get() const` возвращает номер без передачи владения. Копирование и присваивание удалены, перемещающие операции не объявлены. Ошибку открытия проверяет вызывающий код по отрицательному номеру. Ошибка закрытия игнорируется в деструкторе. Вопрос: зачем запрет копирования? Ответ: одна копия номера могла бы закрыть дескриптор, которым всё ещё пользуется другая.

### `include/IsolationManager.h`

`<string>` и `<vector>` нужны для `vector<string>`. `IsolationManager::command() const` возвращает аргументы отдельными строками. Класс не хранит полей, собственных конструкторов или деструктора. При ненадлежащих правах или отсутствии исполняемых инструментов реализация выбрасывает исключение. Заголовок использует `Sandbox.h`. Вопрос: почему вектор? Ответ: `execve` получает раздельные аргументы, включая строки с пробелами как цельные значения.

### `include/ProcessManager.h`

`<chrono>` определяет тип тайм-аута, `<string>` — владеющую строку результата, `<string_view>` — невладеющее представление байтов, `<vector>` — список аргументов. `ProcessResult` содержит `exitCode`, `timedOut` и `output`. `ProcessManager` не хранит состояние и объявляет `run(command, program, timeout) const`. Команда и байты должны оставаться доступны до конца вызова. Метод возвращает `ProcessResult` либо передаёт системное исключение верхнему уровню. Вопрос: почему `string_view` подходит для бинарника? Ответ: размер задан явно, нулевые байты остаются частью значения.

### `include/NetworkMonitor.h`

`Event.h` даёт модель события, `<string>` — вход и диагностику, `<vector>` — последовательность событий. `NetworkTrace` хранит `events` и `diagnostics`. `NetworkMonitor` с методом `parse(const std::string&) const` возвращает `NetworkTrace` и не имеет постоянных полей. Нераспознанные строки сохраняются для диагностики, а отдельные необычные данные могут вызвать исключение преобразования. Вопрос: хранится ли таблица дескрипторов между запусками? Ответ: она локальна вызову `parse`.

### `include/ReportManager.h`

`<filesystem>` и `<string>` задают типы пути и текста. Метод `save(const std::string&) const` сохраняет подготовленную строку и возвращает путь. Класс не хранит полей. Ошибки создания каталога или записи распространяются в виде исключений. Вопрос: где формируется основной текст? Ответ: в `Sandbox`, где доступны события и журнал.

### `include/Sandbox.h`

Включения `Event.h` и заголовков четырёх менеджеров объявляют используемые структуры и составные объекты. `<filesystem>` определяет пути, `<optional>` — наличие последнего результата, `<string>` и `<vector>` — строки и коллекции. `FileInfo` содержит `path`, `size` в байтах, `executable` и `type`. `Analysis` содержит `status`, `events` и `diagnostics`.

`Sandbox` хранит `binaryDirectory_` для допуска тестов, `selected_` как канонический путь, `lastAnalysis_` как необязательный результат, `journal_` как последовательность записей и четыре поля `process_`, `isolation_`, `monitor_`, `reports_`. Они обеспечивают композицию. Конструктор принимает каталог бинарника по значению, сохраняет его и начинает журнал. Методы `select()`, `fileInfo()`, `analyze()` и `saveReport()` выполняют прикладные действия. `selected()`, `lastAnalysis()` и `journal()` возвращают константные ссылки. `log()` закрыт и добавляет запись. Специальный деструктор не нужен, поля управляют своим временем жизни. Вопрос: зачем `optional` при наличии вектора? Ответ: пустой вектор возможен у результата прерванного анализа, `nullopt` означает отсутствие результата.

Во всех заголовках используется `#pragma once`. Пользовательских перечислений нет. Менеджеры, для которых конструкторы не объявлены, создаются стандартным способом по умолчанию.

## Точка входа и команды

### `src/main.cpp`

`ConsoleUI.h` и `Sandbox.h` дают два компонента. `<filesystem>` используется для канонического пути `/proc/self/exe`, `<iostream>` — для `std::cerr`. `int main()` не принимает аргументов. Создаёт ядро на стеке, находит его каталог через `parent_path()`, запускает временный `ConsoleUI`. При исключении возвращает 1. Без исключения выход из `main` означает 0. Точка входа вынесена отдельно, чтобы связать интерфейс с классами, сохранив независимость ядра.

Фрагмент `src/main.cpp`, строки 7–15.

```cpp
int main() {
    try {
        Sandbox sandbox(std::filesystem::canonical("/proc/self/exe").parent_path());
        ConsoleUI(sandbox).run();
    } catch (const std::exception& error) {
        std::cerr << "Не удалось запустить LinuxSandbox: " << error.what() << '\n';
        return 1;
    }
}
```

Строка `try` начинает защиту точки входа. Конструктор `Sandbox` получает путь к каталогу фактического исполняемого файла. Следующая строка создаёт интерфейс и запускает меню. `catch` получает исключение по константной ссылке, выводит сообщение и задаёт код выхода. Пример — запуск `./build/LinuxSandbox` из проекта. Вопрос: что произойдёт при недоступности `/proc/self/exe`? Ответ: ошибка `canonical` попадёт в `catch`.

### `src/ConsoleUI.cpp`

Заголовки `ConsoleUI.h` и `Sandbox.h` связывают реализацию с API. `<iostream>` даёт потоки, `<stdexcept>` — `runtime_error`, `<string>` — буфер команд. Неназванное пространство имён ограничивает видимость переменной `menu` и функции `printEvents`. Последняя принимает `const Analysis&` и ничего не возвращает. Она выводит статус и каждое событие, пропуская пустые дополнительные поля.

Фрагмент `src/ConsoleUI.cpp`, строки 39–56.

```cpp
void ConsoleUI::run() {
    for (;;) {
        std::cout << menu << "\nВыбранный файл: "
                  << (sandbox_.selected().empty() ? "не указан" : sandbox_.selected().string())
                  << "\nВведите номер команды: " << std::flush;

        std::string input;
        if (!std::getline(std::cin, input)) break;
        const auto first = input.find_first_not_of(" \t\r");
        const auto last = input.find_last_not_of(" \t\r");
        const auto command = first == std::string::npos ? "" : input.substr(first, last - first + 1);
        try {
            if (command == "0") break;
            if (command == "1") {
                std::cout << "Введите путь к файлу: " << std::flush;
                if (!std::getline(std::cin, input)) break;
                sandbox_.select(input);
                std::cout << "Файл выбран.\n";
```

Строки 40–49 повторяют показ меню, читают строку и подрезают пробелы, табуляцию и CR. В строках 50–56 команда 0 завершает цикл, команда 1 читает путь и передаёт его в `select`. Пункты 2–6 в строках 57–75 используют `fileInfo`, `analyze`, `lastAnalysis`, `journal` и `saveReport`. Неизвестная команда выводит подсказку, `catch` показывает ошибки операции. В строке 83 показывается конец сеанса. Пункты 4 и 6 без результата не выполняются. Пример — пункт 1 с путём `./build/test_tcp`, затем пункт 2. Вопрос: почему после анализа снова появляется меню? Ответ: `run()` возвращается к началу цикла, получив результат синхронной операции.

## Ядро и подготовка файла

### `src/Sandbox.cpp`

`Sandbox.h` объявляет ядро, `AllowedTests.h` даёт эталонные байты, `FileDescriptor.h` владеет файловыми ресурсами. `<array>` даёт буфер и сигнатуру, `<cerrno>` — `errno` и `EINTR`, `<chrono>` — секунды ожидания, `<ctime>` и `<iomanip>` — текущую дату и формат, `<sstream>` — построение строк, `<stdexcept>` — исключения, `<string_view>` — представление эталона, `<fcntl.h>` — `open` и флаги, `<sys/stat.h>` — `fstat` и `S_ISREG`.

`bytes()` в строках 22–25 принимает ссылку на массив, выводит размер параметром шаблона и возвращает представление после преобразования указателя. `timestamp()` в строках 47–54 получает время, использует `localtime_r` и возвращает строку с часовым поясом. `timeLimit` — постоянный интервал 5 секунд. Конструктор в 57–59 перемещает каталог и записывает начало сеанса. Простые методы доступа возвращают ссылки. `log()` в 137–139 добавляет отметку времени и сообщение.

Фрагмент `src/Sandbox.cpp`, строки 61–75.

```cpp
void Sandbox::select(const fs::path& path) {
    if (path.empty()) throw std::runtime_error("Укажите путь к файлу.");
    std::error_code error;
    const auto resolved = fs::canonical(path, error);
    if (error == std::errc::no_such_file_or_directory) {
        throw std::runtime_error("Файл по указанному пути не найден.");
    }
    if (error) throw std::runtime_error("Не удалось открыть указанный путь.");
    if (!fs::is_regular_file(resolved)) throw std::runtime_error("Ожидается обычный файл.");
    if (access(resolved.c_str(), R_OK) != 0) throw std::runtime_error("Файл недоступен для чтения.");

    selected_ = resolved;
    lastAnalysis_.reset();
    log("Выбран файл " + selected_.string());
}
```

`select(path)` возвращает `void`. Пустой путь, отсутствующий файл, иной сбой канонизации, каталог и непрочитываемый файл приводят к исключению. Канонизация приводит путь к абсолютному виду и разрешает компоненты и символические ссылки. Только после проверок сохраняется `resolved`, сбрасывается анализ и добавляется запись. При ошибке предыдущий выбор сохраняется. Пример — выбор относительного `./build/test_udp`.

Фрагмент `src/Sandbox.cpp`, строки 79–95.

```cpp
FileInfo Sandbox::fileInfo() const {
    if (selected_.empty()) throw std::runtime_error("Сначала выберите файл.");
    const FileDescriptor file(open(selected_.c_str(), O_RDONLY | O_CLOEXEC | O_NOFOLLOW | O_NONBLOCK));
    struct stat info{};
    if (file.get() < 0 || fstat(file.get(), &info) < 0 || !S_ISREG(info.st_mode)) {
        throw std::runtime_error("Выбранный файл недоступен для чтения.");
    }
    std::array<unsigned char, 4> magic{};
    ssize_t count;
    do {
        count = read(file.get(), magic.data(), magic.size());
    } while (count < 0 && errno == EINTR);
    if (count < 0) throw std::runtime_error("Не удалось прочитать заголовок файла.");
    const bool elf = count == 4 && magic == std::array<unsigned char, 4>{0x7f, 'E', 'L', 'F'};
    return {selected_, static_cast<std::uintmax_t>(info.st_size),
            access(selected_.c_str(), X_OK) == 0, elf ? "ELF" : "Обычный файл"};
}
```

`fileInfo()` открывает файл с `O_NOFOLLOW` для последнего компонента и `O_NONBLOCK`, затем использует `fstat`. Чтение четырёх байтов повторяется при `EINTR`. Сравнение сигнатуры распознаёт ELF. Возвращается `FileInfo`; тип отражает сигнатуру, а право исполнения проверяется отдельно через `access`. Пример — команда 2 показывает `ELF` для комплектного бинарника. Вопрос: гарантирует ли сигнатура валидный ELF? Ответ: она проверяет только первые четыре байта.

Фрагмент `src/Sandbox.cpp`, строки 27–45.

```cpp
void verifyFile(const fs::path& path, std::string_view expected) {
    const FileDescriptor file(open(path.c_str(), O_RDONLY | O_CLOEXEC | O_NOFOLLOW | O_NONBLOCK));
    struct stat info{};
    if (file.get() < 0 || fstat(file.get(), &info) < 0 || !S_ISREG(info.st_mode) ||
        info.st_size < 0 || static_cast<std::uintmax_t>(info.st_size) != expected.size()) {
        throw std::runtime_error("Файл отличается от бинарника, собранного вместе с LinuxSandbox.");
    }

    std::array<char, 4096> buffer{};
    for (;;) {
        const auto count = read(file.get(), buffer.data(), buffer.size());
        if (count < 0 && errno == EINTR) continue;
        if (count == 0 && expected.empty()) return;
        if (count <= 0 || expected.substr(0, count) != std::string_view(buffer.data(), count)) {
            throw std::runtime_error("Не удалось подтвердить содержимое тестового файла.");
        }
        expected.remove_prefix(static_cast<std::size_t>(count));
    }
}
```

`verifyFile(path, expected)` открывает обычный файл и проверяет размер. Читает блоками по 4096 байт и сдвигает представление `expected` после совпадения. Подтверждает конец файла и эталона одновременно. При ошибке, несовпадении или неожиданном конце выбрасывается исключение. Эталон передаётся как строковое представление, но сравнение бинарное. Пример — вызов внутри `analyze()` после проверки пути. Вопрос: зачем сравнивать файл, если запускаются встроенные байты? Ответ: проверка допуска подтверждает согласованность выбранного файла с комплектом конкретной сборки.

Фрагмент `src/Sandbox.cpp`, строки 97–115.

```cpp
const Analysis& Sandbox::analyze() {
    lastAnalysis_.emplace();
    auto& analysis = *lastAnalysis_;
    try {
        const auto info = fileInfo();
        std::string_view approved;
        if (info.path == binaryDirectory_ / "test_tcp") approved = bytes(allowed::tcp);
        if (info.path == binaryDirectory_ / "test_udp") approved = bytes(allowed::udp);
        if (approved.empty()) {
            throw std::runtime_error("Разрешены только test_tcp и test_udp рядом с LinuxSandbox.");
        }
        if (!info.executable) throw std::runtime_error("Тестовый файл не имеет права на исполнение.");
        verifyFile(info.path, approved);
        const auto command = isolation_.command();
        log("Начат анализ " + info.path.filename().string() + " через Bubblewrap и strace.");
        const auto result = process_.run(command, approved, timeLimit);
        auto trace = monitor_.parse(result.output);
        analysis.events = std::move(trace.events);
        analysis.diagnostics = std::move(trace.diagnostics);
```

`analyze()` возвращает ссылку на новый `Analysis` внутри `optional`. Комплект определяется одновременно именем и каноническим положением рядом с `LinuxSandbox`. Проверяется бит исполнения, затем `verifyFile`, `isolation_.command()` и `process_.run()`. Сырые данные разбираются `monitor_.parse()`. Строки 116–125 выставляют статус в порядке тайм-аут, ненулевой выход, отсутствие событий, успех. В 126–131 исключение превращается в отказ, пишутся итог и диагностика. Вопрос: когда исчезнет возвращённая ссылка? Ответ: при успешной смене файла или замене последнего анализа прежний объект уничтожается, удерживать ссылку после этих действий нельзя.

Фрагмент `src/Sandbox.cpp`, строки 141–153.

```cpp
fs::path Sandbox::saveReport() {
    if (!lastAnalysis_ || selected_.empty()) {
        throw std::runtime_error("Выберите файл и запустите анализ перед сохранением отчёта.");
    }
    const auto& analysis = *lastAnalysis_;
    std::ostringstream report;
    report << "LINUX SANDBOX v0.1 — отчёт\n"
           << "Дата: " << timestamp() << '\n'
           << "Файл: " << selected_.string() << '\n'
           << "Статус: " << analysis.status << "\n\n"
           << "Сетевые события (" << analysis.events.size() << ")\n"
           << "PID\tВызов\tПротокол\tIP\tПорт\tСтатус\tРезультат\n";
    const auto field = [](const std::string& value) { return value.empty() ? "-" : value; };
```

`saveReport()` проверяет результат попытки анализа и путь. Готовит отчёт с датой, статусом и заголовками колонок. В 154–159 проходит по `vector<Event>`, ставит дефис вместо пустых полей, разделяет значения табуляцией. В 161–165 добавляет журнал, вызывает `reports_.save()` и записывает факт сохранения в текущем сеансе. Пример — пункт 6 после TCP-теста. Даже прерванный анализ может иметь отдельный отчёт. Вопрос: почему сообщение о сохранении текущего файла отсутствует в самом файле? Ответ: оно появляется в журнале после записи.

## Запуск, изоляция и анализ трассы

### `src/IsolationManager.cpp`

`IsolationManager.h` задаёт интерфейс. `<stdexcept>` нужен для диагностических исключений. `<unistd.h>` объявляет `getuid`, `geteuid` и `access`. `command()` проверяет эффективный UID и доступ к фиксированным `bwrap` и `strace`. При успехе возвращает вектор аргументов, запуском занимается `ProcessManager`.

Фрагмент `src/IsolationManager.cpp`, строки 14–26.

```cpp
    // fd 3 содержит копию разрешённого ELF, /proc принадлежит новому PID namespace.
    return {
        "/usr/bin/bwrap", "--unshare-all", "--die-with-parent", "--new-session",
        "--cap-drop", "ALL", "--clearenv", "--setenv", "LC_ALL", "C",
        "--ro-bind", "/usr/lib", "/usr/lib", "--symlink", "usr/lib", "/lib",
        "--ro-bind-try", "/usr/lib64", "/usr/lib64", "--symlink", "usr/lib64", "/lib64",
        "--ro-bind", "/usr/bin/strace", "/usr/bin/strace",
        "--proc", "/proc", "--dev", "/dev", "--chdir", "/",
        "--perms", "0555", "--ro-bind-data", "3", "/sample",
        "--", "/usr/bin/strace", "-f", "-A", "-yy", "-e", "trace=%network",
        "-o", "/proc/self/fd/2", "--", "/sample"
    };
}
```

Строки 16–17 запрашивают пространства имён, завершение с родителем, отдельный сеанс, сброс capabilities и локаль `C`. В 18–22 монтируются библиотеки и strace в режиме чтения, создаются приватные `/proc` и `/dev`, из fd 3 формируется `/sample` с `0555`. В 23–24 strace запускается внутри среды и пишет трассу в дескриптор 2. Пример — вызов `isolation_.command()` перед запуском TCP-теста. Вопрос: виден ли домашний каталог образцу? Ответ: политика запуска его не монтирует, управление ресурсами остаётся функцией общего ядра Linux.

### `src/ProcessManager.cpp`

`ProcessManager.h` задаёт типы, `FileDescriptor.h` управляет временем жизни FD. `<array>` — буфер, `<cerrno>` — системные причины ошибок, `<csignal>` — `SIGKILL`, `<system_error>` — исключение с кодом, `<thread>` — десятимиллисекундное ожидание, `<fcntl.h>` — флаги и настройка FD, `<sys/mman.h>` — `memfd_create`, `<sys/wait.h>` — ожидание и разбор статуса. Включённый `<stdexcept>` непосредственно в выражениях файла не используется. Локальная `systemError(action)` выбрасывает `std::system_error` с `errno` и человекочитаемым действием.

Фрагмент `src/ProcessManager.cpp`, строки 21–38.

```cpp
ProcessResult ProcessManager::run(const std::vector<std::string>& command,
                                  std::string_view program,
                                  std::chrono::milliseconds timeout) const {
    // Дескриптор 3 зарезервирован для передачи проверенного бинарника Bubblewrap.
    const FileDescriptor reserved(open("/dev/null", O_RDWR | O_CLOEXEC));
    if (reserved.get() < 0) systemError("Открытие /dev/null");
    const FileDescriptor sample(memfd_create("sample", MFD_CLOEXEC));
    const FileDescriptor output(memfd_create("trace", MFD_CLOEXEC));
    if (sample.get() < 0 || output.get() < 0) systemError("Создание временного файла");
    if (fcntl(output.get(), F_SETFL, O_APPEND) < 0) systemError("Настройка трассировки");

    while (!program.empty()) {
        const auto written = write(sample.get(), program.data(), program.size());
        if (written < 0 && errno == EINTR) continue;
        if (written <= 0) systemError("Копирование тестового файла");
        program.remove_prefix(static_cast<std::size_t>(written));
    }
    if (lseek(sample.get(), 0, SEEK_SET) < 0) systemError("Чтение копии тестового файла");
```

Аргументы `run` — непустая команда Bubblewrap, представление проверенного бинарника и тайм-аут в миллисекундах. Вначале открывается `/dev/null`, создаются два анонимных файла в памяти и выставляется `O_APPEND` для вывода. Цикл учитывает частичную запись и `EINTR`, затем `lseek` возвращает смещение образца к началу. Пример — `Sandbox` передаёт эталон TCP и лимит 5 с.

Фрагмент `src/ProcessManager.cpp`, строки 46–59.

```cpp
    const auto pid = fork();
    if (pid < 0) systemError("Создание процесса");
    if (pid == 0) {
        if (dup2(output.get(), STDERR_FILENO) < 0 ||
            dup2(reserved.get(), STDIN_FILENO) < 0 ||
            dup2(reserved.get(), STDOUT_FILENO) < 0 ||
            (sample.get() == 3 ? fcntl(3, F_SETFD, 0) : dup2(sample.get(), 3)) < 0) {
            _exit(126);
        }
        execve(arguments.front(), arguments.data(), environment);
        constexpr char message[] = "Не удалось запустить Bubblewrap.\n";
        (void)write(STDERR_FILENO, message, sizeof(message) - 1);
        _exit(127);
    }
```

Перед `fork()` в 40–44 массив аргументов завершается `nullptr`, окружение содержит только `LC_ALL=C`. Ошибка `fork` приводит к исключению. В дочерней ветви `dup2` направляет stdin/stdout в `/dev/null`, stderr в memfd и публикует копию образца как fd 3. `execve` замещает процесс Bubblewrap. Настройка FD может завершиться кодом 126, ошибка `execve` — 127. Вопрос: используется ли shell? Ответ: передаётся массив аргументов непосредственно `execve`.

Фрагмент `src/ProcessManager.cpp`, строки 61–78.

```cpp
    const auto deadline = std::chrono::steady_clock::now() + timeout;
    int status = 0;
    bool timedOut = false;
    for (;;) {
        const auto result = waitpid(pid, &status, WNOHANG);
        if (result == pid) break;
        if (result < 0 && errno != EINTR) systemError("Ожидание Bubblewrap");
        if (std::chrono::steady_clock::now() >= deadline) {
            // Bubblewrap передаёт завершение процессам песочницы через --die-with-parent.
            timedOut = true;
            kill(pid, SIGKILL);
            while (waitpid(pid, &status, 0) < 0) {
                if (errno != EINTR) systemError("Ожидание после остановки Bubblewrap");
            }
            break;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
```

Родитель опрашивает `waitpid`, прерываясь на 10 мс между проверками. Монотонные часы не зависят от перевода настенного времени. При истечении интервала отправляется `SIGKILL`, затем ожидается завершение. Досрочное исключение при системной ошибке передаётся вызывающему коду. Строки 80–91 читают `pread` порциями в строку максимум до 128 КиБ и добавляют отметку, если лимит достигнут. Строки 93–94 возвращают `ProcessResult`, представляя сигнал как `128 + номер`. Вопрос: ограничивает ли 128 КиБ запись трассировщика? Ответ: это предел чтения в приложении, запись в memfd может накопить больше.

### `src/NetworkMonitor.cpp`

`NetworkMonitor.h` определяет структуры. `<regex>` реализует шаблоны, `<sstream>` даёт построчный вход, `<unordered_map>` хранит соответствие PID и дескриптора протоколу. `parse(const string& output)` возвращает события и диагностические строки. В 8–13 объявлены статические регулярные выражения для вызова, порта, адреса, аннотации FD, конечной точки и первого аргумента. Выражения создаются один раз. Строки 15–25 создают локальные структуры и отделяют строки, не совпавшие с шаблоном. Линии обычного завершения исключаются из диагностики.

Фрагмент `src/NetworkMonitor.cpp`, строки 27–50.

```cpp
        const std::string pid = match[1];
        const std::string call = match[2];
        const std::string arguments = match[3];
        const std::string code = match[4];
        const std::string tail = match[5];
        const bool success = code.front() != '-';
        const std::string evidence = arguments + tail;
        std::string protocol;
        if (evidence.find("<TCP") != std::string::npos ||
            (call == "socket" && evidence.find("IPPROTO_TCP") != std::string::npos)) {
            protocol = "TCP";
        } else if (evidence.find("<UDP") != std::string::npos ||
                   (call == "socket" && evidence.find("IPPROTO_UDP") != std::string::npos)) {
            protocol = "UDP";
        }

        std::smatch field;
        if (protocol.empty() && std::regex_search(arguments, field, firstFd)) {
            const auto known = protocols.find(pid + ':' + field[1].str());
            if (known != protocols.end()) protocol = known->second;
        }
        if (success && (call == "socket" || call == "accept" || call == "accept4")) {
            protocols[pid + ':' + code] = protocol;
        }
```

В строках 27–33 группы главного шаблона дают PID, имя, аргументы, число и хвост. По знаку определяется успех. В 35–41 ищется аннотация протокола либо аргумент `IPPROTO_*`. В 43–50 протокол дополняется по ключу `PID:FD`, при создании или принятии сокета обновляется таблица. Состояние действительно только в пределах данного текста.

Фрагмент `src/NetworkMonitor.cpp`, строки 52–73.

```cpp
        std::string ip;
        std::string service;
        if (std::regex_search(arguments, field, address)) ip = field[1];
        if (std::regex_search(arguments, field, port)) service = field[1];
        if (ip.empty() && std::regex_search(arguments, field, descriptor)) {
            // Для соединённого сокета берём адрес собеседника, если он декодирован.
            std::string decoded = field[1];
            const auto peer = decoded.find("->");
            if (peer != std::string::npos) decoded.erase(0, peer + 2);
            if (std::regex_match(decoded, field, endpoint)) {
                ip = field[1].matched ? field[1].str() : field[2].str();
                service = field[3];
            }
        }

        // Декодирование fd не входит в результат системного вызова.
        const auto detail = tail.find_first_not_of(' ');
        const auto result = code + (detail != std::string::npos && tail[detail] != '<' ? tail : "");
        trace.events.push_back({pid.empty() ? 0 : std::stoi(pid), call, protocol, ip, service, result, success});
    }
    return trace;
}
```

Строки 54–55 получают адрес и порт из аргументов. Если адреса нет, 56–64 разбирают конечную точку дескриптора и выбирают удалённую часть после `->`. В 67–70 аннотация результата отбрасывается, код и диагностический хвост сохраняются, затем добавляется `Event`. Пример — успешное подключение с `127.0.0.1` и `=> 0` либо отказ `-1 ECONNREFUSED`. Неизвестный формат строки может перейти в диагностику. Незавершённые и продолженные вызовы отдельно не соединяются. Вопрос: почему у некоторых событий нет адреса? Ответ: операция или текст трассы могут не содержать подходящего адреса.

### `src/ReportManager.cpp`

`ReportManager.h` задаёт контракт, `FileDescriptor.h` закрывает файл. `<cerrno>` даёт `EINTR` и `EIO`, `<system_error>` представляет ошибки ОС. `<stdexcept>` включён, но в выражениях файла напрямую не используется. `save(text)` создаёт каталог и файл `mkstemps`, который заменяет шесть `X` случайной частью имени, сохраняет суффикс `.txt` и выдаёт дескриптор. При ошибке создания метод выбрасывает исключение.

Фрагмент `src/ReportManager.cpp`, строки 16–28.

```cpp
    std::size_t offset = 0;
    while (offset < text.size()) {
        const auto written = write(file.get(), text.data() + offset, text.size() - offset);
        if (written < 0 && errno == EINTR) continue;
        if (written <= 0) {
            const int error = written < 0 ? errno : EIO;
            unlink(path);
            throw std::system_error(error, std::generic_category(), "Запись отчёта");
        }
        offset += static_cast<std::size_t>(written);
    }
    return path;
}
```

`offset` хранит количество уже записанных байтов. Цикл сохраняет оставшуюся часть строки, прерывание повторяет операцию. При необратимой ошибке путь удаляется, возвращается исключение. После успеха возвращённый путь относителен текущему каталогу. Пример — `reports/analysis-XXXXXX.txt` с правами `0600`. Вопрос: как исключается перезапись прежнего отчёта? Ответ: `mkstemps` создаёт новое уникальное имя.

## Комплектные безопасные тесты и сборка

### `tests/test_tcp.cpp`

`FileDescriptor.h` автоматически закрывает сокеты. `<cerrno>` нужен для проверки `ECONNREFUSED`, `<cstdio>` — для `perror` и `fputs`. `<netinet/in.h>` содержит IPv4-типы, номер протокола и сетевые преобразования, `<sys/socket.h>` — операции сокетов. Параметров командной строки нет. `main()` возвращает 0 при ожидаемом исходе, иначе 1. Локальная `fail(operation)` печатает причину ошибки.

Строки 15–29 заполняют `sockaddr_in`, создают слушающий и клиентский TCP-дескрипторы, привязывают сервер к loopback с портом 0, получают фактический порт, включают `listen` и делают `connect`. Строки 30–38 принимают соединение, отправляют символ `T` и сверяют полученный байт. `MSG_NOSIGNAL` предотвращает сигнал `SIGPIPE` в данной отправке. Конец внутреннего блока закрывает сокеты.

Фрагмент `tests/test_tcp.cpp`, строки 41–49.

```cpp
    // После закрытия слушающего сокета ожидается отказ на том же порту.
    const FileDescriptor retry(socket(AF_INET, SOCK_STREAM, IPPROTO_TCP));
    if (retry.get() < 0) return fail("socket TCP");
    if (connect(retry.get(), reinterpret_cast<const sockaddr*>(&address), sizeof(address)) == 0) {
        std::fputs("TCP: ожидался отказ подключения\n", stderr);
        return 1;
    }
    return errno == ECONNREFUSED ? 0 : fail("повторный connect TCP");
}
```

Повторный `connect` на том же порту должен завершиться отказом. Успех вопреки ожиданию делает результат теста отрицательным. Строка 48 принимает именно `ECONNREFUSED`. Пример использования — команда 3 программы с выбранным `test_tcp` в Bubblewrap. Вопрос: почему наблюдается ошибка при успешном тесте? Ответ: тест намеренно проверяет ожидаемое поведение закрытого порта.

### `tests/test_udp.cpp`

`FileDescriptor.h` владеет сокетами, `<cstdio>` — диагностика `perror`, `<netinet/in.h>` — IPv4-адреса и UDP, `<sys/socket.h>` — вызовы обмена. `main()` создаёт два датаграммных сокета, связывает принимающий с loopback, получает выданный ядром порт. Цель вынесена в отдельный файл и отдельный бинарник для проверки независимого UDP-сценария.

Фрагмент `tests/test_udp.cpp`, строки 27–36.

```cpp
    constexpr char sent = 'U';
    if (sendto(client.get(), &sent, 1, 0, reinterpret_cast<const sockaddr*>(&address), sizeof(address)) != 1)
        return fail("sendto UDP");
    char received = 0;
    sockaddr_in peer{};
    length = sizeof(peer);
    if (recvfrom(server.get(), &received, 1, 0, reinterpret_cast<sockaddr*>(&peer), &length) != 1)
        return fail("recvfrom UDP");
    return received == sent && peer.sin_addr.s_addr == htonl(INADDR_LOOPBACK) ? 0 : 1;
}
```

Строки 27–29 отправляют один байт `U` на адрес принимающего сокета. Строки 30–34 читают датаграмму и адрес отправителя. Строка 35 проверяет содержимое и loopback, возвращая 0 при успехе. Вопрос: почему отсутствуют `listen()` и `accept()`? Ответ: сценарий обменивается независимыми UDP-датаграммами. Обе тестовые программы используют локальные адреса и исполняются в предусмотренной изоляции.

### `CMakeLists.txt` и `cmake/EmbedTests.cmake`

Сборка проверяет Linux и конфигурирует C++17 без расширений стандарта в режиме компилятора. Цели `test_tcp` и `test_udp` собираются перед использованием их содержимого. `add_custom_command` передаёт пути тестов и выходного файла скрипту. Скрипт читает бинарники в HEX, преобразует пары в `0xNN`, разбивает список по восемь байтов и пишет `inline constexpr unsigned char` в пространство имён `allowed`. `LinuxSandbox` компилируется с полученным заголовком. Флаги предупреждений применены ко всем трём целям. Команда `ctest` показывает отсутствие зарегистрированных проверок, поэтому контрольные сценарии выполняются через прототип. Вопрос: зачем пересобирать все цели? Ответ: бинарники и встроенные байты должны соответствовать друг другу.

## Основы C++ на примерах проекта

| Тема | Как проявляется |
| --- | --- |
| `main`, `.cpp`, `.h`, `.hpp`, `#include` | `main` запускает приложение. `.cpp` компилируются отдельно, `.h` содержат интерфейсы. `.hpp` мог бы обозначать заголовок C++, в этом проекте таких файлов нет. `#include` добавляет содержимое заголовка на этапе препроцессора. |
| Пространства имён | `std` принадлежит стандартной библиотеке. `fs` — псевдоним `std::filesystem`. `allowed` содержит эталонные байты. Неназванное пространство ограничивает имя текущим `.cpp`. |
| Классы, объекты, структуры | `Sandbox` — класс, `sandbox` в `main` — объект. `Event` и `FileInfo` — структуры со значениями и открытыми полями. Конструкторы инициализируют объекты, деструкторы освобождают их ресурсы. |
| ООП | Инкапсуляция защищает `selected_` и журнал. Абстракция скрывает POSIX за `ProcessManager::run`. Композиция включила менеджеры по значению в `Sandbox`. Пользовательского наследования и динамического полиморфизма нет. |
| Ссылки и указатели | `Sandbox&` связывает интерфейс с существующим объектом. `const std::string&` не копирует строку. `char*` используется для `argv`; `nullptr` обозначает конец массива указателей. |
| `const`, `static`, `auto` | Константные методы не меняют состояние объекта через `this`. `static const` хранит регулярное выражение между вызовами. `auto` выводит тип выражения на этапе компиляции. |
| Коллекции | `string` содержит текст или байты. `vector` хранит упорядоченные события и журналы. `array` задаёт фиксированный буфер. `unordered_map` индексирует протокол по ключу. `map` была бы упорядоченной по ключу, её в проекте нет. |
| C++17 | `optional` описывает отсутствие анализа, `filesystem` — пути, `string_view` — представление байтов, `inline constexpr` — данные эталона в заголовке. `regex_match` сопоставляет всю строку, `regex_search` ищет подстроку. |
| `enum class`, умные указатели | `enum class` даёт типобезопасное перечисление. Пользовательских перечислений нет. `unique_ptr` и `shared_ptr` не используются, дескриптор имеет специальную обёртку RAII. |
| Исключения и ресурсы | `throw` передаёт ошибку месту обработки. Локальные объекты уничтожаются при выходе из области видимости. В `main` и меню есть обработчики `std::exception`. |
| Стек и динамическая память | Локальные объекты `Sandbox`, `FileDescriptor` и буферы `array` имеют автоматическое время жизни. `vector` и `string` управляют своим динамическим хранением. `memfd` — объект ядра, доступный через дескриптор. |

## Основы Linux и сетевых сокетов

Процесс — запущенный экземпляр программы со своим состоянием. PID — идентификатор процесса в данном пространстве имён. `fork` создаёт дочерний процесс, `execve` загружает другую программу в тот же процесс, `waitpid` получает статус дочернего. Сигнал — способ уведомить процесс через ОС. `SIGKILL` останавливает его без обработчика. Код 0 обычно означает успех, ненулевой код отражает ошибку, если так определила программа. При принудительном завершении проект показывает `128 + номер сигнала`.

Файловый дескриптор — целое число, по которому процесс обращается к открытому объекту ядра. `0`, `1`, `2` соответствуют stdin, stdout и stderr. `dup2` связывает целевой номер с уже открытым объектом. Права `0600` дают владельцу чтение и запись, `0555` открывает чтение и исполнение. Пространства имён меняют видимость сети, процессов и монтирований. Bubblewrap формирует их, а `strace` наблюдает переходы приложения к ядру. Обычные права, конкретная схема монтирований и проверка образца дополняют эту границу.

Сокет — конечная точка обмена, доступная через FD. IPv4 использует 32-битные адреса, IPv6 — 128-битные. Порт определяет конечную точку транспортного уровня на данном адресе. `127.0.0.1` — IPv4 loopback. В отдельном сетевом пространстве он относится к этой среде. Код тестов вызывает `htonl(INADDR_LOOPBACK)`, приводя значение к сетевому порядку байтов.

| Вызов | Назначение и видимость в LinuxSandbox |
| --- | --- |
| `socket()` | Создаёт сокет. В трассе видны семейство, тип, протокол и возвращённый FD или ошибка. |
| `bind()` и `getsockname()` | Назначают локальный адрес, затем запрашивают фактический адрес. Порт 0 в `bind` означает автоматическое назначение. |
| `listen()` и `accept()` | Первый переводит TCP-сокет в режим ожидания, второй возвращает FD нового соединения. Фиксируются при наличии в трассе. |
| `connect()` | Устанавливает соединение TCP. В тесте видны успешный вызов и `ECONNREFUSED`. |
| `send()` и `recv()` | Работа с соединённым сокетом. Уровень libc может использовать `sendto` и `recvfrom`, как в проверенной сборке TCP. |
| `sendto()` и `recvfrom()` | Отправка или приём датаграммы с адресом. Код результата сообщает число переданных байтов или ошибку. |
| `close()` | Закрывает дескриптор. Поскольку выбран `%network`, обычный `close` не является систематически регистрируемым событием прототипа. |

TCP предоставляет поток после установления соединения. UDP работает с отдельными датаграммами. В событии доступны PID, имя вызова, протокол, адрес, порт, результат и текстовый класс удачи, когда данные есть в трассе. Из этих записей видно, какие операции наблюдались и что ответило ядро. Для анализа заголовков и последовательности пакетов нужен иной источник данных. IPv6 учтён частично в регулярных выражениях, собственные тесты используют IPv4. Изоляция и трассировка имеют разные роли и используют общее ядро Linux.

## Полный путь сеанса и схема вызовов

1. `src/main.cpp`, `main()` получает каталог исполняемого файла и создаёт `Sandbox`.
2. `src/Sandbox.cpp`, `Sandbox::Sandbox()` открывает журнал. `src/ConsoleUI.cpp`, `ConsoleUI::run()` показывает меню.
3. `ConsoleUI::run()` читает команду. Команда 1 вызывает `Sandbox::select()`.
4. `Sandbox::select()` проверяет путь и читаемость. Команда 2 отдельно вызывает `Sandbox::fileInfo()`.
5. Команда 3 вызывает `Sandbox::analyze()`, который заново получает свойства, проверяет допуск и вызывает `verifyFile()`.
6. `src/IsolationManager.cpp`, `IsolationManager::command()` формирует параметры. `src/ProcessManager.cpp`, `ProcessManager::run()` готовит дескрипторы и выполняет `fork`/`execve`.
7. Bubblewrap создаёт среду и запускает strace с `/sample`. Родитель контролирует время и получает вывод после завершения.
8. `src/NetworkMonitor.cpp`, `NetworkMonitor::parse()` преобразует строки в `NetworkTrace`. `Sandbox::analyze()` определяет статус и обновляет журнал.
9. `ConsoleUI` выводит результат. Команда 4 отображает записи, команда 5 — журнал.
10. Команда 6 вызывает `Sandbox::saveReport()` и `ReportManager::save()` из `src/ReportManager.cpp`.
11. Дескрипторы завершают время жизни в своих областях видимости. Команда 0 или EOF завершает `ConsoleUI::run()`, затем `main()`.

На рисунке 1 показаны вызовы после команды 3. Редактируемый исходник доступен в `docs/diagrams/sequence.dot`, также доступна [схема SVG](diagrams/sequence.svg).

<div data-figure="sequence"></div>
