# Скриншоты к сведениям о ходе выполнения курсовой работы

Снимки получены из окон QTerminal и Visual Studio Code в отдельном графическом сеансе Kali Linux. Файлы сохранены в `docs/progress_report/screenshots/`. Обрезка сохраняет фактический вывод, названия файлов и номера строк. Порты, PID и имя отчёта относятся к конкретному сеансу и могут изменяться при повторении.

## Снимки работающей программы

Соберите LinuxSandbox и контрольные образцы одной конфигурацией. Запускайте приложение от обычного пользователя из каталога сборки.

```sh
cmake -S . -B build-progress -DCMAKE_BUILD_TYPE=Release
cmake --build build-progress --parallel 2
cd build-progress
mkdir -p evidence
printf '%s\n' 'Безопасный текстовый файл для проверки формата.' > evidence/plain.txt
cp ./Release/test_tcp evidence/unapproved_elf
test ! -e evidence/missing.elf
./Release/LinuxSandbox
```

Используйте только образцы test_tcp и test_udp из каталога Release этой сборки. При отказе Bubblewrap в настройке изоляции прекратите испытание. Копию test_tcp с именем unapproved_elf и текстовый файл plain.txt используют только для проверки отказа через меню. Для обработки отсутствующего пути выберите `./evidence/missing.elf`.

| Рисунок | Файл | Действия и область снимка |
| --- | --- | --- |
| 1 | `01_menu.png` | Запустить анализатор. Захватить заголовок версии, меню и приглашение ввода. |
| 2 | `02_file.png` | Выбрать `./Release/test_tcp` командой 1, затем выполнить 2. Захватить подтверждение выбора и свойства файла. |
| 3 | `03_tcp.png` | Выполнить 3 и 4 для test_tcp. Захватить список событий с успешным connect и отказом ECONNREFUSED. |
| 4 | `04_udp.png` | Выбрать `./Release/test_udp`, выполнить 3 и 4. Захватить строки sendto и recvfrom и количество событий. |
| 5 | `05_journal.png` | После двух анализов выполнить 5. Захватить последовательность записей сеанса. |
| 6 | `06_save.png`, `06_report.png` | Выполнить 6, захватить путь сохранения. После выхода открыть указанный TXT командой `cat` и снять данные отчёта. |
| 7 | `07_input.png`, `07_missing.png`, `07_format.png`, `07_text_refused.png`, `07_elf.png` | Ввести команду 99, выбрать по очереди отсутствующий путь, текстовый файл и копию test_tcp с другим именем. Для двух существующих файлов отдельно показать свойства и отказ в запуске. Захватить соответствующие ответы. |
| 8 | `08_net.png` | Заранее получить `/proc/net/dev` в отдельном Bubblewrap. Снять вывод с единственным интерфейсом lo. |

Для независимой проверки в каталоге сборки создайте `evidence/` и снимите результат команды `cat evidence/net-isolation.txt` после успешного выполнения следующей команды.

```sh
mkdir -p evidence
/usr/bin/bwrap --unshare-all --die-with-parent --new-session --cap-drop ALL \
  --clearenv --setenv LC_ALL C --ro-bind /usr/lib /usr/lib --symlink usr/lib /lib \
  --ro-bind-try /usr/lib64 /usr/lib64 --symlink usr/lib64 /lib64 \
  --ro-bind /usr/bin/cat /usr/bin/cat --proc /proc --dev /dev --chdir / \
  -- /usr/bin/cat /proc/net/dev > evidence/net-isolation.txt
```

Рисунки 9 и 10 получены из редактируемых Graphviz-файлов в каталоге `diagrams/`. Для перерасчёта используйте `neato -n2 -Tsvg` и `neato -n2 -Tpng` для каждого файла DOT.

## Снимки исходного кода

В Visual Studio Code откройте соответствующий файл в `CourseWork`. Установите читаемый размер текста и оставьте видимыми вкладку и номера строк. Закройте боковые панели и всплывающие уведомления. Для перехода к первому номеру строки используйте `Ctrl+G`. Захватите окно редактора и обрежьте только пустые поля снаружи указанного участка.

| Рисунок | Участок кода | Файл снимка |
| --- | --- | --- |
| 11 | `src/main.cpp`, строки 1–15 | `code-main.png` |
| 12 | `src/ConsoleUI.cpp`, строки 31–63 | `code-console.png` |
| 13 | `src/Sandbox.cpp`, строки 97–118 | `code-sandbox.png` |
| 14 | `src/IsolationManager.cpp`, строки 6–26 | `code-isolation.png` |
| 15 | `src/ProcessManager.cpp`, строки 40–59 | `code-process.png` |
| 16 | `src/ProcessManager.cpp`, строки 61–78 | `code-timeout.png` |
| 17 | `src/NetworkMonitor.cpp`, строки 28–50 | `code-network.png` |
| 18 | `src/NetworkMonitor.cpp`, строки 52–73 | `code-endpoints.png` |
| 19 | `include/Event.h`, строки 1–13 | `code-event.png` |
| 20 | `include/FileDescriptor.h`, строки 1–16 | `code-descriptor.png` |
| 21 | `src/ReportManager.cpp`, строки 8–28 | `code-report.png` |
| 22 | `tests/test_tcp.cpp`, строки 19–39 | `code-tcp.png` |
| 23 | `tests/test_udp.cpp`, строки 15–36 | `code-udp.png` |

Повторная сборка документа из корня проекта выполняется командой `python3 scripts/generate_progress_report.py` при наличии Times New Roman и подготовленных снимков.
