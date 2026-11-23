// Ex.No 8 - Move Semantics and Perfect Forwarding (User Input Version)
#include <iostream>
#include <cstring>
#include <chrono>
#include <string>
#include <utility>
#include <vector>
using namespace std;
using namespace chrono;

class String
{
private:
    char* data;
    size_t length;

public:
    // Constructor
    String(const char* str = "")
    {
        length = strlen(str);
        data = new char[length + 1];
        strcpy(data, str);
    }

    // Copy Constructor (deep copy)
    String(const String& other)
    {
        length = other.length;
        data = new char[length + 1];
        strcpy(data, other.data);
    }

    // Move Constructor (steals resource)
    String(String&& other) noexcept
    {
        data = other.data;
        length = other.length;
        other.data = nullptr;
        other.length = 0;
    }

    // Copy Assignment
    String& operator=(const String& other)
    {
        if (this != &other)
        {
            delete[] data;
            length = other.length;
            data = new char[length + 1];
            strcpy(data, other.data);
        }
        return *this;
    }

    // Move Assignment
    String& operator=(String&& other) noexcept
    {
        if (this != &other)
        {
            delete[] data;
            data = other.data;
            length = other.length;
            other.data = nullptr;
            other.length = 0;
        }
        return *this;
    }

    // Destructor
    ~String()
    {
        delete[] data;
    }

    size_t size() const { return length; }
    bool isEmpty() const { return data == nullptr; }

    void display() const
    {
        if (data != nullptr)
            cout << data << endl;
        else
            cout << "(empty - resource moved)" << endl;
    }
};

// Perfect Forwarding Function
template <typename T>
String createString(T&& value)
{
    return String(forward<T>(value));
}

int main()
{
    string input;
    long long iterations;

    cout << "Enter a string (any text, e.g. Perfect Forwarding Example): ";
    getline(cin, input);

    cout << "Enter number of iterations (e.g. 100000): ";
    cin >> iterations;

    if (iterations <= 0)
    {
        cout << "Iterations must be a positive number." << endl;
        return 1;
    }

    String original(input.c_str());

    // ---- Demonstrate copy vs move on the user's string ----
    cout << "\n--- Copy vs Move Demonstration ---" << endl;
    String copied = original;             // copy constructor
    cout << "After copy : original = ";
    original.display();
    cout << "             copied   = ";
    copied.display();

    String moved = move(copied);          // move constructor
    cout << "After move : copied   = ";
    copied.display();
    cout << "             moved    = ";
    moved.display();

    // ---- Perfect forwarding demonstration ----
    cout << "\n--- Perfect Forwarding ---" << endl;
    const char* lvaluePtr = input.c_str();
    String fromLvalue = createString(lvaluePtr);          // lvalue forwarded
    String fromRvalue = createString("Rvalue literal");   // rvalue forwarded
    cout << "Forwarded lvalue : ";
    fromLvalue.display();
    cout << "Forwarded rvalue : ";
    fromRvalue.display();

    // Pre-create the source objects (this setup is NOT timed)
    vector<String> sources;
    sources.reserve(iterations);
    for (long long i = 0; i < iterations; i++)
        sources.emplace_back(input.c_str());

    // ---- Benchmark copy: allocate new memory + copy characters ----
    auto startCopy = high_resolution_clock::now();
    for (long long i = 0; i < iterations; i++)
    {
        String c = sources[i];
    }
    auto endCopy = high_resolution_clock::now();
    auto copyTime = duration_cast<microseconds>(endCopy - startCopy);

    // ---- Benchmark move: only transfer the pointer ----
    auto startMove = high_resolution_clock::now();
    for (long long i = 0; i < iterations; i++)
    {
        String m = move(sources[i]);
    }
    auto endMove = high_resolution_clock::now();
    auto moveTime = duration_cast<microseconds>(endMove - startMove);

    cout << "\n--- Benchmark Results (" << iterations
         << " iterations, string length = " << original.size() << ") ---" << endl;
    cout << "Copy Operation Time: " << copyTime.count() << " microseconds" << endl;
    cout << "Move Operation Time: " << moveTime.count() << " microseconds" << endl;

    return 0;
}