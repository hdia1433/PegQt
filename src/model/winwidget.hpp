#pragma once

#include <QWidget>
#include <QPushButton>
#include <QLabel>

#include <QVBoxLayout>

namespace peg
{
    class WinWidget:public QWidget
    {
        Q_OBJECT;
    private:
        QVBoxLayout* mainLayout;
        QVBoxLayout* buttonLayout;

        QLabel* winLabel;
        QPushButton* playAgain;
        QPushButton* mainMenu;
        QPushButton* quit;
    
    public:
        explicit WinWidget(QWidget* parent = nullptr);

    private:
        void setupLayout();
        void setupSignals();
    };
}