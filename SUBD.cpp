#include "Database.h"
#include <iostream>
#include <sstream>

void processSQLCommand(Database& db, const std::string& command) {
    std::istringstream ss(command);
    std::string action;
    ss >> action;

    if (action == "CREATE") {
        std::string tableName;
        ss >> tableName;

        std::vector<std::string> columns;
        std::string column;
        while (ss >> column) {
            columns.push_back(column);
        }
        db.createTable(tableName, columns);

    }
    else if (action == "INSERT") {
        std::string into, tableName;
        ss >> into >> tableName; // Ожидается `INSERT INTO tableName`

        std::vector<std::string> row;
        std::string value;
        while (ss >> value) {
            row.push_back(value);
        }
        db.insertInto(tableName, row);

    }
    else if (action == "SELECT") {
        std::string star, from, tableName, where, condition;
        ss >> star >> from >> tableName; // Ожидается `SELECT * FROM tableName`

        // Проверка на наличие условия WHERE
        if (ss >> where && where == "WHERE") {
            std::getline(ss, condition);
            db.selectFrom(tableName, condition); // Передаем условие для фильтрации
        }
        else {
            db.selectFrom(tableName, ""); // Пустое условие - выводим все строки
        }

    }
    else if (action == "DELETE") {
        std::string from, tableName, where, condition;
        ss >> from >> tableName; // Ожидается `DELETE FROM tableName`

        if (ss >> where && where == "WHERE") {
            std::getline(ss, condition);
            db.deleteFrom(tableName, condition);
        }
        else {
            std::cout << "DELETE requires a WHERE clause.\n";
        }
    }
    else {
        std::cout << "Unknown command.\n";
    }
}

int main() {
    Database db;
    db.loadAllTables(); // Загружаем данные при старте

    // Главный цикл приложения
    bool running = true;
    while (running) {
        // Здесь код для работы с базой данных, ввод команд и т.д.
        std::cout << "> ";
        std::string command;
        std::getline(std::cin, command);

        if (command == "EXIT") break;
        processSQLCommand(db, command);

    }

    db.saveAllTables(); // Сохраняем данные перед выходом
    return 0;
}
