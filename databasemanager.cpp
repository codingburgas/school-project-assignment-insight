#include "databasemanager.hpp"

DatabaseManager::DatabaseManager() {

}

QSqlDatabase DatabaseManager::GetDatabase() {
    return m_db;
}

void DatabaseManager::OpenConnection() {
    // Configure the database connection parameters
    m_db = QSqlDatabase::addDatabase("QMYSQL");
    m_db.setHostName("project-insight.mysql.database.azure.com");
    m_db.setPort(3306);  // Default MySQL port
    m_db.setDatabaseName("qt");
    m_db.setUserName("insight");
    m_db.setPassword("Project-admin");

    // Open the database connection and handle success or failure
    if (m_db.open() && m_db.isOpen()) {
        qDebug() << "Database opened successfully";
    } else {
        // Handle connection error
        qDebug() << "Database failed to open";
        qDebug() << m_db.lastError().text();
    }
}

void DatabaseManager::CloseConnection() {
    // Close the database connection
    m_db.close();
}
