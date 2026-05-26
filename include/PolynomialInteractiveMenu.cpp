#include "PolynomialInteractiveMenu.h"
#include <iostream>
#include <iomanip>
#include <limits>

PolynomialInteractiveMenu::PolynomialInteractiveMenu() 
    : currentStorageType(LIST_STORAGE) {
}

void PolynomialInteractiveMenu::displayHeader() {
    std::cout << "\n";
    std::cout << "╔═══════════════════════════════════════════════════════════╗\n";
    std::cout << "║     СИСТЕМА ВЫБОРА СПОСОБА ХРАНЕНИЯ МОНОМОВ ПОЛИНОМА     ║\n";
    std::cout << "╚═══════════════════════════════════════════════════════════╝\n";
    std::cout << "\n";
}

std::string PolynomialInteractiveMenu::getStorageTypeName(StorageType type) {
    switch (type) {
        case LIST_STORAGE:
            return "Связный список (List)";
        case AVL_TREE_STORAGE:
            return "АВЛ-дерево (AVL Tree)";
        case RED_BLACK_TREE_STORAGE:
            return "Красно-чёрное дерево (Red-Black Tree)";
        case HASH_OPEN_ADDRESS_STORAGE:
            return "Хеш-таблица (Open Addressing)";
        case HASH_FORWARD_STORAGE:
            return "Хеш-таблица (Forward Chaining)";
        case UNORDERED_TAB_STORAGE:
            return "Неупорядоченная таблица (Unordered)";
        default:
            return "Неизвестный тип";
    }
}

void PolynomialInteractiveMenu::displayStorageMenu() {
    std::cout << "\n┌─────────────────────────────────────────────────────────┐\n";
    std::cout << "│ Выберите тип структуры данных для хранения мономов:    │\n";
    std::cout << "└─────────────────────────────────────────────────────────┘\n\n";

    std::cout << "  1. Связный список (List)\n";
    std::cout << "     • Вставка: O(n), Поиск: O(n), Удаление: O(n)\n";
    std::cout << "     • Простая реализация, естественное сохранение порядка\n\n";

    std::cout << "  2. АВЛ-дерево (AVL Tree)\n";
    std::cout << "     • Вставка: O(log n), Поиск: O(log n), Удаление: O(log n)\n";
    std::cout << "     • Быстрый поиск, автоматическая балансировка\n\n";

    std::cout << "  3. Красно-чёрное дерево (Red-Black Tree)\n";
    std::cout << "     • Вставка: O(log n), Поиск: O(log n), Удаление: O(log n)\n";
    std::cout << "     • Быстрые операции, меньше ротаций, чем АВЛ\n\n";

    std::cout << "  4. Хеш-таблица (Open Addressing)\n";
    std::cout << "     • Вставка: O(1) средн., Поиск: O(1) средн., Удаление: O(1) средн.\n";
    std::cout << "     • Очень быстрый доступ, компактное хранение\n\n";

    std::cout << "  5. Хеш-таблица (Forward Chaining)\n";
    std::cout << "     • Вставка: O(1) средн., Поиск: O(1) средн., Удаление: O(1) средн.\n";
    std::cout << "     • Простое разрешение коллизий, легкое удаление\n\n";

    std::cout << "  6. Неупорядоченная таблица (Unordered)\n";
    std::cout << "     • Вставка: O(1) в конец, Поиск: O(n), Удаление: O(n)\n";
    std::cout << "     • Очень быстрая вставка, простая структура\n\n";

    std::cout << "  0. Выход\n\n";
}

void PolynomialInteractiveMenu::displayMainMenu() {
    std::cout << "\n┌─────────────────────────────────────────────────────────┐\n";
    std::cout << "│ Текущее хранилище: " << std::setw(40) << std::left 
              << getStorageTypeName(currentStorageType) << "│\n";
    std::cout << "└─────────────────────────────────────────────────────────┘\n\n";

    std::cout << "  1. Добавить моном\n";
    std::cout << "  2. Показать полином\n";
    std::cout << "  3. Информация о текущем хранилище\n";
    std::cout << "  4. Изменить тип хранилища\n";
    std::cout << "  5. Очистить полином\n";
    std::cout << "  0. Выход\n\n";
}

void PolynomialInteractiveMenu::addMonom() {
    std::cout << "\n┌─ Добавление нового монома ─────────────────────────────┐\n";
    
    double coeff;
    int powerX, powerY, powerZ;

    std::cout << "│ Введите коэффициент: ";
    while (!(std::cin >> coeff)) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "│ Ошибка! Введите число: ";
    }

    std::cout << "│ Введите степень X: ";
    while (!(std::cin >> powerX) || powerX < 0) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "│ Ошибка! Введите неотрицательное число: ";
    }

    std::cout << "│ Введите степень Y: ";
    while (!(std::cin >> powerY) || powerY < 0) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "│ Ошибка! Введите неотрицательное число: ";
    }

    std::cout << "│ Введите степень Z: ";
    while (!(std::cin >> powerZ) || powerZ < 0) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "│ Ошибка! Введите неотрицательное число: ";
    }

    currentPolynomial.add_monom(coeff, powerX, powerY, powerZ);
    
    std::cout << "└────────────────────────────────────────────────────────┘\n";
    std::cout << "✓ Моном успешно добавлен!\n";
}

void PolynomialInteractiveMenu::displayPolynomial() {
    std::cout << "\n┌────────────────────────────────────────────────────────┐\n";
    std::cout << "│ Текущий полином:                                       │\n";
    std::cout << "│                                                        │\n";
    std::cout << "│ " << std::setw(52) << std::left << currentPolynomial.to_string() << "│\n";
    std::cout << "│                                                        │\n";
    std::cout << "└────────────────────────────────────────────────────────┘\n";
}

void PolynomialInteractiveMenu::clearPolynomial() {
    std::cout << "\nОчистить полином? (y/n): ";
    char choice;
    std::cin >> choice;
    if (choice == 'y' || choice == 'Y') {
        currentPolynomial = Polynomial();
        std::cout << "✓ Полином очищен!\n";
    }
}

std::string PolynomialInteractiveMenu::getStorageTypeDescription(StorageType type) {
    switch (type) {
        case LIST_STORAGE:
            return "СВЯЗНЫЙ СПИСОК (List)\n\n"
                   "Описание: Линейная структура данных, где каждый элемент\n"
                   "содержит ссылку на следующий элемент.\n\n"
                   "Характеристики:\n"
                   "  • Вставка: O(n) - требует поиска позиции\n"
                   "  • Удаление: O(n)\n"
                   "  • Поиск: O(n)\n"
                   "  • Память: O(n)\n\n"
                   "Преимущества:\n"
                   "  ✓ Простая реализация\n"
                   "  ✓ Гибкая структура\n"
                   "  ✓ Естественное сохранение порядка\n"
                   "  ✓ Динамическое использование памяти\n\n"
                   "Недостатки:\n"
                   "  ✗ Медленный поиск\n"
                   "  ✗ Нет прямого доступа к элементам\n"
                   "  ✗ Дополнительная память на указатели";

        case AVL_TREE_STORAGE:
            return "АВЛ-ДЕРЕВО (AVL Tree)\n\n"
                   "Описание: Самобалансирующееся дерево поиска, где глубины\n"
                   "левого и правого поддеревьев отличаются не более чем на 1.\n\n"
                   "Характеристики:\n"
                   "  • Вставка: O(log n)\n"
                   "  • Удаление: O(log n)\n"
                   "  • Поиск: O(log n)\n"
                   "  • Память: O(n)\n\n"
                   "Преимущества:\n"
                   "  ✓ Быстрый поиск\n"
                   "  ✓ Автоматическая балансировка\n"
                   "  ✓ Гарантированная высота log n\n"
                   "  ✓ Отсортированный порядок обхода\n\n"
                   "Недостатки:\n"
                   "  ✗ Сложная реализация\n"
                   "  ✗ Дополнительная память на баланс-факторы\n"
                   "  ✗ Требует большего числа ротаций";

        case RED_BLACK_TREE_STORAGE:
            return "КРАСНО-ЧЁРНОЕ ДЕРЕВО (Red-Black Tree)\n\n"
                   "Описание: Самобалансирующееся дерево поиска с дополнительным\n"
                   "цветовым атрибутом (красный или чёрный) для каждого узла.\n\n"
                   "Характеристики:\n"
                   "  • Вставка: O(log n)\n"
                   "  • Удаление: O(log n)\n"
                   "  • Поиск: O(log n)\n"
                   "  • Память: O(n)\n\n"
                   "Преимущества:\n"
                   "  ✓ Быстрые операции\n"
                   "  ✓ Меньше ротаций, чем АВЛ\n"
                   "  ✓ Хороший баланс между простотой и эффективностью\n"
                   "  ✓ Широко используется на практике\n\n"
                   "Недостатки:\n"
                   "  ✗ Более сложная реализация, чем АВЛ\n"
                   "  ✗ Требует дополнительный бит для цвета";

        case HASH_OPEN_ADDRESS_STORAGE:
            return "ХЕШ-ТАБЛИЦА (Open Addressing)\n\n"
                   "Описание: Хеш-таблица, где коллизии разрешаются путём поиска\n"
                   "другой позиции в массиве согласно зондирующей последовательности.\n\n"
                   "Характеристики:\n"
                   "  • Вставка: O(1) средн., O(n) худш.\n"
                   "  • Удаление: O(1) средн., O(n) худш.\n"
                   "  • Поиск: O(1) средн., O(n) худш.\n"
                   "  • Память: O(n)\n\n"
                   "Преимущества:\n"
                   "  ✓ Очень быстрый доступ в среднем случае\n"
                   "  ✓ Компактное хранение\n"
                   "  ✓ Нет дополнительных указателей\n"
                   "  ✓ Хорошая кеш-локальность\n\n"
                   "Недостатки:\n"
                   "  ✗ Коллизии хеширования\n"
                   "  ✗ Требует хорошей хеш-функции\n"
                   "  ✗ Проблема удаления элементов\n"
                   "  ✗ Деградация при высокой заполненности";

        case HASH_FORWARD_STORAGE:
            return "ХЕШ-ТАБЛИЦА (Forward Chaining)\n\n"
                   "Описание: Хеш-таблица, где коллизии разрешаются путём\n"
                   "создания цепочек (списков) элементов в каждой ячейке.\n\n"
                   "Характеристики:\n"
                   "  • Вставка: O(1) средн., O(n) худш.\n"
                   "  • Удаление: O(1) средн., O(n) худш.\n"
                   "  • Поиск: O(1) средн., O(n) худш.\n"
                   "  • Память: O(n)\n\n"
                   "Преимущества:\n"
                   "  ✓ Простое разрешение коллизий\n"
                   "  ✓ Удаление легче, чем в open addressing\n"
                   "  ✓ Эффективна при малом факторе загрузки\n"
                   "  ✓ Гибкая структура\n\n"
                   "Недостатки:\n"
                   "  ✗ Требует дополнительная память для указателей\n"
                   "  ✗ Хуже кеш-локальность\n"
                   "  ✗ Может быть медленнее на практике";

        case UNORDERED_TAB_STORAGE:
            return "НЕУПОРЯДОЧЕННАЯ ТАБЛИЦА (Unordered)\n\n"
                   "Описание: Простой динамический массив без сортировки,\n"
                   "новые элементы добавляются в конец.\n\n"
                   "Характеристики:\n"
                   "  • Вставка: O(1) в конец, O(n) в произвольное место\n"
                   "  • Удаление: O(n)\n"
                   "  • Поиск: O(n)\n"
                   "  • Память: O(n)\n\n"
                   "Преимущества:\n"
                   "  ✓ Очень быстрая вставка в конец\n"
                   "  ✓ Простая структура\n"
                   "  ✓ Минимальные накладные расходы\n"
                   "  ✓ Хорошая кеш-локальность\n\n"
                   "Недостатки:\n"
                   "  ✗ Медленный поиск\n"
                   "  ✗ Медленное удаление\n"
                   "  ✗ Требует перестроения при добавлении\n"
                   "  ✗ Нет сортировки";

        default:
            return "Неизвестный тип хранилища";
    }
}

void PolynomialInteractiveMenu::getStorageInfo() {
    std::cout << "\n┌────────────────────────────────────────────────────────┐\n";
    std::cout << "│ ИНФОРМАЦИЯ О ТЕКУЩЕМ ХРАНИЛИЩЕ                         │\n";
    std::cout << "└────────────────────────────────────────────────────────┘\n\n";
    
    std::string info = getStorageTypeDescription(currentStorageType);
    // Выводим информацию с форматированием
    for (char c : info) {
        std::cout << c;
    }
    std::cout << "\n\n";
}

void PolynomialInteractiveMenu::selectStorage() {
    int choice;
    while (true) {
        displayHeader();
        displayStorageMenu();
        std::cout << "Ваш выбор: ";
        
        if (!(std::cin >> choice)) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Ошибка ввода! Попробуйте снова.\n";
            continue;
        }

        if (choice == 0) {
            return;
        } else if (choice >= 1 && choice <= 6) {
            currentStorageType = static_cast<StorageType>(choice - 1);
            std::cout << "\n✓ Хранилище '" << getStorageTypeName(currentStorageType) 
                      << "' успешно применено!\n";
            std::cout << "\nНажмите Enter для продолжения...";
            std::cin.ignore();
            std::cin.ignore();
            break;
        } else {
            std::cout << "✗ Некорректный выбор! Попробуйте снова.\n";
        }
    }
}

void PolynomialInteractiveMenu::run() {
    while (true) {
        displayHeader();
        displayMainMenu();
        
        int choice;
        std::cout << "Ваш выбор: ";
        
        if (!(std::cin >> choice)) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "✗ Ошибка ввода! Попробуйте снова.\n";
            continue;
        }

        switch (choice) {
            case 1:
                addMonom();
                std::cout << "\nНажмите Enter для продолжения...";
                std::cin.ignore();
                std::cin.ignore();
                break;
            case 2:
                displayPolynomial();
                std::cout << "\nНажмите Enter для продолжения...";
                std::cin.ignore();
                std::cin.ignore();
                break;
            case 3:
                getStorageInfo();
                std::cout << "Нажмите Enter для продолжения...";
                std::cin.ignore();
                std::cin.ignore();
                break;
            case 4:
                selectStorage();
                currentPolynomial = Polynomial();
                break;
            case 5:
                clearPolynomial();
                std::cout << "\nНажмите Enter для продолжения...";
                std::cin.ignore();
                std::cin.ignore();
                break;
            case 0:
                std::cout << "\nСпасибо за использование программы! До свидания!\n\n";
                return;
            default:
                std::cout << "✗ Некорректный выбор! Попробуйте снова.\n";
                std::cout << "\nНажмите Enter для продолжения...";
                std::cin.ignore();
                std::cin.ignore();
        }
    }
}
