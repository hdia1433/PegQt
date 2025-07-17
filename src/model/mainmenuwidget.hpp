#pragma once

#include <QWidget>

#include <QVBoxLayout>

#include <QPushButton>
#include <QLabel>

namespace peg
{
    class MainMenu:public QWidget
    {
        Q_OBJECT;

    private:
        QVBoxLayout* mainLayout;
        QVBoxLayout* buttonLayout;

        QLabel* title;
        QPushButton* startGame;
        QPushButton* quit;

    public:
        explicit MainMenu(QWidget* parent = nullptr);
    
    private:
        void setupLayout();
        void setupSignals();
    };
}