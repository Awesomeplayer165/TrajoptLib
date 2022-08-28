#pragma once

#include <vector>

#include <casadi/casadi.hpp>

#include "HolonomicDrivetrain.h"
#include "Obstacle.h"
#include "SwerveModule.h"

namespace helixtrajectory {

    /**
     * @brief This class represents a swerve drivetrain robot. It includes the physical properties necessary to
     * accurately model the dynamics of the system. An arbitrary number of swerve modules can be specified,
     * but typically it will be four. The order the swerve modules are listed does not matter.
     */
    class SwerveDrivetrain : public HolonomicDrivetrain {
    private:
        /**
         * @brief Gives an expression for the position of a swerve module relative
         * to the nonrotating robot coordinate system, given the robot's heading.
         * The first row contains the x-coordinate, and the second row
         * contains the y-coordinate.
         * 
         * @param theta the instantaneous heading of the robot
         * @param module the swerve module to find the position for
         * @return a 2 x 1 vector of positions where each row is a coordinate
         */
        const casadi::MX SolveModulePosition(const casadi::MX& theta, const SwerveModule& module) const;
    public:
        /**
         * @brief the list of swerve modules that make the robot move, usually one in each corner
         */
        std::vector<SwerveModule> modules;

        /**
         * @brief Construct a new SwerveDrivetrain with the robot's mass, moment of inertia, swerve modules, and bumpers.
         * 
         * @param mass the mass of the entire robot
         * @param momentOfInertia the moment of inertia of the robot about the center of rotation, which 
         * @param modules the list of modules the make up this swerve drivetrain
         * @param bumpers the bumpers of the robot represented as an obstacle
         */
        SwerveDrivetrain(double mass, double momentOfInertia, const std::vector<SwerveModule>& modules, const Obstacle& bumpers);

        /**
         * @brief Applies the drivetrain-specific constraints to the optimizer. These constraints
         * prevent motors from spinning too fast or with too much power. For swerve, this applies
         * constraints that connect the speed and direction of each swerve module wheel to the overall
         * kinematics and dynamics of the system. There are two parts: the velocity of each wheel is
         * connected to the velocity of the robot, and the force generated by each wheel is connected
         * to the acceleration of the robot. For both of these, limits are placed on the speed and torque
         * of each wheel. This allows the optimizer to generate an efficient, smooth path that the robot
         * can follow.
         * 
         * @param opti the current optimizer upon which constraints will be applied
         * @param theta (controlIntervalTotal + 1) x 1 column vector of the robot's heading for each sample point
         * @param vx (controlIntervalTotal + 1) x 1 column vector of the x-coordinate of the robot's velocity for each sample point
         * @param vy (controlIntervalTotal + 1) x 1 column vector of the y-coordinate of the robot's velocity for each sample point
         * @param omega (controlIntervalTotal + 1) x 1 column vector of the robot's angular velocity for each sample point
         * @param ax controlIntervalTotal x 1 column vector of the x-coordinate of the robot's acceleration for each sample
         *           point
         * @param ay controlIntervalTotal x 1 column vector of the y-coordinate of the robot's acceleration for each sample
         *           point
         * @param alpha controlIntervalTotal x 1 column vector of the robot's angular velocity for each sample point
         * @param controlIntervalTotal the number of segments in this trajectory (number of sample points - 1)
         */
        virtual void ApplyDynamicsConstraints(casadi::Opti& opti,
                const casadi::MX& theta, const casadi::MX& vx, const casadi::MX& vy,
                const casadi::MX& omega, const casadi::MX& ax, const casadi::MX& ay,
                const casadi::MX& alpha, size_t controlIntervalTotal) const;

        friend std::ostream& operator<<(std::ostream& stream, const SwerveDrivetrain& swerveDrivetrain);
    };
}