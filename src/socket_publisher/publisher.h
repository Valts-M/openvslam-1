#ifndef SOCKET_PUBLISHER_PUBLISHER_H
#define SOCKET_PUBLISHER_PUBLISHER_H

#include "socket_publisher/data_serializer.h"
#include "socket_publisher/socket_client.h"

#include <mutex>
#include <memory>

namespace openvslam {

class config;
class system;

namespace publish {
class frame_publisher;
class map_publisher;
} // namespace publish

} // namespace openvslam

namespace socket_publisher {

class publisher {
public:
    publisher(const YAML::Node& yaml_node, const std::shared_ptr<openvslam::system>& system,
              const std::shared_ptr<openvslam::publish::frame_publisher>& frame_publisher,
              const std::shared_ptr<openvslam::publish::map_publisher>& map_publisher);

    void run();

    /**
     * Save settings for report generator
     */
    void create_report_config_file(const std::string &mapDir, const std::string &resultDir);

    /* thread controls */
    void request_pause();
    bool is_paused();
    void resume();
    void request_terminate();
    bool is_terminated();

private:
    const std::shared_ptr<openvslam::system> system_;
    const unsigned int emitting_interval_;
    const unsigned int image_quality_;

    std::unique_ptr<socket_client> client_;
    std::unique_ptr<data_serializer> data_serializer_;

    void callback(const std::string& message, const double value);

    /* thread controls */
    bool check_and_execute_pause();

    bool terminate_is_requested();
    void terminate();

    std::mutex mtx_terminate_;
    bool terminate_is_requested_ = false;
    bool is_terminated_ = true;

    std::mutex mtx_pause_;
    bool pause_is_requested_ = false;
    bool is_paused_ = true;

    double dist_to_ground_{0.5};
    double dist_to_ceiling_{1.0};
    double brush_width_{0.8};
};

} // namespace socket_publisher

#endif // SOCKET_PUBLISHER_PUBLISHER_H
