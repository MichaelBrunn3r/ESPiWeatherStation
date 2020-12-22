#pragma once

#ifndef MAX_LOGLEVEL
	#define MAX_LOGLEVEL 1
#endif

#ifdef LOGGING
	#define LOG(x) Serial.println(x)
	#define LOGF(format, args) Serial.printf(format, args)
	#define ERROR(x) Serial.println("Error: " + x)
	#define ERRORF(format, args) Serial.printf("Error: " + format, args)
#else
	#define LOG(x)
	#define LOGF(format, args)
	#define ERROR(x)
	#define ERRORF(format, args)
#endif