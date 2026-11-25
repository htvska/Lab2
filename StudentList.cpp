
#include "StudentList.h"

#include <iostream>
#include <limits>
#include <stdexcept>
#include <utility>

StudentList::StudentList() : data_(nullptr), size_(0), capacity_(0) {
    std::cout << "[Жизненный цикл] StudentList " << this
        << ": конструктор без параметров.\n";
}

StudentList::StudentList(std::size_t initialCapacity)
    : data_(nullptr), size_(0), capacity_(0) {
    ensureCapacity(initialCapacity);
    std::cout << "[Жизненный цикл] StudentList " << this
        << ": конструктор с параметром, вместимость "
        << initialCapacity << ".\n";
}

StudentList::StudentList(const StudentList& other)
    : data_(nullptr), size_(0), capacity_(0) {
    try {
        ensureCapacity(other.capacity_);
        for (std::size_t i = 0; i < other.size_; ++i) {
            data_[i] = new STUDENT(*other.data_[i]); // глубокие копии
            ++size_;
        }
    }
    catch (...) {
        clear();
        delete[] data_;
        throw;
    }
    std::cout << "[Жизненный цикл] StudentList " << this
        << ": конструктор копирования из " << &other << ".\n";
}

StudentList& StudentList::operator=(const StudentList& other) {
    if (this != &other) {
        // Идиома copy-and-swap: при ошибке выделения памяти исходный список цел.
        StudentList copy(other);
        std::swap(data_, copy.data_);
        std::swap(size_, copy.size_);
        std::swap(capacity_, copy.capacity_);
    }
    return *this;
}

StudentList::~StudentList() {
    std::cout << "[Жизненный цикл] StudentList " << this
        << ": деструктор, студентов: " << size_ << ".\n";
    clear();
    delete[] data_;
}

std::size_t StudentList::getSize() const { return size_; }
std::size_t StudentList::getCapacity() const { return capacity_; }

const STUDENT& StudentList::getAt(std::size_t index) const {
    if (index >= size_) {
        throw std::out_of_range("Студента с таким порядковым номером нет.");
    }
    return *data_[index];
}

STUDENT& StudentList::getAt(std::size_t index) {
    if (index >= size_) {
        throw std::out_of_range("Студента с таким порядковым номером нет.");
    }
    return *data_[index];
}

void StudentList::ensureCapacity(std::size_t needed) {
    if (needed <= capacity_) return;
    const std::size_t limit = std::numeric_limits<std::size_t>::max();
    std::size_t newCapacity = capacity_ == 0 ? 1 : capacity_;
    while (newCapacity < needed) {
        if (newCapacity > limit / 2) {
            newCapacity = needed;
            break;
        }
        newCapacity *= 2;
    }
    STUDENT** newData = new STUDENT * [newCapacity]();
    for (std::size_t i = 0; i < size_; ++i) {
        newData[i] = data_[i]; // переносим указатели, объекты не копируем
    }
    delete[] data_;
    data_ = newData;
    capacity_ = newCapacity;
}

void StudentList::insertAt(std::size_t index, const STUDENT& student) {
    if (index > size_) {
        throw std::out_of_range("Позиция вставки вне списка (допустимо от 1 до N+1).");
    }
    if (size_ == std::numeric_limits<std::size_t>::max()) {
        throw std::length_error("Размер списка больше максимально допустимого.");
    }
    // Не разрешаем помещать в список объект без оценок.
    (void)student.getAverage();
    ensureCapacity(size_ + 1);
    STUDENT* copy = new STUDENT(student);
    for (std::size_t i = size_; i > index; --i) {
        data_[i] = data_[i - 1];
    }
    data_[index] = copy;
    ++size_;
    // Позиция вставки доступна любая; окончательный порядок — по среднему баллу.
    sortByAverage();
}

void StudentList::setAt(std::size_t index, const STUDENT& student) {
    (void)student.getAverage(); // нельзя заменить запись незаполненным студентом
    getAt(index) = student;
    sortByAverage();
}

void StudentList::eraseAt(std::size_t index) {
    if (index >= size_) {
        throw std::out_of_range("Позиция удаления вне списка.");
    }
    delete data_[index];
    for (std::size_t i = index; i + 1 < size_; ++i) {
        data_[i] = data_[i + 1];
    }
    --size_;
    data_[size_] = nullptr;
}

void StudentList::sortByAverage() {
    // Сортировка вставками. Меняем только указатели, не копируем массивы оценок.
    // Равные средние баллы сохраняют относительный порядок (стабильность).
    for (std::size_t i = 1; i < size_; ++i) {
        STUDENT* current = data_[i];
        std::size_t j = i;
        while (j > 0 && data_[j - 1]->getAverage() > current->getAverage()) {
            data_[j] = data_[j - 1];
            --j;
        }
        data_[j] = current;
    }
}

void StudentList::printAll(std::ostream& out) const {
    if (size_ == 0) {
        out << "Список студентов пуст.\n";
        return;
    }
    out << "Студенты по возрастанию среднего балла:\n";
    for (std::size_t i = 0; i < size_; ++i) {
        out << i + 1 << ". " << *data_[i] << '\n';
    }
}

void StudentList::printExcellent(std::ostream& out) const {
    bool found = false;
    for (std::size_t i = 0; i < size_; ++i) {
        if (data_[i]->hasOnlyFoursAndFives()) {
            if (!found) out << "Студенты, у которых все оценки 4 или 5:\n";
            out << data_[i]->getSurname() << " " << data_[i]->getInitials()
                << ", группа " << data_[i]->getGroup() << '\n';
            found = true;
        }
    }
    if (!found) {
        out << "Студентов, имеющих только оценки 4 и 5, нет.\n";
    }
}

void StudentList::clear() {
    for (std::size_t i = 0; i < size_; ++i) {
        delete data_[i];
        data_[i] = nullptr;
    }
    size_ = 0;
}
