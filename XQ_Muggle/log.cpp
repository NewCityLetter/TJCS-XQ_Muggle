#include <iostream>
#include <fstream>
#include <string>
#include <stdarg.h>
#include "log.h"

Log a;

void Log::clear()
{
	out.open(path, std::ios::out | std::ios::binary);
	if (out.is_open())
	{
		out << __DATE__ << "--" << __TIME__ << ":\n";
		out << "?¯" << std::endl;
		out.close();
	}
}


void Log::baseMsg()
{
	out.open(path, std::ios::out | std::ios::binary | std::ios::ate | std::ios::app);
	if (out.is_open())
	{
		out << __DATE__ << "--" << __TIME__ << ":\n";
		out.close();
	}
}

void Log::error(std::string msg)
{
	baseMsg();
	out.open(path, std::ios::out | std::ios::binary | std::ios::ate | std::ios::app);
	if (out.is_open())
	{
		out << "ERROR: " << msg << std::endl;
		out.close();
	}
}