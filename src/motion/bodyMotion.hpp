#pragma once
#include "hardware/joints.hpp"

/**
 * @brief A collection of free methods used to achieve different full-body
 * motions
 *
 * Each method expects a reference to the robot joint which are then used to
 * achieve the desired body motion.
 */
namespace BodyMotion
{

/**
 * @brief Set both arms to the same joint angle
 *
 * @param joints
 * @param angle
 */
void setBothArmsToAngle(Joints& joints, int angle);

/**
 * @brief Set all the robot joints to their respective zero position
 *
 * @param joints
 */
void allJointsToZero(Joints& joints);

/**
 * @brief Used to achieve a specific synchronised body motion. A single dance
 * motion is defined as:
 *
 * - Waist moves from 0     -> MaxAngle -> MinAngle -> 0
 * - Right Arm moves from 0 -> MaxAngle -> MinAngle -> 0
 * - Left Arm moves from 0  -> MinAngle -> MaxAngle -> 0
 *
 * @param joints
 * @param milliSeconds
 * @param armOffset
 */
void singleDanceMotion(Joints& joints, int milliSeconds, int armOffset = 0);

} // namespace BodyMotion