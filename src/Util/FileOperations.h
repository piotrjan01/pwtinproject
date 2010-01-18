/*
 * FileOperations.h
 *
 *  Created on: Jan 18, 2010
 *      Author: piotrrr
 */

#ifndef FILEOPERATIONS_H_
#define FILEOPERATIONS_H_

#include <string>
#include "../debug/debug.h"
#include "../Main/Main.h"

class FileOperations {
public:

	/**
	 * Appends given data to given file.
	 */
	static void writeToFile(string fname, char* data, int dataSize) {
		FILE *fp = 0;
		fp = fopen(fname.c_str(), "a+");
		if ( ! fp ) Main::getInstance()->handleError("Couldn't open file to write: "+fname);
		fwrite(data, sizeof(char), dataSize, fp);
		PRN_(1, "Wrote to file: "+fname);
	}

};

#endif /* FILEOPERATIONS_H_ */
