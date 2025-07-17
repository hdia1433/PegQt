#include "mainmenuwidget.hpp"
#include "../helper/global.hpp"
#include <QApplication>

namespace peg
{
    MainMenu::MainMenu(QWidget* parent):QWidget{parent}, mainLayout(new QVBoxLayout), buttonLayout(new QVBoxLayout), title(new QLabel("PEG")), startGame(new QPushButton("Start Game")), quit(new QPushButton("Quit"))
    {
        setupLayout();
        setupSignals();
    }

    void MainMenu::setupLayout()
    {
        setLayout(mainLayout);

        title->setAlignment(Qt::AlignCenter);
        
        QFont font = title->font();
        font.setPointSize(100);
        title->setFont(font);

        mainLayout->addWidget(title);
        mainLayout->addLayout(buttonLayout);

        buttonLayout->addStretch();
        buttonLayout->addWidget(startGame);
        buttonLayout->addStretch();
        buttonLayout->addWidget(quit);
        buttonLayout->addStretch();
    }

    void MainMenu::setupSignals()
    {
        connect(startGame, &QPushButton::pressed, this, []()
        {
            peg::window->setScene(Peg);
        });

        connect(quit, &QPushButton::pressed, this, &QApplication::quit);
    }
}