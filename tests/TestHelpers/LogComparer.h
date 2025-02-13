#ifndef LOG_COMPARER_H
#define LOG_COMPARER_H

#include <cstdlib>
#include <iostream>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

class LogComparer
{
public:
  // Constructor
  LogComparer(std::string ref_log, std::string new_log)
      : ref_log_file(std::move(ref_log))
      , new_log_file(std::move(new_log))
  {
  }

  // Supply any regex patterns to ignore lines that match those patterns
  void setIgnorePatterns(const std::vector<std::string>& patterns)
  {
    ignore_patterns = patterns;
  }

  // Run the diff with filtering
  auto compareLogs() const -> bool
  {
    const std::string diff_cmd = buildDiffCommand();
    int diff_status = system(diff_cmd.c_str());
    return diff_status == 0;  // Return true if logs match, false otherwise
  }

  // Get the generated diff command (for debugging/logging)
  auto getDiffCommand() const -> std::string { return buildDiffCommand(); }

private:
  const std::string ref_log_file;
  const std::string new_log_file;
  std::vector<std::string> ignore_patterns;

  // Build the dynamic grep pattern that ignores lines matching any of the
  // regexes
  auto buildGrepPattern() const -> std::string
  {
    // If no patterns, no filtering
    if (ignore_patterns.empty()) {
      return "";
    }

    // Build something like:  grep -Ev '(PAT1|PAT2|...)'
    std::ostringstream pattern_stream;
    pattern_stream << "'(";
    for (size_t i = 0; i < ignore_patterns.size(); ++i) {
      if (i > 0) {
        pattern_stream << "|";
      }
      pattern_stream << ignore_patterns[i];
    }
    pattern_stream << ")'";

    // Return the pipeline to grep out (-v) lines matching any pattern
    return "| grep -Ev " + pattern_stream.str();
  }

  // Build the full diff command with filtering
  auto buildDiffCommand() const -> std::string
  {
    // Make the grep pipeline
    std::string grep_filter = buildGrepPattern();

    std::cout << "Comparing " << ref_log_file << " and " << new_log_file
              << std::endl;
    std::cout << "Grep filter: " << grep_filter << std::endl;

    // We'll replace memory addresses (0x[0-9a-f]+ → 0xADDR)
    // and timing info ([0-9]+\.[0-9]+ s → XX.XX s).
    // Then pass both files through the same sed+grep pipeline
    // and compare them with a unified diff (-u).
    std::ostringstream cmd;
    cmd << "diff -u "
        << "<(sed -E 's/0x[0-9a-f]+/0xADDR/g; s/[0-9]+\\.[0-9]+ s/XX.XX s/g' "
        << ref_log_file << " " << grep_filter << ") "
        << "<(sed -E 's/0x[0-9a-f]+/0xADDR/g; s/[0-9]+\\.[0-9]+ s/XX.XX s/g' "
        << new_log_file << " " << grep_filter << ")";

    return cmd.str();
  }
};

#endif  // LOG_COMPARER_H
