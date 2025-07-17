#pragma once

#include <QMainWindow>
#include <QWidget>
#include <QStackedWidget>
#include <QVBoxLayout>
#include <QCheckBox>
#include <QHBoxLayout>

#include "../model/pegwidget.hpp"
#include "../model/mainmenuwidget.hpp"

namespace peg {
	enum Widgets
	{
		Menu = 0,
		Peg = 1,
		Lose = 2,
		Win = 3
	};

	class PegWindow : public QMainWindow
	{
		Q_OBJECT
	private:
		//widgets
		QStackedWidget* stack;

	public:
		PegWindow(QWidget *parent = nullptr);

		void setScene(Widgets widget);
	};

}
