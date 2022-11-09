//
// Created by lowkey on 2022/11/9.
//

#include "MyClock.h"

#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include <chrono>
#include <random>
#include <cmath>

namespace lk {
    namespace gl {

        float MyClock::pi = static_cast<float >(std::acos(-1));
        float MyClock::du = static_cast<float >(2.0 * pi / 360.0);

        void MyClock::forEachWatchPoint(MyClock::WatchPointType type, const std::function<void(WatchPoint &)> &f) {
            auto step = watchPointTypeToStep(type);
            for (int i = 0; i < kMaxPointCount; i+=step) {
                if (type == WatchPointType::PointType_Sixtieth && (isQuarterPoint(i) || isTwelfthPoint(i))) {
                    continue;
                }
                f(m_watchPoint[i]);
            }
        }

        void MyClock::draw() {
            //直径1的，中心(0，0)的圆,带花边, 因为求出来的线段很多
            glColor3f(0.0,0.0,0.0);
            glLineWidth(10);
            // GL_LINE_LOOP 首尾相连，GL_LINE_STRIP最后一个点和第一个点会出现空隙
            glBegin(GL_LINE_LOOP);
            //用一个字节减少60次 i/6
            for (const auto & i : m_watchPoint) {
                glVertex2f(i.x, i.y);
            }
            glEnd();
            // 画中心点
            m_centrePoint.draw();
            // 画时间点
            for (auto& p : m_watchPoint) {
                p.draw();
            }
            auto timeT = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
            auto localTime = std::localtime(&timeT);
            m_watchHands[WatchHandType::WatchHandType_second].to = m_watchPoint[localTime->tm_sec % 60] * 0.85;
            m_watchHands[WatchHandType::WatchHandType_Minutes].to = m_watchPoint[localTime->tm_min % 60] * 0.7;
            m_watchHands[WatchHandType::WatchHandType_Hour].to = m_watchPoint[(localTime->tm_hour % 24 - 12) * 5] * 0.6;
            // 画表针
            for (auto& h : m_watchHands) {
                h.draw();
            }
        }

        void MyClock::start() {
            uint8_t index = 0;
            for (int i = 0; i < 360; i+=6) {
                //点越多越平滑360 / 10还不够平滑
                float x = std::sin(du * i) + m_centrePoint.x;
                float y = std::cos(du * i) + m_centrePoint.y;
                m_watchPoint[index].x = x;
                m_watchPoint[index].y = y;
                index++;
            }
        }

        void MyClock::init(MyClock::Point p) {
            m_centrePoint.x = p.x;
            m_centrePoint.y = p.y;
            m_centrePoint.z = p.z;
            m_centrePoint.color = kCentPointColor;
            m_centrePoint.size = kCentPointSize;

            this->setPointSize(WatchPointType::PointType_Sixtieth, kSixtiethSize);
            this->setPointSize(WatchPointType::PointType_Twelfth, KTwelfthSize);
            this->setPointSize(WatchPointType::PointType_Quarter, kQuarterSize);

            this->setPointLen(WatchPointType::PointType_Sixtieth, 0.05);
            this->setPointLen(WatchPointType::PointType_Twelfth, 0.07);
            this->setPointLen(WatchPointType::PointType_Quarter, 0.10);


            this->setPointColor(WatchPointType::PointType_Sixtieth, kCentPointColor);
            this->setPointColor(WatchPointType::PointType_Twelfth, kCentPointColor);
            this->setPointColor(WatchPointType::PointType_Quarter, kCentPointColor);

            this->setWatchHandColor(WatchHandType::WatchHandType_Hour, kCentPointColor);
            this->setWatchHandColor(WatchHandType::WatchHandType_Minutes, kCentPointColor);
            this->setWatchHandColor(WatchHandType::WatchHandType_second, kCentPointColor);

            this->setWatchHandSize(WatchHandType::WatchHandType_Hour, kHSize);
            this->setWatchHandSize(WatchHandType::WatchHandType_Minutes, kMSize);
            this->setWatchHandSize(WatchHandType::WatchHandType_second, kSSize);
        }

        void MyClock::release() {

        }

        void MyClock::stop() {

        }

        void MyClock::WatchPoint::draw() {
            glColor3f(color.R, color.G, color.B);
            glLineWidth(size);
            glBegin(GL_LINES);
            glVertex3f(x, y, z);
            glVertex3f(x * (1 - len), y * (1 - len), z * (1 - len));
            glEnd();
        }

        void MyClock::WatchHand::draw() {
            glColor3f(color.R, color.G, color.B);
            glLineWidth(size);
            glBegin(GL_LINES);
            glVertex3f(from.x, from.y, from.z);
            glVertex3f(to.x, to.y, to.z);
            glEnd();
        }
    }
}
