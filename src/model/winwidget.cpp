#include "winwidget.hpp"
#include "../helper/global.hpp"

namespace peg
{
    WinWidget::WinWidget(QWidget* parent):QWidget{parent}, mainLayout(new QVBoxLayout), buttonLayout(new QVBoxLayout), winLabel(new QLabel("You Win!")), playAgain(new QPushButton("Play Again")), mainMenu(new QPushButton("Main Menu")), quit(new QPushButton("Quit"))
    {
        setupLayout();
        setupSignals();
    }

    void WinWidget::setupLayout()
    {
        setLayout(mainLayout);

        QFont font = winLabel->font();
        font.setPointSize(100);
        winLabel->setFont(font);

        winLabel->setAlignment(Qt::AlignCenter);

        mainLayout->addWidget(winLabel);
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