#ifndef MYSLAM_BACKEND_H
#define MYSLAM_BACKEND_H

#include "./common_include.h"
#include "./frame.h"
#include "./map.h"

namespace myslam {
    class Map;

    class Backend {
        public:
            EIGEN_MAKE_ALIGNED_OPERATOR_NEW;
            typedef std::shared_ptr<Backend> Ptr;

            // Start the backend thread in the constructor
            Backend();

            // Set cameras and fetch the params
            void SetCameras(Camera::Ptr left, Camera::Ptr right) {
                cam_left_ = left;
                cam_right_ = right;
            }

            void SetMap(std::shared_ptr<Map> map) { map_ = map; }

            // Optimize and update the map
            void UpdateMap();

            // stop the backend
            void Stop();

        private:
            // backend thread
            void BackendLoop();

            // optimize the sliding window
            void Optimize(Map::KeyframesType& keyframes, Map::LandmarksType& landmarks);

            std::shared_ptr<Map> map_;
            std::thread* backend_thread_;
            std::mutex data_mutex_;

            std::condition_variable map_update_;
            std::atomic<bool> backend_running_;

            Camera::Ptr cam_left_ = nullptr, cam_right_ = nullptr;
    };

}  // namespace myslam

#endif