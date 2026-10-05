#include <iostream>
#include <string>
#include <utility>

template <typename T>
T max_like(const T& a, const T& b) {
    return (a < b) ? b : a;
}

template <typename T>
class Box {
public:
    explicit Box(T value) : value_(std::move(value)) {}

    const T& get() const { return value_; }
    void set(T value) { value_ = std::move(value); }

private:
    T value_;
};

static void increment(int& value) {
    ++value;
}

int main() {
    int n = 41;
    increment(n);

    Box<int> number_box{7};
    Box<std::string> text_box{"data"};

    std::cout << "n=" << n << "\n";
    std::cout << "max=" << max_like(10, 20) << "\n";
    std::cout << number_box.get() << " " << text_box.get() << "\n";

    return (n == 42 && max_like(10, 20) == 20) ? 0 : 1;
}
