#include <QtTest/QtTest>
#include "loginwindow.h"

// ---------------------------
//   MOCK DATABASE
// ---------------------------
class MockDatabase : public Database {
public:
    QString correctUser = "test";
    QString correctPass = "1234";
    QString role = "student";

    bool loginResult = false;

    bool openConnection() override { return true; }
    void closeConnection() override {}

    bool checkLogin(const QString &user, const QString &pass) override {
        return loginResult && user == correctUser && pass == correctPass;
    }

    QString getUserRole(const QString &) override {
        return role;
    }
};

// ---------------------------
//      TEST CLASS
// ---------------------------
class LoginWindowTest : public QObject {
    Q_OBJECT

private slots:

    void test_showPassword_checkbox() {
        LoginWindow w;

        // Hidden by default
        QCOMPARE(w.getPasswordEdit()->echoMode(), QLineEdit::Password);

        w.getShowPasswordCheckBox()->setCheckState(Qt::Checked);
        QCOMPARE(w.getPasswordEdit()->echoMode(), QLineEdit::Normal);

        w.getShowPasswordCheckBox()->setCheckState(Qt::Unchecked);
        QCOMPARE(w.getPasswordEdit()->echoMode(), QLineEdit::Password);
    }


    void test_wrong_login_shows_warning() {
        LoginWindow w;

        MockDatabase *mock = new MockDatabase();
        mock->loginResult = false;
        w.setDatabase(mock);

        w.getUsernameEdit()->setText("wrong");
        w.getPasswordEdit()->setText("wrong");

        QSignalSpy spy(&w, &QDialog::accepted);

        QTest::mouseClick(w.getLoginButton(), Qt::LeftButton);

        QVERIFY(spy.count() == 0);
    }


    void test_correct_student_login_opens_student_menu() {
        LoginWindow w;

        MockDatabase *mock = new MockDatabase();
        mock->loginResult = true;
        mock->role = "student";
        w.setDatabase(mock);

        w.getUsernameEdit()->setText("test");
        w.getPasswordEdit()->setText("1234");

        QSignalSpy spy(&w, &LoginWindow::loginSuccessful);

        // Click the button
        QTest::mouseClick(w.getLoginButton(), Qt::LeftButton);

        // Student => signal should NOT be emitted
        QCOMPARE(spy.count(), 0);
    }


    void test_correct_admin_login_emits_signal() {
        LoginWindow w;

        MockDatabase *mock = new MockDatabase();
        mock->loginResult = true;
        mock->role = "admin";
        w.setDatabase(mock);

        w.getUsernameEdit()->setText("test");
        w.getPasswordEdit()->setText("1234");

        QSignalSpy spy(&w, &LoginWindow::loginSuccessful);

        // Click the button
        QTest::mouseClick(w.getLoginButton(), Qt::LeftButton);

        // Admin => signal MUST be emitted
        QCOMPARE(spy.count(), 1);

        QList<QVariant> args = spy.takeFirst();
        QCOMPARE(args.at(0).toString(), QString("admin"));
    }

};

QTEST_MAIN(LoginWindowTest)
#include "tst_loginwindowtest.moc"
