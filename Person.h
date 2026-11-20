#ifndef PERSON_H
#define PERSON_H

#include <iosfwd>
#include <string>

// Базовый класс: общие сведения о человеке.
// STUDENT дополнит их номером группы и массивом оценок.
class Person {
private:
    std::string surname_;
    std::string initials_;

public:
    Person();
    Person(const std::string& surname, const std::string& initials);
    Person(const Person& other);
    Person& operator=(const Person& other);
    virtual ~Person();

    const std::string& getSurname() const;
    const std::string& getInitials() const;
    void setSurname(const std::string& surname);
    void setInitials(const std::string& initials);
    void setName(const std::string& surname, const std::string& initials);

    // Виртуальный метод позволит наследнику расширить вывод.
    virtual void print(std::ostream& out) const;
};

#endif

