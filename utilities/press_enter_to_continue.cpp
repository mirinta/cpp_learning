#include <termios.h>

#include <iostream>
#include <sstream>
#include <thread>

void pressEnterToContinue(const std::string& message)
{
    std::atomic<bool> enterPressed{false};
    auto print = [&]() {
        while (!enterPressed.load()) {
            std::cout << message << '\n';
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
        }
    };
    std::thread worker(print);
    std::string input;
    while (true) {
        tcflush(STDIN_FILENO, TCIFLUSH); // only for linux, TODO: support other platforms
        std::getline(std::cin, input);
        if (input.empty()) {
            enterPressed.exchange(true);
            break;
        }
    }
    worker.join();
}

int main()
{
    std::cout << "running..." << '\n';
    std::cout << "error occurred..." << '\n';
    pressEnterToContinue("Press enter to continue...");
    std::cout << "error handled!" << '\n';
    std::cout << "running..." << '\n';
}
