/*






*/

#include <fstream>
#include <iostream>
#include <sstream>
#include <unistd.h>

#if USE_LIBOSRM
#include "osrm/exception.hpp"
#endif

#include "problems/vrp.h"
#include "structures/cl_args.h"
#include "structures/typedefs.h"
#include "structures/EfOVRP/input/input.h"
#include "utils/exception.h"
#include "utils/helpers.h"
#include "utils/input_parser.h"
#include "utils/output_json.h"

void display_usage() {
  std::string usage = "EfOVRP \n";
  //7777 usage += "Version: " + EfOVRP::get_version() + "\n";
  usage += "Usage:\n\tEfOVRP [OPTION]... \"INPUT\"";
  usage += "\n\tEfOVRP [OPTION]... -i FILE\n";
  usage += "Options:\n";
  usage += "\t-a PROFILE:HOST (=" + EfOVRP::DEFAULT_PROFILE +
           ":0.0.0.0)\t routing server\n";
  usage += "\t-g,\t\t\t\t add detailed route geometry and indicators\n";
  usage += "\t-i FILE,\t\t\t read input from FILE rather than from stdin\n";
  usage += "\t-o OUTPUT,\t\t\t output file name\n";
  usage += "\t-p PROFILE:PORT (=" + EfOVRP::DEFAULT_PROFILE +
           ":5000),\t routing server port\n";
  usage += "\t-r ROUTER (=osrm),\t\t osrm, libosrm or ors\n";
  usage += "\t-t THREADS (=4),\t\t number of threads to use\n";
  usage += "\t-x EXPLORE (=5),\t\t exploration level to use (0..5)";
  std::cout << usage << std::endl;
  exit(0);
}

int main(int argc, char** argv) {
  // Load default command-line options.
  EfOVRP::io::CLArgs cl_args;

  // Parsing command-line arguments.
  const char* optString = "a:e:gi:o:p:r:t:x:h?";
  int opt = getopt(argc, argv, optString);

  std::string router_arg;
  std::string nb_threads_arg = std::to_string(cl_args.nb_threads);
  std::string exploration_level_arg = std::to_string(cl_args.exploration_level);
  std::vector<std::string> heuristic_params_arg;

  while (opt != -1) {
    switch (opt) {
    case 'a':
      EfOVRP::io::update_host(cl_args.servers, optarg);
      break;
    case 'e':
      heuristic_params_arg.push_back(optarg);
      break;
    case 'g':
      cl_args.geometry = true;
      break;
    case 'h':
      display_usage();
      break;
    case 'i':
      cl_args.input_file = optarg;
      break;
    case 'o':
      cl_args.output_file = optarg;
      break;
    case 'p':
      EfOVRP::io::update_port(cl_args.servers, optarg);
      break;
    case 'r':
      router_arg = optarg;
      break;
    case 't':
      nb_threads_arg = optarg;
      break;
    case 'x':
      exploration_level_arg = optarg;
      break;
    default:
      break;
    }
    opt = getopt(argc, argv, optString);
  }

  try {
    // Needs to be done after previous switch to make sure the
    // appropriate output file is set.
    cl_args.nb_threads = std::stoul(nb_threads_arg);
    cl_args.exploration_level = std::stoul(exploration_level_arg);

    cl_args.exploration_level =
      std::min(cl_args.exploration_level, cl_args.max_exploration_level);
  } catch (const std::exception& e) {
    auto error_code = EfOVRP::utils::get_code(EfOVRP::ERROR::INPUT);
    std::string message = "Invalid numerical value in option.";
    std::cerr << "[Error] " << message << std::endl;
    EfOVRP::io::write_to_json({error_code, message}, false, cl_args.output_file);
    exit(error_code);
  }

  // Determine routing engine (defaults to ROUTER::OSRM).
  if (router_arg == "libosrm") {
    cl_args.router = EfOVRP::ROUTER::LIBOSRM;
  } else if (router_arg == "ors") {
    cl_args.router = EfOVRP::ROUTER::ORS;
  } else if (!router_arg.empty() and router_arg != "osrm") {
    auto error_code = EfOVRP::utils::get_code(EfOVRP::ERROR::INPUT);
    std::string message = "Invalid routing engine: " + router_arg + ".";
    std::cerr << "[Error] " << message << std::endl;
    EfOVRP::io::write_to_json({error_code, message}, false, cl_args.output_file);
    exit(error_code);
  }

  try {
    // Force heuristic parameters from the command-line, useful for
    // debugging.
    std::transform(heuristic_params_arg.begin(),
                   heuristic_params_arg.end(),
                   std::back_inserter(cl_args.h_params),
                   [](const auto& str_param) {
                     return EfOVRP::utils::str_to_heuristic_param(str_param);
                   });
  } catch (const EfOVRP::Exception& e) {
    auto error_code = EfOVRP::utils::get_code(e.error);
    std::cerr << "[Error] " << e.message << std::endl;
    EfOVRP::io::write_to_json({error_code, e.message},
                             false,
                             cl_args.output_file);
    exit(error_code);
  }

  // Add default server if none provided in input.
  if (cl_args.servers.empty()) {
    cl_args.servers.emplace(EfOVRP::DEFAULT_PROFILE, EfOVRP::Server());
  }

  if (cl_args.input_file.empty()) {
    // Getting input from command-line.
    if (argc == optind) {
      // Missing argument!
      display_usage();
    }
    cl_args.input = argv[optind];
  } else {
    // Getting input from provided file.
    std::ifstream ifs(cl_args.input_file);
    std::stringstream buffer;
    buffer << ifs.rdbuf();
    cl_args.input = buffer.str();
  }

  try {
    // Build problem.
    EfOVRP::Input problem_instance = EfOVRP::io::parse(cl_args);

    EfOVRP::Solution sol = problem_instance.solve(cl_args.exploration_level,
                                                 cl_args.nb_threads,
                                                 cl_args.h_params);

    // Write solution.
    EfOVRP::io::write_to_json(sol, cl_args.geometry, cl_args.output_file);
  } catch (const EfOVRP::Exception& e) {
    auto error_code = EfOVRP::utils::get_code(e.error);
    std::cerr << "[Error] " << e.message << std::endl;
    EfOVRP::io::write_to_json({error_code, e.message},
                             false,
                             cl_args.output_file);
    exit(error_code);
  }
#if USE_LIBOSRM
  catch (const osrm::exception& e) {
    // In case of an unhandled routing error.
    auto error_code = EfOVRP::utils::get_code(EfOVRP::ERROR::ROUTING);
    auto message = "Routing problem: " + std::string(e.what());
    std::cerr << "[Error] " << message << std::endl;
    EfOVRP::io::write_to_json({error_code, message}, false, cl_args.output_file);
    exit(error_code);
  }
#endif
  catch (const std::exception& e) {
    // In case of an unhandled internal error.
    auto error_code = EfOVRP::utils::get_code(EfOVRP::ERROR::INTERNAL);
    std::cerr << "[Error] " << e.what() << std::endl;
    EfOVRP::io::write_to_json({error_code, e.what()},
                             false,
                             cl_args.output_file);
    exit(error_code);
  }

  return 0;
}
