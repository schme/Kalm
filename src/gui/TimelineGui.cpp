#include "TimelineGui.h"
#include "imgui.h"
#include "imgui_internal.h"

namespace ks {

namespace style {
const ImColor sceneItemColor = 0xFFFF00FF;
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

int TimelineGui::GetFrameMin() const
{
	return frameMin;
}
int TimelineGui::GetFrameMax() const
{
	return frameMax;
}
int TimelineGui::GetItemCount() const
{
	return items.size();
}

void TimelineGui::Get(int index, int** start, int** end, int* type, unsigned int* color)
{
	TimelineItem &item = items[index];
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
