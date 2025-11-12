#ifndef CALCULATOR_H
#define CALCULATOR_H

#include <QWidget>

class calculator : public QWidget
{
    Q_OBJECT

public:
    calculator(QWidget *parent = nullptr);
    ~calculator();
};
#endif // CALCULATOR_H
