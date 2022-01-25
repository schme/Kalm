#include "ExportXml.h"

#include "Scene.h"
#include "Timeline.h"

namespace ks
{

XmlExporter::XmlExporter()
	: printer(tinyxml2::XMLPrinter())
{
}


void XmlExporter::push(const Scene& scene)
{
	printer.OpenElement("Scene");

	printer.PushAttribute("name", scene.name.c_str());

	printer.CloseElement();
}

void XmlExporter::push(const Timeline& timeline)
{
	printer.OpenElement("TIMELINE");
	printer.PushAttribute("FrameMax", timeline.frameMax);
	for (const TimelineItem &item : timeline.items)	{
		push(item);
	}
	printer.CloseElement();
}

void XmlExporter::push(const TimelineItem& item)
{
	printer.OpenElement("Item");

	printer.PushAttribute("Id", toStr(item.id));
	printer.PushAttribute("Start", item.frameStart);
	printer.PushAttribute("End", item.frameEnd);
	printer.PushAttribute("Type", static_cast<int>(item.type));

	printer.CloseElement();
}

void XmlExporter::push(const Shader& shader)
{
}

void XmlExporter::push(const Camera& camera)
{
}

void XmlExporter::push(const Model& model)
{
}

}
