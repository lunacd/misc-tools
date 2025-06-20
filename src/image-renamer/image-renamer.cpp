#include <TinyEXIF.h>

#include <chrono>
#include <filesystem>
#include <format>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

int main(int argc, char *argv[]) {
  if (argc != 2) {
    std::cout << "Usage: image-renamer <image_directory>\n";
    std::cout << "This will rename all images in the specified directory based "
                 "on their EXIF metadata.\n";
    std::cout << "Renamed files will be in the format: dd-mm-yy hh-mm-ss "
                 "<original_file_name>.<extension>\n";
    return -1;
  }

  std::filesystem::path imagePath(argv[1]);

  for (const auto &entry : std::filesystem::directory_iterator(imagePath)) {
    if (!std::filesystem::is_regular_file(entry)) {
      continue;
    }
    // Only process jpeg files
    if (entry.path().extension() != ".jpg" &&
        entry.path().extension() != ".jpeg") {
      continue;
    }

    // Get EXIF metadata
    std::chrono::sys_time<std::chrono::seconds> originalDateTime;
    {
      std::ifstream istream(entry.path(), std::ifstream::binary);
      istream.seekg(0, std::ios::end);
      std::streampos length = istream.tellg();
      istream.seekg(0, std::ios::beg);
      std::vector<uint8_t> data(length);
      istream.read(reinterpret_cast<char *>(data.data()), length);
      TinyEXIF::EXIFInfo imageEXIF(data.data(), length);
      const auto dateTime = imageEXIF.DateTimeOriginal;

      // Parse EXIF datetime
      std::stringstream dateTimeStream{dateTime};
      dateTimeStream >>
          std::chrono::parse("%Y:%m:%d %H:%M:%S", originalDateTime);
    }

    // Construct new filename
    std::time_t originalTimeT =
        std::chrono::system_clock::to_time_t(originalDateTime);
    std::stringstream formattedOriginalTime;
    formattedOriginalTime << std::put_time(std::localtime(&originalTimeT),
                                           "%y-%m-%d %H-%M-%S");

    // Check if file needs renaming
    if (entry.path().filename().string().starts_with(
            formattedOriginalTime.str())) {
      std::cout << "Skipping: " << entry.path() << " (already renamed)\n";
      continue;
    }

    const auto newFilename = std::format("{} {}", formattedOriginalTime.str(),
                                         entry.path().filename().string());

    // Rename file
    std::cout << "Renaming: " << entry.path() << " to "
              << entry.path().parent_path() / newFilename << "\n";
    std::filesystem::rename(entry.path(),
                            entry.path().parent_path() / newFilename);
  }

  return 0;
}