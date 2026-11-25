
#ifndef STUDENT_LIST_H
#define STUDENT_LIST_H

#include "STUDENT.h"

#include <cstddef>
#include <iosfwd>

// Динамический список указателей на объекты STUDENT 
// Список владеет каждым STUDENT* и освобождает память в деструкторе.
class StudentList {
private:
    STUDENT** data_;
    std::size_t size_;
    std::size_t capacity_;

    void ensureCapacity(std::size_t needed);

public:
    StudentList();
    explicit StudentList(std::size_t initialCapacity);
    StudentList(const StudentList& other);
    StudentList& operator=(const StudentList& other);
    ~StudentList();

    std::size_t getSize() const;
    std::size_t getCapacity() const;
    const STUDENT& getAt(std::size_t index) const;
    STUDENT& getAt(std::size_t index);
    void setAt(std::size_t index, const STUDENT& student);

    void insertAt(std::size_t index, const STUDENT& student);
    void eraseAt(std::size_t index);
    void sortByAverage();
    void printAll(std::ostream& out) const;
    void printExcellent(std::ostream& out) const;
    void clear();
};

#endif
