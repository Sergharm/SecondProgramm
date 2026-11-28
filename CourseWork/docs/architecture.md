# Архитектура LinuxSandbox

## Версия 0.1

- **ConsoleUI** читает команды и отображает данные ядра. **Sandbox** хранит выбранный файл, результат последнего анализа и журнал сеанса.
- **Sandbox** читает метаданные файла и проверяет путь и содержимое комплектного бинарника. CMake встраивает эталонные байты двух тестов в LinuxSandbox.
- **IsolationManager** формирует вызов Bubblewrap. В среду попадают системные библиотеки и strace в режиме чтения, приватные `/proc`, `/dev` и проверенная копия образца. Каталог проекта и домашний каталог не монтируются.
- **ProcessManager** передаёт образец через временный файловый дескриптор, контролирует завершение Bubblewrap и считывает трассу и диагностику. При истечении пяти секунд он завершает Bubblewrap. Параметр `--die-with-parent` связывает время жизни процессов среды с Bubblewrap.
- **NetworkMonitor** преобразует завершённые сетевые вызовы strace в события. **ReportManager** сохраняет текст отчёта в новый файл с правами `0600`.

```mermaid
flowchart TD
    User["Пользователь"] --> UI["ConsoleUI"]
    UI --> Core["Sandbox"]
    Build["Два тестовых бинарника сборки"] --> Core
    Core --> Iso["IsolationManager"]
    Iso --> PM["ProcessManager"]
    Core --> PM
    PM --> BW["Bubblewrap"]
    subgraph Namespace["Изолированная среда"]
        ST["strace"]
        Sample["test_tcp или test_udp"]
        ST --> Sample
    end
    BW --> ST
    ST -->|Сетевая трасса| PM
    PM --> NM["NetworkMonitor"]
    NM -->|Сетевые события| Core
    Core --> RM["ReportManager"]
    RM --> Report["Текстовый отчёт"]
```

## Перспектива с Qt 6 Widgets

Графическое окно будет обращаться к ядру через контроллер. Анализ будет выполняться в рабочем потоке, а результаты будут передаваться в поток интерфейса.

```mermaid
flowchart TD
    User["Пользователь"] --> Window["Главное окно Qt 6 Widgets"]
    Window --> Controller["Контроллер анализа"]
    Controller --> Worker["Рабочий поток"]
    Worker --> Core["Ядро Sandbox"]
    Core --> Monitor["Мониторинг"]
    Core --> Export["Экспорт отчётов"]
    Worker -->|Сигналы с результатами| Controller
    Controller --> Table["Таблица сетевых событий"]
    Controller --> Processes["Представление процессов"]
    Controller --> Log["Журнал"]
```

Описание параметров инструментов доступно в документации проектов. [Bubblewrap](https://github.com/containers/bubblewrap/blob/main/bwrap.xml) · [strace](https://strace.io/)
