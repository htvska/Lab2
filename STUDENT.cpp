
#include "STUDENT.h"

#include <iomanip>
#include <iostream>
#include <limits>
#include <stdexcept>

namespace {
    void validateGrade(int grade) {
        if (grade < 1 || grade > 5) {
            throw std::invalid_argument("Оценка должна быть целым числом от 1 до 5.");
        }
    }
}

STUDENT::STUDENT() : Person(), group_(0), grades_(nullptr), gradeCount_(0) {
    std::cout << "[Жизненный цикл] STUDENT " << this
        << ": конструктор без параметров.\n";
}

STUDENT::STUDENT(const std::string& surname, const std::string& initials,
    int group, const int* grades, std::size_t gradeCount)
    : Person(surname, initials), group_(0), grades_(nullptr), gradeCount_(0) {
    setGroup(group);
    setGrades(grades, gradeCount);
    std::cout << "[Жизненный цикл] STUDENT " << this
        << ": конструктор с параметрами (" << getSurname() << " "
        << getInitials() << ").\n";
}

STUDENT::STUDENT(const STUDENT& other)
    : Person(other), group_(other.group_), grades_(nullptr), gradeCount_(0) {
    if (other.gradeCount_ != 0) {
        // Глубокое копирование: объекты НЕ разделяют один массив оценок.
        grades_ = new int[other.gradeCount_];
        for (std::size_t i = 0; i < other.gradeCount_; ++i) {
            grades_[i] = other.grades_[i];
        }
        gradeCount_ = other.gradeCount_;
    }
    std::cout << "[Жизненный цикл] STUDENT " << this
        << ": конструктор копирования из " << &other << ".\n";
}

STUDENT& STUDENT::operator=(const STUDENT& other) {
    if (this == &other) {
        return *this;
    }

    // Подготавливаем новую память ДО изменения текущих оценок.
    int* newGrades = nullptr;
    if (other.gradeCount_ != 0) {
        newGrades = new int[other.gradeCount_];
        for (std::size_t i = 0; i < other.gradeCount_; ++i) {
            newGrades[i] = other.grades_[i];
        }
    }
    try {
        Person::operator=(other);
    }
    catch (...) {
        delete[] newGrades;
        throw;
    }
    delete[] grades_;
    grades_ = newGrades;
    gradeCount_ = other.gradeCount_;
    group_ = other.group_;
    return *this;
}

STUDENT::~STUDENT() {
    std::cout << "[Жизненный цикл] STUDENT " << this
        << ": деструктор (" << getSurname() << " " << getInitials()
        << ").\n";
    delete[] grades_;
}

int STUDENT::getGroup() const { return group_; }
std::size_t STUDENT::getGradeCount() const { return gradeCount_; }
const int* STUDENT::getGrades() const { return grades_; }

int STUDENT::getGrade(std::size_t index) const {
    if (index >= gradeCount_) {
        throw std::out_of_range("Оценки с таким порядковым номером нет.");
    }
    return grades_[index];
}

void STUDENT::setGroup(int group) {
    if (group <= 0) {
        throw std::invalid_argument("Номер группы должен быть положительным.");
    }
    group_ = group;
}

void STUDENT::setGrade(std::size_t index, int grade) {
    if (index >= gradeCount_) {
        throw std::out_of_range("Оценки с таким порядковым номером нет.");
    }
    validateGrade(grade);
    grades_[index] = grade;
}

void STUDENT::setGrades(const int* grades, std::size_t count) {
    if (grades == nullptr || count == 0) {
        throw std::invalid_argument("Нужно указать хотя бы одну оценку.");
    }
    for (std::size_t i = 0; i < count; ++i) {
        validateGrade(grades[i]);
    }
    int* newGrades = new int[count];
    for (std::size_t i = 0; i < count; ++i) {
        newGrades[i] = grades[i];
    }
    delete[] grades_;
    grades_ = newGrades;
    gradeCount_ = count;
}

double STUDENT::getAverage() const {
    if (gradeCount_ == 0) {
        throw std::logic_error("Нельзя вычислить средний балл без оценок.");
    }
    // Суммирование в double исключает переполнение int при большом числе оценок.
    double sum = 0.0;
    for (std::size_t i = 0; i < gradeCount_; ++i) {
        sum += grades_[i];
    }
    return sum / static_cast<double>(gradeCount_);
}

bool STUDENT::hasOnlyFoursAndFives() const {
    if (gradeCount_ == 0) {
        return false;
    }
    for (std::size_t i = 0; i < gradeCount_; ++i) {
        if (grades_[i] != 4 && grades_[i] != 5) {
            return false;
        }
    }
    return true;
}

void STUDENT::print(std::ostream& out) const {
    Person::print(out);
    out << ", группа " << group_ << ", оценки: ";
    for (std::size_t i = 0; i < gradeCount_; ++i) {
        if (i != 0) out << ' ';
        out << grades_[i];
    }
    if (gradeCount_ != 0) {
        // Восстанавливаем настройки потока после вывода среднего балла.
        const std::ios_base::fmtflags oldFlags = out.flags();
        const std::streamsize oldPrecision = out.precision();
        out << ", средний балл: " << std::fixed << std::setprecision(2)
            << getAverage();
        out.flags(oldFlags);
        out.precision(oldPrecision);
    }
}

std::ostream& operator<<(std::ostream& out, const STUDENT& student) {
    student.print(out);
    return out;
}

std::istream& operator>>(std::istream& in, STUDENT& student) {
    const bool interactive = &in == &std::cin;
    std::string surname;
    std::string initials;
    int group = 0;
    long long count = 0;

    if (interactive) std::cout << "Фамилия: ";
    if (!std::getline(in >> std::ws, surname)) return in;
    if (interactive) std::cout << "Инициалы (например, И.И.): ";
    if (!std::getline(in, initials)) return in;
    if (interactive) std::cout << "Номер группы: ";
    if (!(in >> group)) return in;
    if (interactive) std::cout << "Количество оценок: ";
    if (!(in >> count)) return in;

    if (count <= 0 ||
        static_cast<unsigned long long>(count) >
        static_cast<unsigned long long>(std::numeric_limits<std::size_t>::max())) {
        throw std::invalid_argument("Количество оценок должно быть положительным.");
    }
    const std::size_t gradeCount = static_cast<std::size_t>(count);
    int* inputGrades = new int[gradeCount];
    for (std::size_t i = 0; i < gradeCount; ++i) {
        if (interactive) std::cout << "Оценка №" << i + 1 << " (1–5): ";
        if (!(in >> inputGrades[i])) {
            delete[] inputGrades;
            return in;
        }
    }

    try {
        // Временный полноценный объект обеспечивает проверку и целостность данных.
        STUDENT entered(surname, initials, group, inputGrades, gradeCount);
        student = entered; // перегруженный оператор присваивания
    }
    catch (...) {
        delete[] inputGrades;
        throw;
    }
    delete[] inputGrades;
    return in;
}
