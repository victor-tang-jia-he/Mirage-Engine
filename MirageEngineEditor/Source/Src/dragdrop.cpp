#include "dragdrop.h"
#include "imgui.h"
#include <string>

#include <Logs/log.h>

DEFINE_LOG_CATEGORY_WITH_COLOR(LogDragDrop, Loggers::Color::CYAN);

namespace Mirage
{
	static const char* GetTagString(DragDropTag _tag)
	{
		switch (_tag)
		{
			case DragDropTag::CONTENT_BROWSER_ITEM:
				LOG(logTemp, LogDragDrop, "Returning tag string for CONTENT_BROWSER_ITEM");
				return "CONTENT_BROWSER_ITEM";
			case DragDropTag::HIERARCHY_ITEM:
				LOG(logTemp, LogDragDrop, "Returning tag string for HIERARCHY_ITEM");
				return "HIERARCHY_ITEM";
			default:
				return "UNKNOWN";
		}
	}

	void DragDrop::SetPathPayload(Mirage::DragDropTag _tag, const std::filesystem::path& _path)
	{
		const std::string pathstr = _path.string();
		ImGui::SetDragDropPayload(GetTagString(_tag), pathstr.c_str(), pathstr.size()+1);
	}

	void DragDrop::SetPointerPayload(Mirage::DragDropTag _tag, void* _pointer)
	{
		ImGui::SetDragDropPayload(GetTagString(_tag), &_pointer, sizeof(void*));
	}

	std::filesystem::path DragDrop::GetPathPayload(Mirage::DragDropTag _tag)
	{
		if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(GetTagString(_tag)))
			return std::filesystem::path((const char*)payload->Data);

		return {};
	}

	void* DragDrop::GetPointerPayload(Mirage::DragDropTag _tag)
	{
		if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(GetTagString(_tag)))
			return *(void**)payload->Data;

		return nullptr;
	}
}