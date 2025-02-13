#ifndef LOG_COMPARER_H
#define LOG_COMPARER_H

#include <cstdlib>
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

  // Set prefixes to ignore
  void setIgnoredPrefixes(const std::vector<std::string>& prefixes)
  {
    ignore_prefixes = prefixes;
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
  std::vector<std::string> ignore_prefixes;

  // Build the dynamic grep pattern for ignored prefixes
  auto buildGrepPattern() const -> std::string
  {
    if (ignore_prefixes.empty())
      return "";  // No filtering needed

    std::ostringstream grep_pattern;
    grep_pattern << "'^(";
    for (size_t i = 0; i < ignore_prefixes.size(); ++i) {
      if (i > 0)
        grep_pattern << "|";
      grep_pattern << ignore_prefixes[i];
    }
    grep_pattern << ")'";

    return "| grep -Ev " + grep_pattern.str();
  }

  // Build the full diff command with filtering
  auto buildDiffCommand() const -> std::string
  {
    std::string grep_filter = buildGrepPattern();

    std::cout<<"Comparing "<<ref_log_file<<" and "<<new_log_file<<std::endl;
    std::cout<<"Grep filter: "<<grep_filter<<std::endl;

    // Compare the log files, but ignore any
    // memory addresses (0x[0-9a-f]+ → 0xADDR)
    // and timing information (0-9]+\.[0-9]+ s → XX.XX s)
    return "diff -u <(sed -E 's/0x[0-9a-f]+/0xADDR/g; s/[0-9]+\\.[0-9]+ "
           "s/XX.XX s/g' "
        + ref_log_file + " " + grep_filter + ") "
        + "<(sed -E 's/0x[0-9a-f]+/0xADDR/g; s/[0-9]+\\.[0-9]+ s/XX.XX s/g' "
        + new_log_file + " " + grep_filter + ")";
  }
};

#endif  // LOG_COMPARER_H
