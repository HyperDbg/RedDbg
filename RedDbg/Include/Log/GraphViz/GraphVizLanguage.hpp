#pragma once
#include <ntddk.h>

class GraphVizLanguage
{
public:
	PCH NameOfGraph = nullptr;
	CHAR DigraphQuote[9] = "digraph ";
	CHAR StartOfdigraph[16] = " {\n"; CHAR EndOfdigraph[2] = "}";
	CHAR Line[5] = " -> ";
	CHAR StartOfLabel[3] = " [";
	CHAR EndOfLabel[3] = "]\n";
	CHAR Label[8] = "label=\"";
	CHAR Shape[7] = "shape=";
	CHAR RecordShape[9] = "\"record\"";
	CHAR Address[8] = "Addr_";
	CHAR Node[6] = "node ";
	CHAR FillBlackColor[21] = " fillcolor = \"black\"";
	CHAR FontColor[20] = " fontcolor=\"yellow\"";
	CHAR FontCourierName[20] = " fontname=\"courier\"";
	CHAR StyleTrue[16] = " style=\"filled\"";
};