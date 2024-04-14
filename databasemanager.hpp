#pragma once

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QtSql>
#include <QDebug>
class DatabaseManager
{
public:
    DatabaseManager();
    QSqlDatabase GetDatabase();
    void OpenConnection();
    void CloseConnection();
private:
    QSqlDatabase m_db;
};
