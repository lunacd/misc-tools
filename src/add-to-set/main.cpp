#include <filesystem>
#include <format>
#include <iostream>
#include <optional>
#include <string>
#include <vector>

#include <pugixml.hpp>

std::optional<std::filesystem::path>
find_poster_in_folder(const std::filesystem::path &folder) {
  if (!std::filesystem::exists(folder) ||
      !std::filesystem::is_directory(folder)) {
    return std::nullopt;
  }

  for (const auto &entry : std::filesystem::directory_iterator(folder)) {
    if (!entry.is_regular_file()) {
      continue;
    }

    std::string ext = entry.path().extension().string();
    for (char &c : ext) {
      c = static_cast<char>(std::tolower(static_cast<unsigned char>(c)));
    }

    if (ext == ".jpg" || ext == ".png") {
      return entry.path();
    }
  }

  return std::nullopt;
}

void add_to_set(const std::string &set_name,
                const std::vector<std::filesystem::path> &movies) {
  // 1: Create set folder
  std::filesystem::path set_path(std::format("{} [boxset]", set_name));
  std::filesystem::create_directory(set_path);

  // 2: Move all movies into a set folder
  std::filesystem::path first_movie;
  for (const auto &movie : movies) {
    // A trailing separator causes filename() to return empty; strip it.
    std::filesystem::path normalized =
        movie.filename().empty() ? movie.parent_path() : movie;
    std::filesystem::path destination_path = set_path / normalized.filename();
    std::cout << "Moving " << normalized << " to " << destination_path
              << std::endl;
    std::filesystem::rename(normalized, destination_path);

    if (first_movie.empty()) {
      first_movie = destination_path;
    }
  }

  // 3: Take the first movie's poster and use it for the set
  if (!first_movie.empty()) {
    std::cout << "Looking for poster in " << first_movie << std::endl;
    std::optional<std::filesystem::path> poster_path =
        find_poster_in_folder(first_movie);

    std::cout << "Using poster: "
              << (poster_path.has_value() ? poster_path->string() : "None")
              << std::endl;
    if (poster_path.has_value()) {
      std::filesystem::path target_poster =
          set_path / std::format("poster{}", poster_path->extension().string());
      std::filesystem::copy_file(
          *poster_path, target_poster,
          std::filesystem::copy_options::overwrite_existing);
    } else {
      std::cerr << "No .jpg or .png poster found in " << first_movie
                << std::endl;
    }
  } else {
    std::cerr << "No movies were moved to the set folder." << std::endl;
  }
}

int main(int argc, char *argv[]) {
  if (argc < 3) {
    std::cerr << "Usage: " << argv[0] << " <set_name> <movies...>" << std::endl;
    return 1;
  }

  std::string set_name = argv[1];
  std::vector<std::filesystem::path> movies;
  for (int i = 2; i < argc; i++) {
    movies.emplace_back(argv[i]);
  }

  try {
    add_to_set(set_name, movies);
  } catch (const std::exception &e) {
    std::cerr << "Error: " << e.what() << std::endl;
    return 1;
  }
}