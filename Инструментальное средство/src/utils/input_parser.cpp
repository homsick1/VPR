#include <array>
#include <vector>

#if USE_LIBOSRM
#include "osrm/exception.hpp"
#endif

#include "../include/rapidjson/document.h"
#include "../include/rapidjson/error/en.h"

#if USE_LIBOSRM
// #include "routing/libosrm_wrapper.h" homsick
#endif
// #include "routing/ors_wrapper.h" homsick
#include "routing/osrm_routed_wrapper.h"
#include "structures/cl_args.h"
#include "structures/generic/matrix.h"
#include "structures/typedefs.h"
#include "structures/EfOVRP/amount.h"
#include "structures/EfOVRP/job.h"
#include "structures/EfOVRP/time_window.h"
#include "structures/EfOVRP/vehicle.h"
#include "utils/exception.h"
#include "utils/input_parser.h"

namespace EfOVRP {
namespace io {

// Helper to get optional array of coordinates.
inline Coordinates parse_coordinates(const rapidjson::Value& object,
                                     const char* key) {
  if (!object[key].IsArray() or (object[key].Size() < 2) or
      !object[key][0].IsNumber() or !object[key][1].IsNumber()) {
    throw Exception(ERROR::INPUT, "Invalid " + std::string(key) + " array.");
  }
  return {{object[key][0].GetDouble(), object[key][1].GetDouble()}};
}

inline std::string get_string(const rapidjson::Value& object, const char* key) {
  assert(object.HasMember(key));
  return object[key].GetString();
}

inline unsigned get_amount_size(const rapidjson::Value& json_input) {
  unsigned amount_size_candidate = 0;

  // Only return the first non-zero capacity size as amount size
  // consistency is further checked upon jobs/vehicles addition in
  // Input.
  for (rapidjson::SizeType i = 0; i < json_input["vehicles"].Size(); ++i) {
    auto& json_vehicle = json_input["vehicles"][i];
    if (json_vehicle.HasMember("capacity") and
        json_vehicle["capacity"].IsArray() and
        json_vehicle["capacity"].Size() > 0) {
      amount_size_candidate = json_vehicle["capacity"].Size();
      break;
    }
  }

  return amount_size_candidate;
}

inline Amount get_amount(const rapidjson::Value& object,
                         const char* key,
                         unsigned size) {
  // Default to zero amount with provided size.
  Amount amount(size);

  if (object.HasMember(key)) {
    if (!object[key].IsArray()) {
      throw Exception(ERROR::INPUT, "Invalid " + std::string(key) + " array.");
    }

    if (object[key].Size() != size) {
      throw Exception(ERROR::INPUT,
                      "Inconsistent " + std::string(key) +
                        " length: " + std::to_string(object[key].Size()) +
                        " and " + std::to_string(size) + '.');
    }

    for (rapidjson::SizeType i = 0; i < object[key].Size(); ++i) {
      if (!object[key][i].IsUint()) {
        throw Exception(ERROR::INPUT,
                        "Invalid " + std::string(key) + " value.");
      }
      amount[i] = object[key][i].GetUint();
    }
  }

  return amount;
}

inline Skills get_skills(const rapidjson::Value& object) {
  Skills skills;
  if (object.HasMember("skills")) {
    if (!object["skills"].IsArray()) {
      throw Exception(ERROR::INPUT, "Invalid skills object.");
    }
    for (rapidjson::SizeType i = 0; i < object["skills"].Size(); ++i) {
      if (!object["skills"][i].IsUint()) {
        throw Exception(ERROR::INPUT, "Invalid skill value.");
      }
      skills.insert(object["skills"][i].GetUint());
    }
  }

  return skills;
}

inline Duration get_service(const rapidjson::Value& object) {
  Duration service = 0;
  if (object.HasMember("service")) {
    if (!object["service"].IsUint()) {
      throw Exception(ERROR::INPUT, "Invalid service value.");
    }
    service = object["service"].GetUint();
  }
  return service;
}

inline Duration get_priority(const rapidjson::Value& object) {
  Priority priority = 0;
  if (object.HasMember("priority")) {
    if (!object["priority"].IsUint()) {
      throw Exception(ERROR::INPUT, "Invalid priority value.");
    }
    priority = object["priority"].GetUint();
    if (priority > MAX_PRIORITY) {
      throw Exception(ERROR::INPUT, "Invalid priority value.");
    }
  }
  return priority;
}

inline void check_id(const rapidjson::Value& v, const std::string& type) {
  if (!v.IsObject()) {
    throw Exception(ERROR::INPUT, "Invalid " + type + ".");
  }
  if (!v.HasMember("id") or !v["id"].IsUint64()) {
    throw Exception(ERROR::INPUT, "Invalid or missing id for " + type + ".");
  }
}

inline void check_shipment(const rapidjson::Value& v) {
  if (!v.IsObject()) {
    throw Exception(ERROR::INPUT, "Invalid shipment.");
  }
  if (!v.HasMember("pickup") or !v["pickup"].IsObject()) {
    throw Exception(ERROR::INPUT, "Missing pickup for shipment.");
  }
  if (!v.HasMember("delivery") or !v["delivery"].IsObject()) {
    throw Exception(ERROR::INPUT, "Missing delivery for shipment.");
  }
}

inline void check_location_index(const rapidjson::Value& v,
                                 const std::string& type,
                                 rapidjson::SizeType matrix_size) {
  if (!v.HasMember("location_index") or !v["location_index"].IsUint()) {
    throw Exception(ERROR::INPUT,
                    "Invalid location_index for " + type + " " +
                      std::to_string(v["id"].GetUint64()) + ".");
  }
  if (matrix_size <= v["location_index"].GetUint()) {
    throw Exception(ERROR::INPUT,
                    "location_index exceeding matrix size for " + type + " " +
                      std::to_string(v["id"].GetUint64()) + ".");
  }
}

inline void check_location(const rapidjson::Value& v, const std::string& type) {
  if (!v.HasMember("location") or !v["location"].IsArray()) {
    throw Exception(ERROR::INPUT,
                    "Invalid location for " + type + " " +
                      std::to_string(v["id"].GetUint64()) + ".");
  }
}

inline TimeWindow get_time_window(const rapidjson::Value& tw) {
  if (!tw.IsArray() or tw.Size() < 2 or !tw[0].IsUint() or !tw[1].IsUint()) {
    throw Exception(ERROR::INPUT, "Invalid time-window.");
  }
  return TimeWindow(tw[0].GetUint(), tw[1].GetUint());
}

inline TimeWindow get_vehicle_time_window(const rapidjson::Value& v) {
  TimeWindow v_tw = TimeWindow();
  if (v.HasMember("time_window")) {
    v_tw = get_time_window(v["time_window"]);
  }
  return v_tw;
}

inline std::vector<TimeWindow> get_job_time_windows(const rapidjson::Value& j) {
  std::vector<TimeWindow> tws;
  if (j.HasMember("time_windows")) {
    if (!j["time_windows"].IsArray() or j["time_windows"].Empty()) {
      throw Exception(ERROR::INPUT,
                      "Invalid time_windows array for job " +
                        std::to_string(j["id"].GetUint64()) + ".");
    }

    std::transform(j["time_windows"].Begin(),
                   j["time_windows"].End(),
                   std::back_inserter(tws),
                   [](auto& tw) { return get_time_window(tw); });

    std::sort(tws.begin(), tws.end());
  } else {
    tws = std::vector<TimeWindow>(1, TimeWindow());
  }

  return tws;
}

Input parse(const CLArgs& cl_args) {
  // Input json object.
  rapidjson::Document json_input;

  // Parsing input string to populate the input object.
  if (json_input.Parse(cl_args.input.c_str()).HasParseError()) {
    std::string error_msg =
      std::string(rapidjson::GetParseError_En(json_input.GetParseError())) +
      " (offset: " + std::to_string(json_input.GetErrorOffset()) + ")";
    throw Exception(ERROR::INPUT, error_msg);
  }

  // Main checks for valid json input.
  bool has_jobs = json_input.HasMember("jobs") and
                  json_input["jobs"].IsArray() and !json_input["jobs"].Empty();
  bool has_shipments = json_input.HasMember("shipments") and
                       json_input["shipments"].IsArray() and
                       !json_input["shipments"].Empty();
  if (!has_jobs and !has_shipments) {
    throw Exception(ERROR::INPUT, "Invalid jobs or shipments.");
  }

  if (!json_input.HasMember("vehicles") or !json_input["vehicles"].IsArray() or
      json_input["vehicles"].Empty()) {
    throw Exception(ERROR::INPUT, "Invalid vehicles.");
  }

  // Used to make sure all vehicles have the same profile.
  std::string common_profile;

  // Custom input object embedding jobs, vehicles and matrix.
  auto amount_size = get_amount_size(json_input);
  Input input(amount_size);
  input.set_geometry(cl_args.geometry);

  // Switch input type: explicit matrix or using OSRM.
  if (json_input.HasMember("matrix")) {
    if (!json_input["matrix"].IsArray()) {
      throw Exception(ERROR::INPUT, "Invalid matrix.");
    }

    // Load custom matrix while checking if it is square.
    rapidjson::SizeType matrix_size = json_input["matrix"].Size();

    Matrix<Cost> matrix_input(matrix_size);
    for (rapidjson::SizeType i = 0; i < matrix_size; ++i) {
      if (!json_input["matrix"][i].IsArray() or
          (json_input["matrix"][i].Size() != matrix_size)) {
        throw Exception(ERROR::INPUT,
                        "Invalid matrix line " + std::to_string(i) + ".");
      }
      rapidjson::Document::Array mi = json_input["matrix"][i].GetArray();
      for (rapidjson::SizeType j = 0; j < matrix_size; ++j) {
        if (!mi[j].IsUint()) {
          throw Exception(ERROR::INPUT,
                          "Invalid matrix entry (" + std::to_string(i) + "," +
                            std::to_string(j) + ").");
        }
        Cost cost = mi[j].GetUint();
        matrix_input[i][j] = cost;
      }
    }
    input.set_matrix(std::move(matrix_input));

    // Add all vehicles.
    for (rapidjson::SizeType i = 0; i < json_input["vehicles"].Size(); ++i) {
      auto& json_vehicle = json_input["vehicles"][i];
      check_id(json_vehicle, "vehicle");
      auto v_id = json_vehicle["id"].GetUint64();

      // Check if vehicle has start_index or end_index.
      bool has_start_index = json_vehicle.HasMember("start_index");
      Index start_index = 0; // Initial value actually never used.
      if (has_start_index) {
        if (!json_vehicle["start_index"].IsUint()) {
          throw Exception(ERROR::INPUT,
                          "Invalid start_index for vehicle " +
                            std::to_string(v_id) + ".");
        }
        start_index = json_vehicle["start_index"].GetUint();

        if (matrix_size <= start_index) {
          throw Exception(ERROR::INPUT,
                          "start_index exceeding matrix size for vehicle" +
                            std::to_string(v_id) + ".");
        }
      }

      bool has_start_coords = json_vehicle.HasMember("start");

      bool has_end_index = json_vehicle.HasMember("end_index");
      Index end_index = 0; // Initial value actually never used.
      if (has_end_index) {
        if (!json_vehicle["end_index"].IsUint()) {
          throw Exception(ERROR::INPUT,
                          "Invalid end_index for vehicle" +
                            std::to_string(v_id) + ".");
        }
        end_index = json_vehicle["end_index"].GetUint();

        if (matrix_size <= end_index) {
          throw Exception(ERROR::INPUT,
                          "end_index exceeding matrix size for vehicle" +
                            std::to_string(v_id) + ".");
        }
      }

      bool has_end_coords = json_vehicle.HasMember("end");

      // Add vehicle to input
      boost::optional<Location> start;
      if (has_start_index) {
        if (has_start_coords) {
          start = boost::optional<Location>(
            Location({start_index, parse_coordinates(json_vehicle, "start")}));
        } else {
          start = boost::optional<Location>(start_index);
        }
      }

      boost::optional<Location> end;
      if (has_end_index) {
        if (has_end_coords) {
          end = boost::optional<Location>(
            Location({end_index, parse_coordinates(json_vehicle, "end")}));
        } else {
          end = boost::optional<Location>(end_index);
        }
      }

      Vehicle vehicle(v_id,
                      start,
                      end,
                      get_amount(json_vehicle, "capacity", amount_size),
                      get_skills(json_vehicle),
                      get_vehicle_time_window(json_vehicle));

      input.add_vehicle(vehicle);

      bool has_profile = json_vehicle.HasMember("profile");
      std::string current_profile =
        (has_profile) ? get_string(json_vehicle, "profile") : DEFAULT_PROFILE;

      if (common_profile.empty()) {
        // First iteration only.
        common_profile = current_profile;
      } else {
        // Check profile consistency.
        if (current_profile != common_profile) {
          throw Exception(ERROR::INPUT, "Mixed vehicle profiles in input.");
        }
      }
    }

    if (has_jobs) {
      // Add the jobs.
      for (rapidjson::SizeType i = 0; i < json_input["jobs"].Size(); ++i) {
        auto& json_job = json_input["jobs"][i];

        check_id(json_job, "job");
        check_location_index(json_job, "job", matrix_size);

        auto job_loc_index = json_job["location_index"].GetUint();

        // Only for retro-compatibility: when no pickup and delivery
        // keys are defined and (deprecated) amount key is present, it
        // should be interpreted as a delivery.
        bool need_amount_compat = json_job.HasMember("amount") and
                                  !json_job.HasMember("delivery") and
                                  !json_job.HasMember("pickup");

        Job job(json_job["id"].GetUint64(),
                json_job.HasMember("location")
                  ? Location(job_loc_index,
                             parse_coordinates(json_job, "location"))
                  : Location(job_loc_index),
                get_service(json_job),
                need_amount_compat
                  ? get_amount(json_job, "amount", amount_size)
                  : get_amount(json_job, "delivery", amount_size),
                get_amount(json_job, "pickup", amount_size),
                get_skills(json_job),
                get_priority(json_job),
                get_job_time_windows(json_job));

        input.add_job(job);
      }
    }

    if (has_shipments) {
      // Add the shipments.
      for (rapidjson::SizeType i = 0; i < json_input["shipments"].Size(); ++i) {
        auto& json_shipment = json_input["shipments"][i];

        check_shipment(json_shipment);

        // Retrieve common stuff for both pickup and delivery.
        auto amount = get_amount(json_shipment, "amount", amount_size);
        auto skills = get_skills(json_shipment);
        auto priority = get_priority(json_shipment);

        // Defining pickup job.
        auto& json_pickup = json_shipment["pickup"];

        check_id(json_pickup, "pickup");
        check_location_index(json_pickup, "pickup", matrix_size);

        auto pickup_loc_index = json_pickup["location_index"].GetUint();

        Job pickup(json_pickup["id"].GetUint64(),
                   JOB_TYPE::PICKUP,
                   json_pickup.HasMember("location")
                     ? Location(pickup_loc_index,
                                parse_coordinates(json_pickup, "location"))
                     : Location(pickup_loc_index),
                   get_service(json_pickup),
                   amount,
                   skills,
                   priority,
                   get_job_time_windows(json_pickup));

        // Defining delivery job.
        auto& json_delivery = json_shipment["delivery"];

        check_id(json_delivery, "delivery");
        check_location_index(json_delivery, "delivery", matrix_size);

        auto delivery_loc_index = json_delivery["location_index"].GetUint();

        Job delivery(json_delivery["id"].GetUint64(),
                     JOB_TYPE::DELIVERY,
                     json_delivery.HasMember("location")
                       ? Location(delivery_loc_index,
                                  parse_coordinates(json_delivery, "location"))
                       : Location(delivery_loc_index),
                     get_service(json_delivery),
                     amount,
                     skills,
                     priority,
                     get_job_time_windows(json_delivery));

        input.add_shipment(pickup, delivery);
      }
    }
  } else {
    // Adding vehicles and jobs only, matrix will be computed using
    // OSRM upon solving.

    // All vehicles.
    for (rapidjson::SizeType i = 0; i < json_input["vehicles"].Size(); ++i) {
      auto& json_vehicle = json_input["vehicles"][i];
      check_id(json_vehicle, "vehicle");

      // Start def is a ugly workaround as using plain:
      //
      // boost::optional<Location> start;
      //
      // will raise a false positive -Wmaybe-uninitialized with gcc,
      // see https://gcc.gnu.org/bugzilla/show_bug.cgi?id=47679

      auto start([]() -> boost::optional<Location> { return boost::none; }());
      if (json_vehicle.HasMember("start")) {
        start =
          boost::optional<Location>(parse_coordinates(json_vehicle, "start"));
      }

      boost::optional<Location> end;
      if (json_vehicle.HasMember("end")) {
        end = boost::optional<Location>(parse_coordinates(json_vehicle, "end"));
      }

      Vehicle vehicle(json_vehicle["id"].GetUint64(),
                      start,
                      end,
                      get_amount(json_vehicle, "capacity", amount_size),
                      get_skills(json_vehicle),
                      get_vehicle_time_window(json_vehicle));

      input.add_vehicle(vehicle);

      bool has_profile = json_vehicle.HasMember("profile");
      std::string current_profile =
        (has_profile) ? get_string(json_vehicle, "profile") : DEFAULT_PROFILE;

      if (common_profile.empty()) {
        // First iteration only.
        common_profile = current_profile;
      } else {
        // Check profile consistency.
        if (current_profile != common_profile) {
          throw Exception(ERROR::INPUT, "Mixed vehicle profiles in input.");
        }
      }
    }

    if (has_jobs) {
      // Add the jobs.
      for (rapidjson::SizeType i = 0; i < json_input["jobs"].Size(); ++i) {
        auto& json_job = json_input["jobs"][i];

        check_id(json_job, "job");
        check_location(json_job, "job");

        // Only for retro-compatibility: when no pickup and delivery
        // keys are defined and (deprecated) amount key is present, it
        // should be interpreted as a delivery.
        bool need_amount_compat = json_job.HasMember("amount") and
                                  !json_job.HasMember("delivery") and
                                  !json_job.HasMember("pickup");

        Job job(json_job["id"].GetUint64(),
                parse_coordinates(json_job, "location"),
                get_service(json_job),
                need_amount_compat
                  ? get_amount(json_job, "amount", amount_size)
                  : get_amount(json_job, "delivery", amount_size),
                get_amount(json_job, "pickup", amount_size),
                get_skills(json_job),
                get_priority(json_job),
                get_job_time_windows(json_job));

        input.add_job(job);
      }
    }

    if (has_shipments) {
      // Add the shipments.
      for (rapidjson::SizeType i = 0; i < json_input["shipments"].Size(); ++i) {
        auto& json_shipment = json_input["shipments"][i];

        check_shipment(json_shipment);

        // Retrieve common stuff for both pickup and delivery.
        auto amount = get_amount(json_shipment, "amount", amount_size);
        auto skills = get_skills(json_shipment);
        auto priority = get_priority(json_shipment);

        // Defining pickup job.
        auto& json_pickup = json_shipment["pickup"];

        check_id(json_pickup, "pickup");
        check_location(json_pickup, "pickup");

        Job pickup(json_pickup["id"].GetUint64(),
                   JOB_TYPE::PICKUP,
                   parse_coordinates(json_pickup, "location"),
                   get_service(json_pickup),
                   amount,
                   skills,
                   priority,
                   get_job_time_windows(json_pickup));

        // Defining delivery job.
        auto& json_delivery = json_shipment["delivery"];

        check_id(json_delivery, "delivery");
        check_location(json_delivery, "delivery");

        Job delivery(json_delivery["id"].GetUint64(),
                     JOB_TYPE::DELIVERY,
                     parse_coordinates(json_delivery, "location"),
                     get_service(json_delivery),
                     amount,
                     skills,
                     priority,
                     get_job_time_windows(json_delivery));

        input.add_shipment(pickup, delivery);
      }
    }
  }

  // Set relevant routing wrapper.
  std::unique_ptr<routing::Wrapper<Cost>> routing_wrapper;
  switch (cl_args.router) {
  case ROUTER::OSRM: {
    // Use osrm-routed.
    auto search = cl_args.servers.find(common_profile);
    if (search == cl_args.servers.end()) {
      throw Exception(ERROR::INPUT, "Invalid profile: " + common_profile + ".");
    }
    routing_wrapper =
      std::make_unique<routing::OsrmRoutedWrapper>(common_profile,
                                                   search->second);
  } break;
  case ROUTER::LIBOSRM:
#if USE_LIBOSRM
    // Use libosrm.
    try {
//      routing_wrapper = homsick
//      std::make_unique<routing::LibosrmWrapper>(common_profile); homsick
    } catch (const osrm::exception& e) {
      throw Exception(ERROR::ROUTING,
                      "Invalid shared memory region: " + common_profile);
    }
#else
    // Attempt to use libosrm while compiling without it.
    throw Exception(ERROR::ROUTING,
                    "EfOVRP compiled without libosrm installed.");
#endif
    break;
  case ROUTER::ORS:
    // Use ORS http wrapper.
    auto search = cl_args.servers.find(common_profile);
    if (search == cl_args.servers.end()) {
      throw Exception(ERROR::INPUT, "Invalid profile: " + common_profile + ".");
    }
//    routing_wrapper = homsick
//      std::make_unique<routing::OrsWrapper>(common_profile, search->second); homsick
    break;
  }
  input.set_routing(std::move(routing_wrapper));

  return input;
}

} // namespace io
} // namespace EfOVRP
