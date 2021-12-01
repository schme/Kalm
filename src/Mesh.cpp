#include "Mesh.h"

namespace ks {

#define CASE(x) case x: return #x;
const char *toStr(BufferType type)
{
    switch (type)
    {
        CASE(BufferType::Vertex);
        CASE(BufferType::Color);
        CASE(BufferType::Normal);
        CASE(BufferType::Texcoord0);
        CASE(BufferType::Texcoord1);
        CASE(BufferType::Texcoord2);
        default: return "Unnamed buffer type";
    }
    return "Unknown";
}
#undef CASE

std::string toStr(const Mesh &mesh)
{
    std::string out;
    out.reserve(256);
    out.clear();

    out += "\nname: "; out += mesh.name;
    out += "\nstride: "; out += mesh.descriptor.stride;
    out += "\nBuffers: ";
    for(const auto& desc : mesh.descriptor.buffers)
    {
        out += toStr(desc.type);
        out += " offset: ";
    }

    return out;
}

}
