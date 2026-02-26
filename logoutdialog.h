#ifndef LOGOUTDIALOG_H
#define LOGOUTDIALOG_H

#include <QDialog>
#include <QPropertyAnimation>

/**
 * @class LogoutDialog
 * @brief Dialog displayed when a user logs out of the system.
 *
 * The LogoutDialog provides visual feedback during the logout process.
 * It may include animations (such as fade-in/fade-out effects) to enhance
 * the user experience before returning to the login or main screen.
 */
class LogoutDialog : public QDialog
{
    Q_OBJECT

public:

    /**
     * @brief Constructs the LogoutDialog.
     *
     * @param parent Pointer to the parent widget (default is nullptr).
     *
     * Initializes the dialog and configures the fade animation.
     */
    explicit LogoutDialog(QWidget *parent = nullptr);

private:

    /**
     * @brief Animation used to create a fade effect for the dialog.
     *
     * Typically controls the window opacity during appearance
     * or disappearance transitions.
     */
    QPropertyAnimation *fadeAnimation;
};

#endif // LOGOUTDIALOG_H
