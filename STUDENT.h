
#ifndef STUDENT_H
#define STUDENT_H

#include "Person.h"

#include <cstddef>
#include <iosfwd>
#include <string>

// Класс-наследник: собственные поля студента — группа и оценки.
// grades_ хранит динамический массив, которым владеет объект STUDENT.
class STUDENT : public Person {
private:
    int group_;
    int* grades_;
    std::size_t gradeCount_;

public:
    STUDENT();
    STUDENT(const std::string& surname, const std::string& initials,
        int group, const int* grades, std::size_t gradeCount);
    STUDENT(const STUDENT& other);
    STUDENT& operator=(const STUDENT& other);
    ~STUDENT() override;

    int getGroup() const;
    std::size_t getGradeCount() const;
    int getGrade(std::size_t index) const;
    const int* getGrades() const;

    void setGroup(int group);
    void setGrade(std::size_t index, int grade);
    void setGrades(const int* grades, std::size_t count);

    double getAverage() const;
    bool hasOnlyFoursAndFives() const;
    void print(std::ostream& out) const override;

    // Перегрузка операций стандартных потоков, требуемая в варианте.
    friend std::istream& operator>>(std::istream& in, STUDENT& student);
    friend std::ostream& operator<<(std::ostream& out, const STUDENT& student);
};

#endif
