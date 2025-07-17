#include "../helper/global.hpp"

#include <QApplication>

class Runner
{
public:
	Runner() = delete;

	static int main(int argc, char *argv[])
	{
		QApplication a(argc, argv);
		peg::window = new peg::PegWindow();
		peg::window->show();

		int result = a.exec();

		delete peg::window;

		return result;
	}
};

int main(int argc, char *argv[])
{
	return Runner::main(argc, argv);
}
