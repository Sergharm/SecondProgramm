#include "IsolationManager.h"

#include <stdexcept>
#include <unistd.h>

std::vector<std::string> IsolationManager::command() const {
    if (geteuid() == 0 || getuid() != geteuid()) {
        throw std::runtime_error("Анализ нужно запускать от обычного пользователя.");
    }
    if (access("/usr/bin/bwrap", X_OK) != 0 || access("/usr/bin/strace", X_OK) != 0) {
        throw std::runtime_error("Нужны исполняемые /usr/bin/bwrap и /usr/bin/strace.");
    }

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
