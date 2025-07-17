#pragma once

#include <QWidget>
#include <QPushButton>
#include <QLabel>

#include <QVBoxLayout>

namespace peg
{
    class LoseWidget:public QWidget
    {
        Q_OBJECT;
    private:
        QVBoxLayout* mainLayout;
        QVBoxLayout* buttonLayout;

        QLabel* loseLabel;
        QPushButton* playAgain;
        QPushButton* mainMenu;
        QPushButton* quit;
    
    public:
        explicit LoseWidget(QWidget* parent = nullptr);

    private:
        void setupLayout();
        void setupSignals();
    };
}