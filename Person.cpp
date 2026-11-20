
#include "Person.h"

#include <iostream>
#include <stdexcept>

Person::Person() : surname_(), initials_() {
    std::cout << "[Жизненный цикл] Person " << this
        << ": конструктор без параметров.\n";
}

Person::Person(const std::string& surname, const std::string& initials)
    : surname_(), initials_() {
    setName(surname, initials);
    std::cout << "[Жизненный цикл] Person " << this
        << ": конструктор с параметрами (" << surname_ << " "
        << initials_ << ").\n";
}

Person::Person(const Person& other)
    : surname_(other.surname_), initials_(other.initials_) {
    std::cout << "[Жизненный цикл] Person " << this
        << ": конструктор копирования из " << &other << ".\n";
}

Person& Person::operator=(const Person& other) {
    if (this != &other) {
        // Сначала копируем данные: если не хватит памяти, объект не меняется.
        std::string newSurname = other.surname_;
        std::string newInitials = other.initials_;
        surname_.swap(newSurname);
        initials_.swap(newInitials);
    }
    return *this;
}

Person::~Person() {
    std::cout << "[Жизненный цикл] Person " << this
        << ": деструктор (" << surname_ << " " << initials_ << ").\n";
}

const std::string& Person::getSurname() const { return surname_; }
const std::string& Person::getInitials() const { return initials_; }

void Person::setSurname(const std::string& surname) {
    if (surname.find_first_not_of(" \t\r\n") == std::string::npos) {
        throw std::invalid_argument("Фамилия не может быть пустой.");
    }
    surname_ = surname;
}

void Person::setInitials(const std::string& initials) {
    if (initials.find_first_not_of(" \t\r\n") == std::string::npos) {
        throw std::invalid_argument("Инициалы не могут быть пустыми.");
    }
    initials_ = initials;
}

void Person::setName(const std::string& surname, const std::string& initials) {
    // Проверяем оба поля до начала изменения объекта.
    if (surname.find_first_not_of(" \t\r\n") == std::string::npos ||
        initials.find_first_not_of(" \t\r\n") == std::string::npos) {
        throw std::invalid_argument("Фамилия и инициалы должны быть заполнены.");
    }
    std::string newSurname = surname;
    std::string newInitials = initials;
    surname_.swap(newSurname);
    initials_.swap(newInitials);
}

void Person::print(std::ostream& out) const {
    out << surname_ << " " << initials_;
}
