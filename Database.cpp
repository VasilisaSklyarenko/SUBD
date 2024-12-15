#include "Database.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <map>
#include <filesystem>


void Database::createTable(const std::string& tableName, const std::vector<std::string>& columns) {
    if (tables.find(tableName) != tables.end()) {
        std::cout << "Table " << tableName << " already exists.\n";
        return;
    }
    tables[tableName] = Table(tableName, columns);
    std::cout << "Table " << tableName << " created.\n";
}

void Database::deleteTable(const std::string& tableName) {
    tables.erase(tableName);
    std::cout << "Table " << tableName << " deleted.\n";
}

void Database::deleteFrom(const std::string& tableName, const std::string& condition) {
    if (tables.find(tableName) == tables.end()) {
        std::cout << "Table " << tableName << " does not exist.\n";
        return;
    }
    Table& table = tables[tableName];

    // �������� ������, ������� ������������� �������
    auto rowsToDelete = table.selectRows(condition);
    for (const auto& row : rowsToDelete) {
        int primaryKey = std::stoi(row[0]); // ��������� ���� � ������� �������
        table.deleteRow(primaryKey);
    }
    std::cout << "Rows deleted from " << tableName << ".\n";
}


void Database::insertInto(const std::string& tableName, const std::vector<std::string>& row) {
    if (tables.find(tableName) == tables.end()) {
        std::cout << "Table " << tableName << " does not exist.\n";
        return;
    }
    tables[tableName].insertRow(row);
    std::cout << "Row inserted into " << tableName << ".\n";
}

void Database::selectFrom(const std::string& tableName, const std::string& condition) {
    if (tables.find(tableName) == tables.end()) {
        std::cout << "Table " << tableName << " does not exist.\n";
        return;
    }

    Table& table = tables[tableName];
    for (const auto& col : table.getColumns()) {
        std::cout << col << "\t";
    }
    std::cout << "\n";
    auto rows = table.selectRows(condition);  

    // ������ ������
    for (const auto& row : rows) {
        for (const auto& value : row) {
            std::cout << value << "\t";
        }
        std::cout << "\n";
    }
}

void Database::loadAllTables() {
    // ������� ���� � ������ CSV
    std::string path = std::filesystem::current_path().string(); // ������������, ��� ����� ��������� � �������� �����
    std::filesystem::directory_iterator entry(path);
    for (; entry != std::filesystem::end(entry); entry++) {
        if (entry->path().extension() == ".csv") {
            Table table(entry->path().stem().string()); // ������� ������� � ������ ����� (��� ����������)
            table.loadFromCSV(entry->path().string());
            tables[table.getName()] = table; // ��������� ������� � ���� ������
        }
    }
}


void Database::saveAllTables() {
    for (const auto& pair : tables) {
        pair.second.saveToCSV(pair.first + ".csv");
    }
}





