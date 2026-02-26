#include "AuthService.h"
#include <QSqlDatabase>

AuthService::AuthService(Database& db)
    : m_db(db), m_userRepo(db), m_studentRepo(db) {}

QString AuthService::login(const QString& username, const QString& password) {
    if (m_userRepo.checkLogin(username, password)) {
        return m_userRepo.getUserRole(username);
    }
    return QString();
}

QString AuthService::registerExistingStudent(const QString& email, const QString& username, const QString& password) {

    int studentId = m_studentRepo.findStudentByEmail(email);
    if (studentId == -1) {
        return "This email is not registered. Please contact the administrator.";
    }

    if (m_studentRepo.studentHasUser(studentId)) {
        return "This student already has an account!";
    }

    if (m_userRepo.userExists(username)) {
        return "User already exists!";
    }

    QSqlDatabase db = m_db.getDb();
    if (!db.transaction()) return "Database transaction error.";

    try {

        int newUserId = m_userRepo.addUserReturnId(username, password, "student");
        if (newUserId == -1) throw std::runtime_error("Failed to create account.");

        if (!m_userRepo.linkStudentWithUser(studentId, newUserId)) {
            throw std::runtime_error("Failed to link student to account.");
        }

        db.commit();
        return "";
    } catch (const std::exception& e) {
        db.rollback();
        return e.what();
    }
}
