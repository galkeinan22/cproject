#include "SPLogger.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <Math.h>


//File open mode
#define SP_LOGGER_OPEN_MODE "w"

// Global variable holding the logger
SPLogger logger = NULL;

struct sp_logger_t {
	FILE* outputChannel; //The logger file
	bool isStdOut; //Indicates if the logger is stdout
	SP_LOGGER_LEVEL level; //Indicates the level
};



//our own new help method: print method to prevent code copy
SP_LOGGER_MSG Print_Info (const char* type, const char* msg, const char* file,
		const char* function, const int line){

	// printing  messages:

	//1-type MSG
	spLoggerPrintMsg(type);

	//2-file MSG
	int f_len = strlen(file);
	char * f_name = (char*) malloc ((f_len+12)*sizeof(char));
	if (f_name ==NULL) //malloc failed
		return SP_LOGGER_OUT_OF_MEMORY;
	sprintf( f_name ,"- file: %s\n",file );
	spLoggerPrintMsg(f_name);
	free(f_name);

	//3-function MSG
	int func_len = strlen(function);
	char * func_name = (char*) malloc ((func_len+15)*sizeof(char));
	if (func_name ==NULL) //malloc failed
		return SP_LOGGER_OUT_OF_MEMORY;
	sprintf( func_name ,"- function: %s\n",function );
	spLoggerPrintMsg(func_name);
	free(func_name);

	//4-line MSG
	int l_len = 32; //number of digits of line is probably less then 10**32;
	//printf ("%d\n", l_len);
	char * l_name = (char*) malloc ((l_len+12)*sizeof(char));
	if (l_name ==NULL) //malloc failed
		return SP_LOGGER_OUT_OF_MEMORY;
	sprintf( l_name ,"- line: %d\n",line );
	spLoggerPrintMsg(l_name);
	free(l_name);

	//5-massage by user
	int msg_len = strlen(msg);
	char * msg_name = (char*) malloc ((msg_len+14)*sizeof(char));
	if (msg_name ==NULL) //malloc failed
		return SP_LOGGER_OUT_OF_MEMORY;
	sprintf( msg_name , "- message: %s\n",msg );
	int rc = spLoggerPrintMsg(msg_name);
	free(msg_name);
	return rc;

}

SP_LOGGER_MSG spLoggerCreate(const char* filename, SP_LOGGER_LEVEL level) {
	if (logger != NULL) { //Already defined
		return SP_LOGGER_DEFINED;
	}
	logger = (SPLogger) malloc(sizeof(*logger));
	if (logger == NULL) { //Allocation failure
		return SP_LOGGER_OUT_OF_MEMORY;
	}
	logger->level = level; //Set the level of the logger
	if (filename == NULL) { //In case the filename is not set use stdout
		logger->outputChannel = stdout;
		logger->isStdOut = true;
	} else { //Otherwise open the file in write mode
		logger->outputChannel = fopen(filename, SP_LOGGER_OPEN_MODE);
		if (logger->outputChannel == NULL) { //Open failed
			free(logger);
			logger = NULL;
			return SP_LOGGER_CANNOT_OPEN_FILE;
		}
		logger->isStdOut = false;
	}
	return SP_LOGGER_SUCCESS;
}

void spLoggerDestroy() {
	if (!logger) {
		return;
	}
	if (!logger->isStdOut) {//Close file only if not stdout
		fclose(logger->outputChannel);
	}
	free(logger);//free allocation
	logger = NULL;
}

SP_LOGGER_MSG spLoggerPrintError(const char* msg, const char* file,
		const char* function, const int line){
	if (logger == NULL)
		return SP_LOGGER_UNDIFINED;

	if (line <0  ||msg ==NULL|| file ==NULL )
		return SP_LOGGER_INVAlID_ARGUMENT;

	return Print_Info("---ERROR---\n",msg, file,function, line);
}


SP_LOGGER_MSG spLoggerPrintWarning(const char* msg, const char* file,
		const char* function, const int line){
	if (logger == NULL)
		return SP_LOGGER_UNDIFINED;
	if (line <0  ||msg ==NULL|| file ==NULL )
		return SP_LOGGER_INVAlID_ARGUMENT;

	//printing WARNING MSG to user:
	if (logger->level >= SP_LOGGER_WARNING_ERROR_LEVEL){
		return Print_Info("---WARNING---\n",msg, file,function, line);
	}
	return SP_LOGGER_SUCCESS;

}

SP_LOGGER_MSG spLoggerPrintInfo(const char* msg){
	if (logger == NULL)
		return SP_LOGGER_UNDIFINED;
	if (msg ==NULL)
		return SP_LOGGER_INVAlID_ARGUMENT;
	if (logger->level < SP_LOGGER_INFO_WARNING_ERROR_LEVEL)
		return SP_LOGGER_SUCCESS;
	//printing WARNING MSG to user:

	//1-INFO HEADER
	spLoggerPrintMsg("---INFO---\n");

	// printing msg to user
	int msg_len = strlen(msg);
	char * msg_name = (char*) malloc ((msg_len+14)*sizeof(char));
	if (msg_name ==NULL) //malloc failed
		return SP_LOGGER_OUT_OF_MEMORY;
	sprintf( msg_name , "- message: %s\n",msg );
	return (spLoggerPrintMsg(msg_name));
}


SP_LOGGER_MSG spLoggerPrintDebug(const char* msg, const char* file,
		const char* function, const int line){
	if (logger == NULL)
		return SP_LOGGER_UNDIFINED;
	if (line <0  ||msg ==NULL|| file ==NULL )
		return SP_LOGGER_INVAlID_ARGUMENT;

	if (logger->level >= SP_LOGGER_DEBUG_INFO_WARNING_ERROR_LEVEL)
		return Print_Info("---DEBUG---\n",msg, file,function, line);
	return SP_LOGGER_SUCCESS;
}




SP_LOGGER_MSG spLoggerPrintMsg(const char* msg){
	if (logger == NULL)  //Already defined
		return SP_LOGGER_UNDIFINED;
	if (msg ==NULL)
		return SP_LOGGER_INVAlID_ARGUMENT;
	if (fprintf(logger->outputChannel , msg)== 0) // write failiure
		return SP_LOGGER_WRITE_FAIL	;
	return SP_LOGGER_SUCCESS;

}
