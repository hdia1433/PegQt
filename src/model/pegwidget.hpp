#pragma once

#include <QHBoxLayout>

namespace peg {
	struct PegBox
	{
		int row;
		int col;

		QPushButton* peg;

		PegBox();

		QPushButton* operator->();

		void setCoords(const int& row, const int& col);
	};

	class PegWidget:public QWidget
	{
		Q_OBJECT
	private:
		//layouts
		QHBoxLayout* wholeLayout;

		QVBoxLayout* mainLayout;
		QVBoxLayout* board;
		QHBoxLayout* rows[5];
		QHBoxLayout* hLabel;

		QVBoxLayout* menuLayout;

		//widgets
		QLabel* helpLabel;
		PegBox pegs[15];

		QWidget* menu;

		QPushButton* mainMenu;
		QPushButton* reset;
		QPushButton* quit;

		//other
		std::variant<int, std::string> gameBoard[5][5];

	public:
		explicit PegWidget(QWidget* parent = nullptr);

		void init();

	private:
		void setupWidget();
		void setupLayout();
		void setupSignals();
		void readBoard();
		void readBoard(std::vector<unsigned int>* targets);
		void checkBoard();
		void animatePegs(const int& source, const int& middle, const int& target);
	
		void pegClicked(const int& row, const int& col);
		void targetSelected(const unsigned int& sourceIndex, const std::vector<unsigned int>& middles, const std::vector<unsigned int>& targets, const unsigned int& index);
	};
}