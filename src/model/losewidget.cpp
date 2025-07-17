#include "losewidget.hpp"
#include "../helper/global.hpp"
#include "pch.hpp"

namespace peg
{
    LoseWidget::LoseWidget(QWidget* parent):QWidget{parent}, mainLayout(new QVBoxLayout), buttonLayout(new QVBoxLayout), loseLabel(new QLabel("You Lose")), playAgain(new QPushButton("Play Again")), mainMenu(new QPushButton("Main Menu")), quit(new QPushButton("Quit"))
    {
        setupLayout();
        setupSignals();
    }

    void LoseWidget::setupLayout()
    {
        setLayout(mainLayout);

        QFont font = loseLabel->font();
        font.setPointSize(100);
        loseLabel->setFont(font);

        mainLayout->addWidget(loseLabel);
        mainLayout->addLayout(buttonLayout);

        buttonLayout->addStretch();
        buttonLayout->addWidget(playAgain);
        buttonLayout->addStretch();
        buttonLayout->addWidget(mainMenu);
        buttonLayout->addStretch();
        buttonLayout->addWidget(quit);
        buttonLayout->addStretch();
    }

    void LoseWidget::setupSignals()
    {
        connect(playAgain, &QPushButton::pressed, this, []()
        {
            peg::window->setScene(Peg);
        });

        connect(mainMenu, &QPushButton::pressed, this, []()
        {
            peg::window->setScene(Menu);
        });

        connect(quit, &QPushButton::pressed, this, &QApplication::quit);
    }
}