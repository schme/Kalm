#pragma once

#include <tinyxml2.h>

namespace ks
{

struct Scene;
struct Timeline;
struct TimelineItem;
struct Shader;
struct Camera;
struct Model;

struct XmlExporter
{
	// TODO: FILE* constructor
	XmlExporter();

	void push(const Timeline& timeline);

	void push(const Scene& scene);
	void push(const Shader& shader);
	void push(const Camera& camera);
	void push(const Model& model);
	void push(const TimelineItem &item);

	tinyxml2::XMLPrinter printer;
};

}
