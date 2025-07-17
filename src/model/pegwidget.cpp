#include "pegwidget.hpp"
#include "../helper/autoflushcout.hpp"
#include "../helper/methods.hpp"
#include "../helper/global.hpp"

#include <print>

#include <QApplication>
#include <QPropertyAnimation>
#include <QSequentialAnimationGroup>
#include <QParallelAnimationGroup>
#include <QEventLoop>

namespace peg
{
	PegBox::PegBox():peg(new QPushButton)
	{
		peg->setCheckable(true);
		peg->setFixedSize(40, 40);
		peg->setText("");
		peg->setFocusPolicy(Qt::NoFocus);


		peg->setStyleSheet(R"(
			QPushButton {
				border: 2px solid black;
				border-radius: 4px;
				padding: 0;
				margin: 0;
				background-color: #7c7c7c;
			}

			QPushButton:checked {
				background-color: #003cff;
			}

			QPushButton:disabled {
				background-color: #444444;
			}

			QPushButton:disabled:checked {
				background-color: #011f60;
			}
		)");
		
	}

	QPushButton* PegBox::operator->()
	{
		return peg;
	}

	void PegBox::setCoords(const int& row, const int& col)
	{
		this->row = row;
		this->col = col;
	}

	PegWidget::PegWidget(QWidget* parent):QWidget{parent}, hLabel(new QHBoxLayout), helpLabel(new QLabel("Blue: Pegs\n\tLight: Can Jump\n\tDark: Can't Jump\nGrey: Empty Spots\n"))
	{
		init();
		setupLayout();
	}

	void PegWidget::init()
	{
		setupWidget();

		int currentIndex = 0;
		int numInRow = std::size(gameBoard[0]);
		
		for(int row = 0; row < std::size(gameBoard); row++)
		{
			for(int col = 0; col < std::size(gameBoard[row]); col++)
			{
				if(col < numInRow)
				{
					gameBoard[row][col] = currentIndex;

					pegs[currentIndex].setCoords(row, col);

					disconnect(pegs[currentIndex].peg, nullptr, nullptr, nullptr);

					currentIndex++;
					continue;
				}
				gameBoard[row][col] = "None";
			}
			numInRow--;
		}

		setupSignals();
		readBoard();
	}

	void PegWidget::setupWidget()
	{
		wholeLayout = new QHBoxLayout;
		mainLayout = new QVBoxLayout();
		board = new QVBoxLayout();

		menu = new QWidget;
		menuLayout = new QVBoxLayout;

		mainMenu = new QPushButton("Main Menu");
		reset = new QPushButton("Reset");
		quit = new QPushButton("Quit");

		for(int i = 0; i < std::size(pegs); i++)
		{
			pegs[i]->setFocusPolicy(Qt::NoFocus);
			if(i == 10)
			{
				pegs[i]->setChecked(false);
				continue;
			}
			pegs[i]->setChecked(true);
		}

		for(int i = 0; i < std::size(rows); i++)
		{
			rows[i] = new QHBoxLayout();
		}
	}

	void PegWidget::setupLayout()
	{
		wholeLayout->addLayout(mainLayout);

		mainLayout->addStretch();
		mainLayout->addLayout(hLabel);
		
		hLabel->addStretch();
		hLabel->addWidget(helpLabel);
		hLabel->addStretch();

		mainLayout->addStretch();
		mainLayout->addLayout(board);
		mainLayout->addStretch();

		wholeLayout->addWidget(menu);
		menu->setLayout(menuLayout);

		menu->setFixedWidth(200);
		menu->setObjectName("Menu");
		menu->setStyleSheet(R"(
			#Menu 
			{
				background-color: black;
			}
			)");

		setLayout(wholeLayout);

		menuLayout->addWidget(mainMenu);
		menuLayout->addWidget(reset);
		menuLayout->addWidget(quit);

		int pegIndex = 0;
		int numInRow = 5;

		for(QHBoxLayout* row: rows)
		{
			row->setAlignment(Qt::AlignCenter);
			row->setSpacing(20);
			board->addLayout(row);

			for(int i = 0; i < numInRow; i++)
			{
				row->addWidget(pegs[pegIndex].peg);

				pegIndex++;
			}
			numInRow--;
		}
	}

	void PegWidget::setupSignals()
	{
		for (PegBox& peg: pegs)
		{
			int row = peg.row;
			int col = peg.col;
			connect(peg.peg, &QPushButton::clicked, this, [this, row, col]()
			{
				pegClicked(row, col);
			});
		}

		connect(mainMenu, &QPushButton::clicked, this, []()
		{
			peg::window->setScene(Menu);
		});

		connect(reset, &QPushButton::clicked, this, []()
		{
			peg::window->setScene(Peg);
		});

		connect(quit, &QPushButton::clicked, this, &QApplication::quit);
	}

	void PegWidget::pegClicked(const int& row, const int& col)
	{
		int pegIndex = std::get<int>(gameBoard[row][col]);
		pegs[pegIndex]->setChecked(false);

		std::vector<unsigned int> targets;
		std::vector<unsigned int> middles;

		if(col + 2 < std::size(gameBoard[row]) && 
		std::holds_alternative<int>(gameBoard[row][col + 2]))
		{
			unsigned int target = std::get<int>(gameBoard[row][col + 2]);
			unsigned int middle = std::get<int>(gameBoard[row][col + 1]);

			if (!pegs[target]->isChecked() && pegs[middle]->isChecked())
			{

				targets.emplace_back(target);
				middles.emplace_back(middle);
			}
		}
		if (col - 2 >= 0 &&
		std::holds_alternative<int>(gameBoard[row][col - 2]))
		{
			unsigned int target = std::get<int>(gameBoard[row][col - 2]);
			unsigned int middle = std::get<int>(gameBoard[row][col - 1]);

			if (!pegs[target]->isChecked() && pegs[middle]->isChecked())
			{
				targets.emplace_back(target);
				middles.emplace_back(middle);
			}
		}
		if (row + 2 < std::size(gameBoard) &&
		std::holds_alternative<int>(gameBoard[row + 2][col]))
		{
			unsigned int target = std::get<int>(gameBoard[row + 2][col]);
			unsigned int middle = std::get<int>(gameBoard[row + 1][col]);

			if (!pegs[target]->isChecked() && pegs[middle]->isChecked())
			{
				targets.emplace_back(target);
				middles.emplace_back(middle);
			}
		}
		if (row - 2 >= 0 &&
		std::holds_alternative<int>(gameBoard[row - 2][col]))
		{
			unsigned int target = std::get<int>(gameBoard[row - 2][col]);
			unsigned int middle = std::get<int>(gameBoard[row - 1][col]);

			if (!pegs[target]->isChecked() && pegs[middle]->isChecked())
			{
				targets.emplace_back(target);
				middles.emplace_back(middle);
			}
		}
		if (row - 2 >= 0 &&
		col + 2 < std::size(gameBoard[row]) &&
		std::holds_alternative<int>(gameBoard[row - 2][col + 2]))
		{
			unsigned int target = std::get<int>(gameBoard[row - 2][col + 2]);
			unsigned int middle = std::get<int>(gameBoard[row - 1][col + 1]);

			if (!pegs[target]->isChecked() && pegs[middle]->isChecked())
			{
				targets.emplace_back(target);
				middles.emplace_back(middle);
			}
		}
		if (row + 2 < std::size(gameBoard) &&
		col - 2 >= 0 &&
		std::holds_alternative<int>(gameBoard[row + 2][col - 2]))
		{
			unsigned int target = std::get<int>(gameBoard[row + 2][col - 2]);
			unsigned int middle = std::get<int>(gameBoard[row + 1][col - 1]);

			if (!pegs[target]->isChecked() && pegs[middle]->isChecked())
			{
				targets.emplace_back(target);
				middles.emplace_back(middle);
			}
		}

		if(targets.size() == 1)
		{
			pegs[middles[0]]->setChecked(false);
			pegs[pegIndex]->setDisabled(true);
			pegs[middles[0]]->setDisabled(true);

			animatePegs(pegIndex, middles[0], targets[0]);

			pegs[targets[0]]->setChecked(true);

			readBoard();
		}
		else
		{
			pegs[pegIndex]->setChecked(true);

			for(unsigned int i = 0; i < std::size(pegs); i++)
			{
				if(in(i, targets))
				{
					pegs[i]->setEnabled(true);
				}
				else
				{
					pegs[i]->setDisabled(true);
				}
			}

			for(unsigned int i = 0; i < std::size(targets); i++)
			{
				disconnect(pegs[targets[i]].peg, nullptr, nullptr, nullptr);

				connect(pegs[targets[i]].peg, &QPushButton::clicked, this, [this, pegIndex, middles, i, targets]()
				{
					targetSelected(pegIndex, middles, targets, i);
				});
			}

			helpLabel->setText("Press the light grey box to pick where you want the peg to go.");

			readBoard(&targets);
		}
	}

	void PegWidget::targetSelected(const unsigned int& sourceIndex, const std::vector<unsigned int>& middles, const std::vector<unsigned int>& targets, const unsigned int& index)
	{
		pegs[sourceIndex]->setChecked(false);

		animatePegs(sourceIndex, middles[index], targets[index]);

		pegs[middles[index]]->setChecked(false);

		for(unsigned int target: targets)
		{
			unsigned int row = pegs[target].row;
			unsigned int col = pegs[target].col;
			disconnect(pegs[target].peg, nullptr, nullptr, nullptr);

			connect(pegs[target].peg, &QPushButton::clicked, this, [this, row, col]()
			{
				pegClicked(row, col);
			});
		}

		for (PegBox& peg : pegs) {
			disconnect(peg.peg, nullptr, nullptr, nullptr); // Reset all
		}
		setupSignals();  // Reattach all
		readBoard(); 

		readBoard();
	}

	void PegWidget::readBoard()
	{
		for(int row = 0; row < std::size(gameBoard); row++)
		{
			for(int col = 0; col < std::size(gameBoard[row]); col++)
			{
				if(std::holds_alternative<std::string>(gameBoard[row][col]))
				{
					continue;
				}
				else if(std::holds_alternative<int>(gameBoard[row][col]) &&
				!pegs[std::get<int>(gameBoard[row][col])]->isChecked())
				{
					pegs[std::get<int>(gameBoard[row][col])]->setDisabled(true);
					continue;
				}

				if(col + 2 < std::size(gameBoard[row]) && 
				std::holds_alternative<int>(gameBoard[row][col + 2]))
				{
					int target = std::get<int>(gameBoard[row][col + 2]);
					int middle = std::get<int>(gameBoard[row][col + 1]);

					if (!pegs[target]->isChecked() && pegs[middle]->isChecked())
					{
						pegs[std::get<int>(gameBoard[row][col])]->setEnabled(true);
						continue;
					}
				}
				if (col - 2 >= 0 &&
				std::holds_alternative<int>(gameBoard[row][col - 2]))
				{
					int target = std::get<int>(gameBoard[row][col - 2]);
					int middle = std::get<int>(gameBoard[row][col - 1]);

					if (!pegs[target]->isChecked() && pegs[middle]->isChecked())
					{
						pegs[std::get<int>(gameBoard[row][col])]->setEnabled(true);
						continue;
					}
				}
				if (row + 2 < std::size(gameBoard) &&
				std::holds_alternative<int>(gameBoard[row + 2][col]))
				{
					int target = std::get<int>(gameBoard[row + 2][col]);
					int middle = std::get<int>(gameBoard[row + 1][col]);

					if (!pegs[target]->isChecked() && pegs[middle]->isChecked())
					{
						pegs[std::get<int>(gameBoard[row][col])]->setEnabled(true);
						continue;
					}
				}
				if (row - 2 >= 0 &&
				std::holds_alternative<int>(gameBoard[row - 2][col]))
				{
					int target = std::get<int>(gameBoard[row - 2][col]);
					int middle = std::get<int>(gameBoard[row - 1][col]);

					if (!pegs[target]->isChecked() && pegs[middle]->isChecked())
					{
						pegs[std::get<int>(gameBoard[row][col])]->setEnabled(true);
						continue;
					}
				}
				if (row - 2 >= 0 &&
				col + 2 < std::size(gameBoard[row]) &&
				std::holds_alternative<int>(gameBoard[row - 2][col + 2]))
				{
					int target = std::get<int>(gameBoard[row - 2][col + 2]);
					int middle = std::get<int>(gameBoard[row - 1][col + 1]);

					if (!pegs[target]->isChecked() && pegs[middle]->isChecked())
					{
						pegs[std::get<int>(gameBoard[row][col])]->setEnabled(true);
						continue;
					}
				}
				if (row + 2 < std::size(gameBoard) &&
				col - 2 >= 0 &&
				std::holds_alternative<int>(gameBoard[row + 2][col - 2]))
				{
					int target = std::get<int>(gameBoard[row + 2][col - 2]);
					int middle = std::get<int>(gameBoard[row + 1][col - 1]);

					if (!pegs[target]->isChecked() && pegs[middle]->isChecked())
					{
						pegs[std::get<int>(gameBoard[row][col])]->setEnabled(true);
						continue;
					}
				}
				
				
				pegs[std::get<int>(gameBoard[row][col])]->setDisabled(true);
			}
		}

		helpLabel->setText("Blue: Pegs\n\tLight: Can Jump\n\tDark: Can't Jump\nGrey: Empty Spots\n");

		checkBoard();
	}

	void PegWidget::readBoard(std::vector<unsigned int>* targets)
	{
		for(int row = 0; row < std::size(gameBoard); row++)
		{
			for(int col = 0; col < std::size(gameBoard[row]); col++)
			{
				if(std::holds_alternative<std::string>(gameBoard[row][col]))
				{
					continue;
				}
				if(targets && in<unsigned int>(std::get<int>(gameBoard[row][col]), *targets))
				{
					continue;
				}
				pegs[std::get<int>(gameBoard[row][col])]->setDisabled(true);
				continue;
			}
		}
	}

	void PegWidget::checkBoard()
	{
		unsigned int disabledCount = 0;
		unsigned int checkedCount = 0;

		for(auto& peg: pegs)
		{
			if(peg->isChecked())
			{
				checkedCount++;
			}
			if(!peg->isEnabled())
			{
				disabledCount++;
			}
		}

		if(checkedCount == 1)
		{
			peg::window->setScene(Win);
		}
		else if(disabledCount == 15)
		{
			peg::window->setScene(Lose);
		}
	}

	void PegWidget::animatePegs(const int& source, const int& middle, const int& target)
	{
		QLabel* ghostPegJump = new QLabel(this);
		ghostPegJump->setStyleSheet(R"(
			background-color: #003cff;
			border: 2px solid black;
			border-radius: 4px;
		)");
		ghostPegJump->show();

		QPoint startPos = pegs[source]->mapTo(this, QPoint(0, 0));
		QRect startRect(startPos, pegs[source]->size());
		ghostPegJump->setGeometry(startRect);

		QPoint endPos = pegs[target]->mapTo(this, QPoint(0, 0));
		QRect endRect(endPos, pegs[target]->size());

		QPoint middlePos = pegs[middle]->mapTo(this, QPoint(0, 0));
		QRect bigRect = startRect;
		bigRect.moveTo(middlePos);
		bigRect.adjust(-5, -5, 5, 5);
		

		QLabel* ghostPegDie = new QLabel(this);
		ghostPegDie->setStyleSheet(R"(
			background-color: #003cff;
			border: 2px solid black;
			border-radius: 4px;
		)");
		ghostPegDie->show();

		ghostPegJump->raise();

		QRect dieRectBig(middlePos, pegs[middle]->size());
		ghostPegDie->setGeometry(dieRectBig);

		QRect dieRectSmall = dieRectBig.adjusted(20, 20, -20, -20);

		QPropertyAnimation* grow = new QPropertyAnimation(ghostPegJump, "geometry");
		grow->setDuration(300);
		grow->setStartValue(startRect);
		grow->setEndValue(bigRect);
		grow->setEasingCurve(QEasingCurve::OutQuad);

		QPropertyAnimation* shrink = new QPropertyAnimation(ghostPegJump, "geometry");
		shrink->setDuration(300);
		shrink->setStartValue(bigRect);
		shrink->setEndValue(endRect);
		shrink->setEasingCurve(QEasingCurve::InQuad);

		QPropertyAnimation* dieShrink = new QPropertyAnimation(ghostPegDie, "geometry");
		dieShrink->setDuration(300);
		dieShrink->setStartValue(dieRectBig);
		dieShrink->setEndValue(dieRectSmall);
		dieShrink->setEasingCurve(QEasingCurve::OutQuad);

		QSequentialAnimationGroup* animGroup = new QSequentialAnimationGroup;
		animGroup->addAnimation(grow);
		animGroup->addAnimation(shrink);
		animGroup->addAnimation(dieShrink);

		QEventLoop loop;

		connect(animGroup, &QSequentialAnimationGroup::finished, &loop, &QEventLoop::quit);

		animGroup->start(QAbstractAnimation::DeleteWhenStopped);
		loop.exec();

		ghostPegJump->deleteLater();
		ghostPegDie->deleteLater();
	}
}
