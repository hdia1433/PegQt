#include "winwidget.hpp"
#include "../helper/global.hpp"

namespace peg
{
    WinWidget::WinWidget(QWidget* parent):QWidget{parent}, mainLayout(new QVBoxLayout), buttonLayout(new QVBoxLayout), loseLabel(new QLabel("You Win!")), playAgain(new QPushButton("Play Again")), mainMenu(new QPushButton("Main Menu")), quit(new QPushButton("Quit"))
    {
        setupLayout();
        setupSignals();
    }

    void WinWidget::setupLayout()
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

    void WinWidget::setupSignals()
    {
        connect(playAgain, &QPushButton::pressed, this, []()
        {
            peg::window->setScene(Peg);
        });
    }
}