#include "TimelineGui.h"
#include "ResourceStorage.h"
#include "Scene.h"
#include "Timeline.h"
#include "imgui.h"
#include "imgui_internal.h"

namespace ks {

namespace style {
const ImColor sceneItemColor = 0xFFAA00AA;
const ImColor postItemColor = 0xFFFFFF00;
const ImColor unknownColor = 0x0F0F0FFF;
}

static ImColor getItemColorByType(TimelineItem::Type type) {
	using Type = TimelineItem::Type;

	switch(type) {
		case Type::Scene:
			return style::sceneItemColor;
		case Type::Post:
			return style::postItemColor;
		default:
			return style::unknownColor;
	}
}

void TimelineGui::init(Timeline *timeline)
{
	this->timeline = timeline;

	sequenceOptions =
		ImSequencer::SEQUENCER_OPTIONS::SEQUENCER_EDIT_ALL
		| ImSequencer::SEQUENCER_OPTIONS::SEQUENCER_ADD
		| ImSequencer::SEQUENCER_OPTIONS::SEQUENCER_DEL
		| ImSequencer::SEQUENCER_OPTIONS::SEQUENCER_COPYPASTE
	;
}

int TimelineGui::GetFrameMin() const
{
	return timeline->frameMin;
}
int TimelineGui::GetFrameMax() const
{
	return timeline->frameMax;
}
int TimelineGui::GetItemCount() const
{
	return timeline->items.size();
}

int TimelineGui::GetItemTypeCount() const 
{
	return static_cast<int>(TimelineItem::Type::Count);
}

const char* TimelineGui::GetItemTypeName(int typeIndex) const
{
	using Type = TimelineItem::Type;
	Type type = Type(typeIndex);

	switch(type) {
		case Type::Scene:
			return "Scene";
		case Type::Post:
			return "Post";
		case Type::Framebuffer:
			return "Framebuffer";
		default:
			return "";
	}

	return "";
}

const char* TimelineGui::GetItemLabel(int index) const 
{
	return timeline->items[index].id.c_str();
}

void TimelineGui::Add(int typeIndex)
{
	TimelineItem::Type type = TimelineItem::Type(typeIndex);
	if (type == TimelineItem::Type::Scene) {
		// TODO: proper handling
		auto& storage = ResourceStorage<Scene>::get().storage;
		for (const auto& [key, scene] : storage) {
			bool sceneInTimeline = false;
			for (const auto& item : timeline->items) {
				if (item.type == type && item.id == key) {
					sceneInTimeline = true;
					break;
				}
			}

			if (!sceneInTimeline) {
				timeline->items.emplace_back(TimelineItem{ResourceId(key), 0, 120, type});
			}
		}
	}
	else {
	}
}

void TimelineGui::Del(int index)
{
	timeline->items.erase(timeline->items.begin() + index);
}

void TimelineGui::Get(int index, int** start, int** end, int* type, unsigned int* color)
{
	TimelineItem &item = timeline->items[index];
	if (color)
		*color = getItemColorByType(item.type);
	if (start)
		*start = &item.frameStart;
	if (end)
		*end = &item.frameEnd;
	if (type)
		*type = static_cast<int>(item.type);
}

#if 0
void TimelineGui::CustomDraw(int index, ImDrawList* drawList, const ImRect& rc, const ImRect& legendRect, const ImRect& clippingRect, const ImRect& legendClippingRect)
{
  auto mMax = ImVec2(float(frameMax), 1.f);
  auto mMin = ImVec2(float(frameMin), 0.f);

  drawList->PushClipRect(legendClippingRect.Min, legendClippingRect.Max, true);

  static const char* labels[] = { "Translation", "Rotation" , "Scale" };

  for (int i = 0; i < 3; i++)
  {
	 ImVec2 pta(legendRect.Min.x + 30, legendRect.Min.y + i * 14.f);
	 ImVec2 ptb(legendRect.Max.x, legendRect.Min.y + (i + 1) * 14.f);
	 drawList->AddText(pta, 0xFFFFFFFF, labels[i]);
  }
  drawList->PopClipRect();
}

void TimelineGui::CustomDrawCompact(int index, ImDrawList* drawList, const ImRect& rc, const ImRect& clippingRect)
{
}

#endif
}
