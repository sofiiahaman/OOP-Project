#ifndef AUTHSERVICE_H
#define AUTHSERVICE_H

#include <QString>
#include "database.h"
#include "repositories/UserRepository.h"
#include "repositories/StudentRepository.h"

class AuthService {
public:
    explicit AuthService(Database& db);

    QString login(const QString& username, const QString& password);

    QString registerExistingStudent(const QString& email, const QString& username, const QString& password);

private:
    Database& m_db;
    UserRepository m_userRepo;
    StudentRepository m_studentRepo;
};

#endif //AUTHSERVICE_H
