#pragma once

#ifndef MYSLAM_FRAME_H
#define MYSLAM_FRAME_H

#include "./feature.h"
#include "./camera.h"
#include "./common_include.h"

namespace myslam {

    struct Frame {
        public:
            EIGEN_MAKE_ALIGNED_OPERATOR_NEW;
            typedef std::shared_ptr<Frame> Ptr;

            unsigned long id_ = 0;              // id of this frame
            unsigned long keyframe_id_ = 0;      // id of keyframe
            bool is_keyframe_ = false;          // keyframe?
            double time_stamp_;                  // timestamp, not used in Kitti
            SE3 pose_;                          // pose defined as Tcw
            std::mutex pose_mutex_;              // Pose data mutex
            cv::Mat left_img_, right_img_;      // stereo images

            // extracted features in left image
            std::vector<std::shared_ptr<Feature>> features_left_;
            // corresponding features in right image, set to nullptr if no corresponding
            std::vector<std::shared_ptr<Feature>> features_right_;

        public:     // data members
            Frame() {}

            Frame(long id, double time_stamp, const SE3 &pose, const Mat &left, const Mat &right);

            // set and get pose, thread safe
            SE3 Pose() {
                std::unique_lock<std::mutex> lck(pose_mutex_);
                return pose_;
            }

            void SetPose(const SE3 &pose) {
                std::unique_lock<std::mutex> lck(pose_mutex_);
                pose_ = pose;
            }

            // Set keyframe and keyframe_id
            void SetKeyFrame();

            // Create new frame and allocate id
            static std::shared_ptr<Frame> CreateFrame();

    };
}


#endif