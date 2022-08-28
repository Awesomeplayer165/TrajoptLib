#pragma once

#include <casadi/casadi.hpp>

#include "CasADiHolonomicTrajectoryOptimizationProblem.h"
#include "HolonomicPath.h"
#include "SwerveDrivetrain.h"

namespace helixtrajectory {

    class CasADiSwerveTrajectoryOptimizationProblem : public CasADiHolonomicTrajectoryOptimizationProblem {
    private:
        /**
         * @brief the swerve drivetrain
         */
        const SwerveDrivetrain& swerveDrivetrain;

        const size_t moduleCount;

        /**
         * @brief the moduleCount x (controlIntervalTotal + 1) matrix of swerve module x-coordinates
         * relative to the robot nonrotating coordinate system; each row is a swerve module, each column
         * is a sample point
         */
        casadi::MX moduleX;
        /**
         * @brief the moduleCount x (controlIntervalTotal + 1) matrix of swerve module y-coordinates
         * relative to the robot nonrotating coordinate system; each row is a swerve module, each column
         * is a sample point
         */
        casadi::MX moduleY;
        /**
         * @brief the moduleCount x (controlIntervalTotal + 1) matrix of swerve module x-components
         * of velocity; each row is a swerve module, each column is a sample point
         */
        casadi::MX moduleVX;
        /**
         * @brief the moduleCount x (controlIntervalTotal + 1) matrix of swerve module y-components
         * of velocity; each row is a swerve module, each column is a sample point
         */
        casadi::MX moduleVY;
        /**
         * @brief the moduleCount x controlIntervalTotal matrix of swerve module y-components
         * of force; each row is a swerve module, each column is a control interval
         */
        casadi::MX moduleFX;
        /**
         * @brief the moduleCount x controlIntervalTotal matrix of swerve module y-components
         * of force; each row is a swerve module, each column is a control interval
         */
        casadi::MX moduleFY;
        /**
         * @brief the moduleCount x controlIntervalTotal matrix of swerve module torques applied
         * about the axis of rotation; each row is a swerve module, each column is a control interval
         */
        casadi::MX moduleTau;

        /**
         * @brief the 1 x controlIntervalTotal vector of the x-component of net force applied to the robot.
         */
        casadi::MX netFX;
        /**
         * @brief the 1 x controlIntervalTotal vector of the y-component of net force applied to the robot.
         */
        casadi::MX netFY;
        /**
         * @brief the 1 x controlIntervalTotal vector of the net torque applied to the robot.
         */
        casadi::MX netTau;

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
        static const casadi::MX SolveModulePosition(const casadi::MX& theta, const SwerveModule& module);

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
         * @param swerveDrivetrain the swerve drivetrain
         */
        static void ApplyDynamicsConstraints(casadi::Opti& opti,
                const casadi::MX& ax, const casadi::MX& ay, const casadi::MX& alpha,
                const casadi::MX& netFX, const casadi::MX& netFY, const casadi::MX& netTau,
                const SwerveDrivetrain& swerveDrivetrain);

        static void ApplyPowerConstraints(casadi::Opti& opti,
                const casadi::MX& moduleVX, const casadi::MX& moduleVY,
                const casadi::MX& moduleFX, const casadi::MX& moduleFY,
                const SwerveDrivetrain& SwerveDrivetrain);

    public:
        /**
         * @brief Construct a new CasADi Swerve Trajectory Optimization Problem
         * with a swerve drivetrain and holonomic path.
         * 
         * @param swerveDrivetrain the swerve drivetrain
         * @param holonomicPath the holonomic path
         */
        CasADiSwerveTrajectoryOptimizationProblem(const SwerveDrivetrain& swerveDrivetrain, const HolonomicPath& holonomicPath);
    };
}