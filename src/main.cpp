
#include "Main/Main.h"

using namespace std;


int main(int argc, char **argv) {
	/* Jedyne co robimy to przekazujemy argumenty do metody
	 * startProgram w klasie Main i dzia�amy ju� tylko na obiektach.
	 */
	return Main::getMain()->startProgram(argc, argv);
}

