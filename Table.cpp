#include "Table.h"
#include <algorithm>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <iostream>

Table::Table() {
}

Table::Table(const std::string& name) : tableName(name) {
}

Table::Table(const std::string& name, const std::vector<std::string>& cols)
    : tableName(name), columns(cols), primaryKeyIndex(0) {
    // Создаем колонку с первичным ключом "<имя_таблицы>_pk"
    columns.insert(columns.begin(), name + "_pk");
}

void Table::insertRow(const std::vector<std::string>& row) {
    int primaryKey = data.size() + 1; // Уникальный первичный ключ
    std::vector<std::string> rowData = { std::to_string(primaryKey) };
    rowData.insert(rowData.end(), row.begin(), row.end());
    data.push_back(rowData);
}

void Table::deleteRow(int primaryKeyValue) {
    for (auto it = data.begin(); it != data.end(); ++it) {
        if (std::stoi((*it)[primaryKeyIndex]) == primaryKeyValue) {
            data.erase(it);
            break;
        }
    }
}





bool Table::evaluateCondition(const std::vector<std::string>& row, const std::string& condition) {
    std::istringstream ss(condition);
    std::string columnName;
    std::string op;
    std::string value;

    // Получаем имя колонки, оператор и значение
    if (!(ss >> columnName >> op >> value)) {
        std::cout << "Failed to parse condition: " << condition << std::endl;
        return false;
    }

    // Удаляем одинарные кавычки из значения
    if (value.front() == '\'' && value.back() == '\'') {
        value = value.substr(1, value.size() - 2);
    }

    // Найдем индекс колонки
    auto it = std::find(columns.begin(), columns.end(), columnName);
    if (it == columns.end()) {
        std::cout << "Column = " << columnName << " does not exist." << std::endl;
        return false;
    }

    int colIndex = std::distance(columns.begin(), it);

    // Сравнение значений
    if (op == "=") {
        return row[colIndex] == value;
    }
    else if (op == ">") {
        return row[colIndex] > value;
    }
    else if (op == "<") {
        return row[colIndex] < value;
    }
    else if (op == ">=") {
        return row[colIndex] >= value;
    }
    else if (op == "<=") {
        return row[colIndex] <= value;
    }
    else if (op == "!=") {
        return row[colIndex] != value;
    }
    else {
        std::cout << "Invalid operator: " << op << std::endl;
        return false;
    }
}


const std::vector<std::vector<std::string>>& Table::getRows() const {
    return rows; // Возвращаем строки данных
}



bool Table::evaluateConditions(const std::vector<std::string>& row, const std::string& condition) {
    // Разделение условий по логическим операторам
    std::string::size_type pos = 0;
    std::string conditionCopy = condition;
    std::vector<std::string> subConditions;

    while ((pos = conditionCopy.find("AND")) != std::string::npos || (pos = conditionCopy.find("OR")) != std::string::npos) {
        subConditions.push_back(conditionCopy.substr(0, pos));
        if (pos + 3 < conditionCopy.size()) { // 3 - длина "AND" или "OR"
            conditionCopy = conditionCopy.substr(pos + 3);
        }
        else {
            conditionCopy.clear();
            break;
        }
    }
    if (!conditionCopy.empty()) {
        subConditions.push_back(conditionCopy);
    }

    // Начинаем с первого условия
    bool result = evaluateCondition(row, subConditions[0]);

    for (size_t i = 1; i < subConditions.size(); ++i) {
        // Определение оператора
        std::string op = (subConditions[i - 1].find("AND") != std::string::npos) ? "AND" : "OR";
        bool nextResult = evaluateCondition(row, subConditions[i]);

        if (op == "AND") {
            result = result && nextResult;
        }
        else if (op == "OR") {
            result = result || nextResult;
        }
    }

    return result;
}







std::vector<std::vector<std::string>> Table::selectRows(const std::string& condition) {
    std::vector<std::vector<std::string>> result;

    // Если условие пустое, возвращаем все строки
    if (condition.empty()) {
        return data;
    }

    // Разделяем условия по операторам AND и OR
    std::vector<std::string> subConditions;
    std::vector<std::string> operators;
    std::istringstream ss(condition);
    std::string token;

    while (ss >> token) {
        if (token == "AND" || token == "OR") {
            operators.push_back(token);
        }
        else {
            subConditions.push_back(token);
        }
    }

    // Фильтрация строк на основе условий AND и OR
    for (const auto& row : data) {
        if (evaluateConditions(row, condition)) {
            for (const auto& value : row) {
                std::cout << value << " ";
            }
            std::cout << std::endl;
        }
    }

    return result;
}

std::vector<std::string> Table::getColumns() const {
    return columns;
}

void Table::displayTable() {
    for (const auto& col : columns) {
        std::cout << col << "\t";
    }
    std::cout << std::endl;

    for (const auto& row : data) {
        for (const auto& cell : row) {
            std::cout << cell << "\t";
        }
        std::cout << std::endl;
    }
}


void Table::loadFromCSV(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error opening file: " << filename << std::endl;
        return;
    }

    std::string line;
    // Считываем первую строку, которая содержит названия колонок
    if (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string column;
        while (std::getline(ss, column, ',')) {
            columns.push_back(column);
        }
    }

    // Считываем остальные строки, которые содержат данные
    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string value;
        std::vector<std::string> row;
        while (std::getline(ss, value, ',')) {
            row.push_back(value);
        }
        data.push_back(row);
    }

    file.close();
}

std::string Table::getName() const {
    return tableName; // Возвращаем имя таблицы
}

void Table::saveToCSV(const std::string& filename) const {
    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error opening file: " << filename << std::endl;
        return;
    }

    // Сохранение названий колонок
    for (size_t i = 0; i < columns.size(); ++i) {
        file << columns[i];
        if (i < columns.size() - 1) {
            file << ",";
        }
    }
    file << "\n";

    // Сохранение данных
    for (const auto& row : data) {
        for (size_t i = 0; i < row.size(); ++i) {
            file << row[i];
            if (i < row.size() - 1) {
                file << ",";
            }
        }
        file << "\n";
    }

    file.close();
}