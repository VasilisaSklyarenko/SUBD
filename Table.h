#include <string>
#include <vector>
#include <unordered_map>
#include <iostream>
#include <fstream>
#include <sstream>

class Table {
private:
    std::string tableName;
    std::vector<std::string> columns;
    std::vector<std::vector<std::string>> rows;
    std::vector<std::vector<std::string>> data;
    std::vector<std::string> columnNames;

    int primaryKeyIndex;


public:
    Table();
    Table(const std::string& name);
    Table(const std::string& name, const std::vector<std::string>& cols);
    void insertRow(const std::vector<std::string>& row);
    void deleteRow(int primaryKeyValue);

    std::string getName() const;

    bool evaluateCondition(const std::vector<std::string>& row, const std::string& condition);
    bool evaluateConditions(const std::vector<std::string>& row, const std::string& condition);

    void loadFromCSV(const std::string& filename);
    void saveToCSV(const std::string& filename) const;

    const std::vector<std::vector<std::string>>& getRows() const;
    std::vector<std::string> getColumns() const;
    std::vector<std::vector<std::string>> selectRows(const std::string& condition);
    void displayTable();
};
