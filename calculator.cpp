#include "calculator.h"
#include <QGridLayout>
#include <QFont>

calculator::calculator(QWidget *parent)
    : QWidget(parent), currentValue(0.0), storedValue(0.0), waitingForOperand(true)
{
    // Create the display (LCD-like)
    display = new QLineEdit("0");
    display->setReadOnly(true);
    display->setAlignment(Qt::AlignRight);
    display->setMaxLength(15);
    
    QFont font = display->font();
    font.setPointSize(18);
    display->setFont(font);
    display->setMinimumHeight(50);
    
    // Create layout
    QGridLayout *mainLayout = new QGridLayout;
    mainLayout->addWidget(display, 0, 0, 1, 4);
    
    // Row 1: ← / * -
    QPushButton *backspaceButton = createButton("←");
    QPushButton *divideButton = createButton("/");
    QPushButton *multiplyButton = createButton("*");
    QPushButton *subtractButton = createButton("-");
    
    mainLayout->addWidget(backspaceButton, 1, 0);
    mainLayout->addWidget(divideButton, 1, 1);
    mainLayout->addWidget(multiplyButton, 1, 2);
    mainLayout->addWidget(subtractButton, 1, 3);
    
    connect(backspaceButton, &QPushButton::clicked, this, &calculator::backspaceClicked);
    connect(divideButton, &QPushButton::clicked, this, &calculator::operatorClicked);
    connect(multiplyButton, &QPushButton::clicked, this, &calculator::operatorClicked);
    connect(subtractButton, &QPushButton::clicked, this, &calculator::operatorClicked);
    
    // Row 2: 7 8 9 +
    QPushButton *button7 = createButton("7");
    QPushButton *button8 = createButton("8");
    QPushButton *button9 = createButton("9");
    QPushButton *addButton = createButton("+");
    
    mainLayout->addWidget(button7, 2, 0);
    mainLayout->addWidget(button8, 2, 1);
    mainLayout->addWidget(button9, 2, 2);
    mainLayout->addWidget(addButton, 2, 3);
    
    connect(button7, &QPushButton::clicked, this, &calculator::digitClicked);
    connect(button8, &QPushButton::clicked, this, &calculator::digitClicked);
    connect(button9, &QPushButton::clicked, this, &calculator::digitClicked);
    connect(addButton, &QPushButton::clicked, this, &calculator::operatorClicked);
    
    // Row 3: 4 5 6 (empty space spans 2 rows)
    QPushButton *button4 = createButton("4");
    QPushButton *button5 = createButton("5");
    QPushButton *button6 = createButton("6");
    
    mainLayout->addWidget(button4, 3, 0);
    mainLayout->addWidget(button5, 3, 1);
    mainLayout->addWidget(button6, 3, 2);
    
    connect(button4, &QPushButton::clicked, this, &calculator::digitClicked);
    connect(button5, &QPushButton::clicked, this, &calculator::digitClicked);
    connect(button6, &QPushButton::clicked, this, &calculator::digitClicked);
    
    // Row 4: 1 2 3 ↲ (equals button spans 2 rows)
    QPushButton *button1 = createButton("1");
    QPushButton *button2 = createButton("2");
    QPushButton *button3 = createButton("3");
    QPushButton *equalButton = createButton("=");
    
    mainLayout->addWidget(button1, 4, 0);
    mainLayout->addWidget(button2, 4, 1);
    mainLayout->addWidget(button3, 4, 2);
    mainLayout->addWidget(equalButton, 3, 3, 2, 1); // Span 2 rows
    
    connect(button1, &QPushButton::clicked, this, &calculator::digitClicked);
    connect(button2, &QPushButton::clicked, this, &calculator::digitClicked);
    connect(button3, &QPushButton::clicked, this, &calculator::digitClicked);
    connect(equalButton, &QPushButton::clicked, this, &calculator::equalClicked);
    
    // Row 5: 0 (spans 2 columns) .
    QPushButton *button0 = createButton("0");
    QPushButton *decimalButton = createButton(".");
    
    mainLayout->addWidget(button0, 5, 0, 1, 2); // Span 2 columns
    mainLayout->addWidget(decimalButton, 5, 2);
    
    connect(button0, &QPushButton::clicked, this, &calculator::digitClicked);
    connect(decimalButton, &QPushButton::clicked, this, &calculator::decimalClicked);
    
    setLayout(mainLayout);
    setWindowTitle("Calculator");
}

calculator::~calculator() {}

QPushButton *calculator::createButton(const QString &text)
{
    QPushButton *button = new QPushButton(text);
    QFont font = button->font();
    font.setPointSize(14);
    button->setFont(font);
    button->setMinimumSize(60, 50);
    return button;
}

void calculator::digitClicked()
{
    QPushButton *clickedButton = qobject_cast<QPushButton*>(sender());
    QString digitValue = clickedButton->text();
    
    if (waitingForOperand) {
        display->clear();
        waitingForOperand = false;
    }
    
    if (display->text() == "0" && digitValue != "0") {
        display->setText(digitValue);
    } else {
        display->setText(display->text() + digitValue);
    }
}

void calculator::operatorClicked()
{
    QPushButton *clickedButton = qobject_cast<QPushButton*>(sender());
    QString clickedOperator = clickedButton->text();
    double operand = display->text().toDouble();
    
    if (!pendingOperator.isEmpty()) {
        if (!waitingForOperand) {
            double result = 0.0;
            if (pendingOperator == "+") {
                result = storedValue + operand;
            } else if (pendingOperator == "-") {
                result = storedValue - operand;
            } else if (pendingOperator == "*") {
                result = storedValue * operand;
            } else if (pendingOperator == "/") {
                if (operand != 0.0) {
                    result = storedValue / operand;
                } else {
                    display->setText("Error");
                    storedValue = 0.0;
                    pendingOperator.clear();
                    waitingForOperand = true;
                    return;
                }
            }
            display->setText(QString::number(result));
            storedValue = result;
        }
    } else {
        storedValue = operand;
    }
    
    pendingOperator = clickedOperator;
    waitingForOperand = true;
}

void calculator::equalClicked()
{
    double operand = display->text().toDouble();
    
    if (!pendingOperator.isEmpty()) {
        double result = 0.0;
        if (pendingOperator == "+") {
            result = storedValue + operand;
        } else if (pendingOperator == "-") {
            result = storedValue - operand;
        } else if (pendingOperator == "*") {
            result = storedValue * operand;
        } else if (pendingOperator == "/") {
            if (operand != 0.0) {
                result = storedValue / operand;
            } else {
                display->setText("Error");
                storedValue = 0.0;
                pendingOperator.clear();
                waitingForOperand = true;
                return;
            }
        }
        display->setText(QString::number(result));
        storedValue = 0.0;
        pendingOperator.clear();
    }
    
    waitingForOperand = true;
}

void calculator::backspaceClicked()
{
    if (!waitingForOperand) {
        QString text = display->text();
        text.chop(1);
        if (text.isEmpty()) {
            text = "0";
            waitingForOperand = true;
        }
        display->setText(text);
    }
}

void calculator::clearClicked()
{
    display->setText("0");
    storedValue = 0.0;
    pendingOperator.clear();
    waitingForOperand = true;
}

void calculator::decimalClicked()
{
    if (waitingForOperand) {
        display->setText("0.");
        waitingForOperand = false;
    } else if (!display->text().contains(".")) {
        display->setText(display->text() + ".");
    }
}
