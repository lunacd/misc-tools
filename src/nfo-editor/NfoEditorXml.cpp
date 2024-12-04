#include <NfoEditorXml.hpp>

#include <sstream>

#include <fmt/format.h>
#include <pugixml.hpp>

namespace Lunacd::NfoEditor {
std::string Xml::exportToStr() const {
  pugi::xml_document doc;
  pugi::xml_node root = doc.append_child("movie");

  pugi::xml_node titleNode = root.append_child("title");
  titleNode.append_child(pugi::node_pcdata).set_value(title.c_str());

  pugi::xml_node studioNode = root.append_child("studio");
  studioNode.append_child(pugi::node_pcdata).set_value(studio.c_str());

  for (const auto &actor : actors) {
    pugi::xml_node actorNode = root.append_child("actor");
    pugi::xml_node actorNameNode = actorNode.append_child("name");
    actorNameNode.append_child(pugi::node_pcdata).set_value(actor.c_str());
  }

  for (const auto &tag : tags) {
    pugi::xml_node tagNode = root.append_child("tag");
    tagNode.append_child(pugi::node_pcdata).set_value(tag.c_str());
  }

  std::stringstream ss;
  doc.save(ss);
  return ss.str();
}
} // namespace Lunacd::NfoEditor