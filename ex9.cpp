// Ex.No 9 - Singleton, Factory and Observer Patterns (User Input Version)
#include <iostream>
#include <vector>
#include <memory>
#include <string>

using namespace std;

/** @brief Observer interface. */
class Observer
{
public:
    virtual void notify(const string& message) = 0;
    virtual ~Observer() {}
};

/** @brief Email notification observer. */
class EmailNotification : public Observer
{
private:
    string email;

public:
    EmailNotification(string e) { email = e; }

    void notify(const string& message) override
    {
        cout << "Email sent to " << email << ": " << message << endl;
    }
};

/** @brief SMS notification observer. */
class SMSNotification : public Observer
{
private:
    string phoneNumber;

public:
    SMSNotification(string number) { phoneNumber = number; }

    void notify(const string& message) override
    {
        cout << "SMS sent to " << phoneNumber << ": " << message << endl;
    }
};

/** @brief Factory class for creating notifications. */
class NotificationFactory
{
public:
    static shared_ptr<Observer> createNotification(const string& type,
                                                   const string& receiver)
    {
        if (type == "EMAIL")
            return make_shared<EmailNotification>(receiver);
        else if (type == "SMS")
            return make_shared<SMSNotification>(receiver);

        return nullptr;
    }
};

/** @brief Singleton Event Manager. */
class EventManager
{
private:
    vector<shared_ptr<Observer>> observers;

    EventManager() {}   // private constructor

public:
    EventManager(const EventManager&) = delete;
    EventManager& operator=(const EventManager&) = delete;

    static EventManager& getInstance()
    {
        static EventManager instance;
        return instance;
    }

    void subscribe(shared_ptr<Observer> observer)
    {
        observers.push_back(observer);
    }

    size_t subscriberCount() const { return observers.size(); }

    void triggerEvent(const string& message)
    {
        cout << "\nEvent Triggered: " << message << endl;
        cout << "Sending Notifications...\n";

        for (auto observer : observers)
            observer->notify(message);
    }
};

int main()
{
    EventManager& manager = EventManager::getInstance();
    int choice;

    do
    {
        cout << "\n===== Event-Driven Notification System =====" << endl;
        cout << "1. Add Email subscriber" << endl;
        cout << "2. Add SMS subscriber" << endl;
        cout << "3. Trigger an event" << endl;
        cout << "4. Exit" << endl;
        cout << "Enter your choice: ";

        if (!(cin >> choice))
            break;
        cin.ignore(1000, '\n');

        if (choice == 1 || choice == 2)
        {
            string type = (choice == 1) ? "EMAIL" : "SMS";
            string receiver;

            cout << (choice == 1 ? "Enter email address (e.g. student@gmail.com): "
                                 : "Enter phone number (e.g. 9876543210): ");
            getline(cin, receiver);

            auto observer = NotificationFactory::createNotification(type, receiver);
            if (observer)
            {
                manager.subscribe(observer);
                cout << type << " subscriber added. Total subscribers: "
                     << manager.subscriberCount() << endl;
            }
        }
        else if (choice == 3)
        {
            if (manager.subscriberCount() == 0)
            {
                cout << "No subscribers yet! Add at least one subscriber first." << endl;
                continue;
            }
            string message;
            cout << "Enter event message (e.g. Exam starts tomorrow!): ";
            getline(cin, message);
            manager.triggerEvent(message);
        }
        else if (choice != 4)
        {
            cout << "Invalid choice. Try again." << endl;
        }

    } while (choice != 4);

    cout << "\nExiting program. Goodbye!" << endl;
    return 0;
}