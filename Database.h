#include "Table.h"
#include <unordered_map>
#include <string>

class Database {
private:
    std::unordered_map<std::string, Table> tables;

public:
    void createTable(const std::string& tableName, const std::vector<std::string>& columns);
    void deleteTable(const std::string& tableName);
    void insertInto(const std::string& tableName, const std::vector<std::string>& row);
    void selectFrom(const std::string& tableName, const std::string& condition);
    void deleteFrom(const std::string& tableName, const std::string& condition);

    void saveAllTables();
    void loadAllTables();
};
