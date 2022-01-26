#include "Exporter.h"

#include "ExportXml.h"
#include "Timeline.h"
#include "filesystem/Filesystem.h"

#include <string>

namespace ks {

void Exporter::saveProject(const char *projectDir)
{
	XmlExporter exporter;
	exporter.push(Timeline::get());

	std::string projFile = projectDir;
	projFile += "/";
	projFile += "demo.xml";

	writeToFile(exporter.getContent(), exporter.getContentSize(), projFile.c_str());
}

}
