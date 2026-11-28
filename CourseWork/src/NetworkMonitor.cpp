#include "NetworkMonitor.h"

#include <regex>
#include <sstream>
#include <unordered_map>

NetworkTrace NetworkMonitor::parse(const std::string& output) const {
    static const std::regex syscall(R"(^(?:([0-9]+)\s+)?([a-z_][a-z_0-9]*)\((.*)\)\s+=\s+(-?[0-9]+)(.*)$)");
    static const std::regex port(R"(sin6?_port=htons\(([0-9]+)\))");
    static const std::regex address(R"re((?:inet_addr\(|inet_pton\(AF_INET6, )"([^"]+)")re");
    static const std::regex descriptor(R"((?:TCP|UDP)(?:v6)?:\[((?:->|[^>])*)\]>)");
    static const std::regex endpoint(R"(^(?:\[([^\]]+)\]|([0-9.]+)):([0-9]+)$)");
    static const std::regex firstFd(R"(^([0-9]+))");

    NetworkTrace trace;
    std::unordered_map<std::string, std::string> protocols;
    std::istringstream input(output);
    for (std::string line; std::getline(input, line);) {
        std::smatch match;
        if (!std::regex_match(line, match, syscall)) {
            if (!line.empty() && line.find("+++ exited with ") == std::string::npos) {
                trace.diagnostics += line + '\n';
            }
            continue;
        }

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
