#pragma once
#include "GUI/WindowCodes.hpp"
#include "GUI/ClickedCodesOfTreeNodes.hpp"

#include <string_view>
#include <vector>
#include <unordered_map>
#include <string>
#include <any>

struct Callable {
	static int CallBackInputWrapper(void* Arg, void* Class);
};

typedef enum {
	TableMenu,
	WindowMenu
} ClickMenuType_;

class Window_ {
private:
	void ChildWindowDispatcher(const WindowCodes Code, const std::any ObjOfClass);
public:
	class ClickMenu_
	{
	private:
		uint64_t IdHash;
	public:
		template<typename TypeIndex>
		void MenuInit(std::string_view id, ImGuiMouseButton button, TypeIndex CurrentIndexString, ClickMenuType_ MenuType)
		{
			if (ImGui::IsItemHovered())
			{
				IdHash = std::hash<std::string_view>{}(id);
				if (ImGui::IsMouseClicked(button))
				{
					ImGui::OpenPopup(std::to_string(IdHash).c_str());
				}
			}
		}

		template<typename Callback>
			requires std::is_invocable_r_v<void, Callback>
		void MenuCall(Callback&& callback)
		{
			if (ImGui::BeginPopup(std::to_string(IdHash).c_str()))
			{
				std::invoke(std::forward<Callback>(callback));
				ImGui::EndPopup();
			}
		}
	};
public:
	void CheckBoxOpened(std::vector<bool>* Openeds, bool Opened, const unsigned char Index);

	bool SplitterVertical(const std::string_view id, int* posY, const int height, const ImVec2& posMin, const ImVec2& posMax, static int* startPosY);
	bool SplitterHorizontal(const std::string_view id, int* posX, const int width, const ImVec2& posMin, const ImVec2& posMax, static int* startPosX);

	void SelectAllAndColored(const unsigned char CountOfColumns, const uint32_t U32Color);

	bool ClickOnTreeNode(const bool Open, const ClickCodesOfTreeNodes_ Code, std::vector<bool>* Nodes);
	void ClickOnTreeNodeDispatcher(const ClickCodesOfTreeNodes_ Code, std::vector<bool>* Nodes);

	int InputWrapper(ImGuiInputTextCallbackData* Data);
public:
	void ChildWindowRender(
		const std::string_view id, 
		const ImVec2& pos, 
		const ImVec2& size,
		const WindowCodes Code, 
		const std::any ObjOfClass,
		const bool border = false, 
		const ImGuiWindowFlags WindowFlags = 0);
};