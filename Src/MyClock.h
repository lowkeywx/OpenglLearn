//
// Created by lowkey on 2022/11/9.
//

#ifndef OPENGLLEARNING_MYCLOCK_H
#define OPENGLLEARNING_MYCLOCK_H

#include <vector>
#include <array>
#include <cassert>
#include <functional>

namespace lk {
    namespace gl{
        // 基于opengl的，就不用context了，直接使用main中的shader和program
        class MyClock
        {
        public:
            enum WatchPointType
            {
                PointType_Quarter,
                PointType_Twelfth,
                PointType_Sixtieth,
                PointType_Max
            };
            enum WatchHandType
            {
                WatchHandType_Hour,
                WatchHandType_Minutes,
                WatchHandType_second,
                WatchHandType_Max
            };
            struct RgbColor
            {
                float R {0.0};
                float G {0.0};
                float B {0.0};
            };

            struct Point
            {
                Point operator*(float rate) {
                    Point p;
                    p.x = x * rate;
                    p.y = y * rate;
                    p.z = z * rate;
                    return p;
                }
                float x {0};
                float y {0};
                float z {0};
            };

            struct WatchPoint : public Point
            {
                void draw();
                WatchPoint() = default;
                explicit WatchPoint(const Point& p) : Point() {
                    x = p.x;
                    y = p.y;
                    z = p.z;
                }
                RgbColor color {0,0,0};
                WatchPointType type {PointType_Max};
                // 线段的宽度
                int size {2};
                // 直径的百分比
                float len {0.1};
            };

            struct WatchHand
            {
                void draw();
                int size {1};
                RgbColor color{0,0,1};
                WatchHandType type;
                Point from {0,0,0};
                Point to {0,0,0};
            };

            struct WatchCircle
            {
                RgbColor color {0,0,1};
                int size {1};
                Point p {0,0,0};
            };

        public:
            MyClock() = default;
            virtual ~MyClock() = default;

        public:
            void init(Point p);
            void release();
            void start();
            void stop();
            void draw();
            inline void setPointSize(WatchPointType type, int size) {
                forEachWatchPoint(type, [size](WatchPoint& p) {
                    p.size = size;
                });
            }
            inline void setPointLen(WatchPointType type, float len) {
                forEachWatchPoint(type, [len](WatchPoint& p) {
                    p.len = len;
                });
            }
            inline void setPointColor(WatchPointType type, const RgbColor& c) {
                forEachWatchPoint(type, [c](WatchPoint& p) {
                    p.color = c;
                });
            }
            inline void setWatchHandSize(WatchHandType type, int size) {
                m_watchHands[type].size = size;
            }
            inline void setWatchHandColor(WatchHandType type, const RgbColor& c) {
                m_watchHands[type].color = c;
            }
            inline void setPosition(Point p) {
                m_centrePoint.x = p.x;
                m_centrePoint.y = p.y;
                m_centrePoint.z = p.z;
            }
            inline void setSize(float s) {
                m_size = s;
            }
        private:
            inline static int watchPointTypeToStep(WatchPointType type) {
                switch (type) {
                    case PointType_Quarter:
                        return kPointTypeQuarterStep;
                    case PointType_Twelfth:
                        return kPointTypeTwelfthStep;
                    case PointType_Sixtieth:
                        return 1;
                    case PointType_Max:
                        assert(0);
                        break;
                }
            }

            inline static bool isQuarterPoint(int index) {
                return index % kPointTypeQuarterStep == 0;
            }

            inline static bool isTwelfthPoint(int index) {
                return index % kPointTypeTwelfthStep == 0;
            }

            void forEachWatchPoint(WatchPointType type, const std::function<void(WatchPoint& p)>& f);

        private:
            static float pi;
            static float du;
            static constexpr int kMaxPointCount {60};
            static constexpr int kPointTypeQuarterStep {15};
            static constexpr int kPointTypeTwelfthStep {5};
            static constexpr int kCentPointSize{20};
            static constexpr RgbColor kCentPointColor {0.0,0.0,0.0};
            static constexpr int kHSize {8};
            static constexpr int kMSize {4};
            static constexpr int kSSize {2};
            static constexpr int kQuarterSize {8};
            static constexpr int KTwelfthSize {6};
            static constexpr int kSixtiethSize{2};
            std::array<WatchPoint, kMaxPointCount> m_watchPoint;
            std::array<WatchHand, WatchHandType_Max> m_watchHands;

            WatchPoint m_centrePoint;
            // 大小
            float m_size {1};
        };
    }
}

class MyClock {

};


#endif //OPENGLLEARNING_MYCLOCK_H
