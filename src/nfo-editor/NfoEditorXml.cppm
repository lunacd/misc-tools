module;

#include <fmt/format.h>
#include <pugixml.hpp>

export module lunacd.nfoEditor:xml;

import std;

namespace NfoEditor {
export class NfoData {
public:
  NfoData(std::string title, std::string studio,
          std::vector<std::string> actors, std::vector<std::string> tags)
      : title(std::move(title)), studio(std::move(studio)),
        actors(std::move(actors)), tags(std::move(tags)) {}

  std::string title;
  std::string studio;
  std::vector<std::string> actors{};
  std::vector<std::string> tags{};

  void saveToFile(const std::filesystem::path &path) const {
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

    // If the given path has no extension, give it .nfo extension
    // If the given path has another extension, replace it with .nfo
    // This is useful for generating nfo files for another existing file.
    auto pathToSave = path;
    pathToSave.replace_extension("nfo");

    if (!doc.save_file(pathToSave.c_str())) {
      throw std::runtime_error("Could not save file");
    }
  }
};
} // namespace NfoEditor