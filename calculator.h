#ifndef CALCULATOR_H
#define CALCULATOR_H

#include <QWidget>
#include <QPushButton>
#include <QLineEdit>
#include <QGridLayout>
#include <QString>

class calculator : public QWidget
{
    Q_OBJECT

public:
    calculator(QWidget *parent = nullptr);
    ~calculator();

private slots:
    void digitClicked();
    void operatorClicked();
    void equalClicked();
    void backspaceClicked();
    void clearClicked();
    void decimalClicked();

private:
    QLineEdit *display;
    QPushButton *createButton(const QString &text);
    
    double currentValue;
    double storedValue;
    QString pendingOperator;
    bool waitingForOperand;
};
#endif // CALCULATOR_H
