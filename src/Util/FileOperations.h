/**
 * \~
 * @file FileOperations.h
 * \~polish
 * Klasa opakowująca używane operacje na plikach.
 *
 * \~english
 * Wrapper classes for file manipulation functions.
 *
 * \~
 * @date created 2010-01-18
 * @author: Piotr Gwizdała
 */

#ifndef FILEOPERATIONS_H_
#define FILEOPERATIONS_H_

#include <string>
#include "../Debug/Debug.h"
#include "../Main/Main.h"

/**
 * \~
 * \~polish
 * Klasa opakowująca używane operacje na plikach.
 *
 * \~english
 * Wrapper classes for file manipulation functions.
 *
 * \~
 * @date created 2010-01-18
 * @author: Piotr Gwizdała
 */
class FileOperations {
public:
	/**
	 * \~
	 * \~polish
	 * Dopisuje dane o określonym rozmiarze do pliku o nazwe <code>filename</code>.
	 * \~english
	 * Appends data of given size to the file identified by <code>filename</code>.
	 * \~
	 */
	static void writeToFile(string filename, char* data, int dataSize) {
		FILE *fp = 0;
		fp = fopen(filename.c_str(), "a+");
		if (!fp)
			Main::getInstance()->handleError("Couldn't open file to write: "
					+ filename);
		fseek(fp, 0L, SEEK_END);
		fwrite(data, sizeof(char), dataSize, fp);
		fclose(fp);
		PRN_(1, "Wrote to file: "+filename);
	}

};

#endif /* FILEOPERATIONS_H_ */
