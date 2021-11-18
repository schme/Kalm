#include "TimelineGui.h"
#include "imgui.h"
#include "imgui_internal.h"

namespace ks {

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
		*color = 0xFFAA8080;
	if (start)
		*start = &item.frameStart;
	if (end)
		*end = &item.frameEnd;
	if (type)
		*type = item.type;
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
