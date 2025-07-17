#include "pegwindow.hpp"
#include "../model/losewidget.hpp"
#include "../model/winwidget.hpp"

using namespace peg;

PegWindow::PegWindow(QWidget *parent):QMainWindow(parent)
{
	resize(600, 400);
	setWindowTitle("Peg");

	stack = new QStackedWidget;

	setScene(Menu);

	stack->addWidget(new MainMenu());
	stack->addWidget(new PegWidget);
	stack->addWidget(new LoseWidget);
	stack->addWidget(new WinWidget);

	setCentralWidget(stack);
}

void PegWindow::setScene(Widgets widget)
{
	stack->setCurrentIndex(widget);

	if(widget == Peg)
	{
		qobject_cast<PegWidget*>(stack->currentWidget())->init();
	}
}
