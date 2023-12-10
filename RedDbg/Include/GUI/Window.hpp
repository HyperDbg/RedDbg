#pragma once
#include "GUI/WindowCodes.hpp"
#include "GUI/ClickedCodesOfTreeNodes.hpp"

#include <string_view>
#include <vector>
#include <any>

struct Callable {
	static int CallBackInputWrapper(void* Arg, void* Class);
	/* {
		ImGuiInputTextCallbackData* Data = reinterpret_cast<ImGuiInputTextCallbackData*>(Arg);
		int Status = static_cast<Window_*>(Class)->InputWrapper(Data);
		return Status;
	}*/
};

class Window_ {
private:
	void ChildWindowDispatcher(const WindowCodes Code, const std::any ObjOfClass);
public:
	bool SplitterVertical(const std::string_view id, int* posY, const int height, const ImVec2& posMin, const ImVec2& posMax, static int* startPosY);
	bool SplitterHorizontal(const std::string_view id, int* posX, const int width, const ImVec2& posMin, const ImVec2& posMax, static int* startPosX);

	void SelectAllAndColored(const unsigned char CountOfColumns, const uint32_t U32Color);

	bool ClickOnTreeNode(const bool Open, const ClickCodesOfTreeNodes_ Code, std::vector<bool>* Nodes);
	void ClickOnTreeNodeDispatcher(const ClickCodesOfTreeNodes_ Code, std::vector<bool>* Nodes);

	int InputWrapper(ImGuiInputTextCallbackData* Data);
public:
	//template <typename DispatcherClass>
	void ChildWindowRender(
		const std::string_view id, 
		const ImVec2& pos, 
		const ImVec2& size,
		const WindowCodes Code, 
		const std::any ObjOfClass,
		const bool border = false, 
		const ImGuiWindowFlags WindowFlags = 0);
};