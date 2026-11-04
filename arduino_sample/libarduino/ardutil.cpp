#include "ardutil.h"

#include <cctype>
#include <chrono>
#include <thread>

extern "C" unsigned long millis(void)
{
    using Clock = std::chrono::steady_clock;
    const auto now = Clock::now();
    static const auto start = now;

    if (now <= start) {
        return 0;
    }

    return static_cast<unsigned long>(
        std::chrono::duration_cast<std::chrono::milliseconds>(now - start).count());
}

extern "C" void delay(unsigned long ms)
{
    std::this_thread::sleep_for(std::chrono::milliseconds(ms));
}

extern "C" bool isHexadecimalDigit(int character)
{
    return std::isxdigit(static_cast<unsigned char>(character)) != 0;
}

extern "C" bool isSpace(int character)
{
    return std::isspace(static_cast<unsigned char>(character)) != 0;
}

extern "C" char *itoa(int value, char *buffer, int base)
{
    if (base < 2 || base > 36) {
        buffer[0] = '\0';
        return buffer;
    }

    static constexpr char digits[] = "0123456789abcdefghijklmnopqrstuvwxyz";
    const bool negative = value < 0 && base == 10;
    unsigned int remaining = static_cast<unsigned int>(value);
    if (negative) {
        remaining = 0U - remaining;
    }

    const unsigned int radix = static_cast<unsigned int>(base);
    char *cursor = buffer;
    do {
        *cursor++ = digits[remaining % radix];
        remaining /= radix;
    } while (remaining != 0);

    if (negative) {
        *cursor++ = '-';
    }
    std::reverse(buffer, cursor);
    *cursor = '\0';
    return buffer;
}