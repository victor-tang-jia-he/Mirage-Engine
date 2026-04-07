#pragma once
#include <filesystem>

namespace Mirage
{
	enum class DragDropTag
	{
		HIERARCHY_ITEM,
		CONTENT_BROWSER_ITEM
	};

	class DragDrop
	{
	public:
	protected:
	private:

	public:
		static void SetPathPayload(DragDropTag _tag, const std::filesystem::path& _path);
		static void SetPointerPayload(DragDropTag _tag, void* _pointer);

		static  std::filesystem::path GetPathPayload(DragDropTag _tag);
		static void* GetPointerPayload(DragDropTag _tag);
	protected:
	private:
	};
}