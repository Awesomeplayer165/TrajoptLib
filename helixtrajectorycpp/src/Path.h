#pragma once

#include <vector>

namespace helixtrajectory {

    /**
     * @brief An initial guess of a possible state the robot may be in during the trajectory.
     */
    struct InitialGuessPoint {
        /**
         * @brief the initial guess of the x-coordinate of the robot
         */
        double x;
        /**
         * @brief the initial guess of the y-coordinate of the robot
         */
        double y;
        /**
         * @brief the initial guess of the heading of the robot
         */
        double heading;
    };

    /**
     * @brief A certain state that the robot must have during some instance of the trajectory.
     * Includes options to constrain dynamics like position and velocity during that instance.
     */
    class Waypoint {
    public:
        /**
         * @brief x-coordinate of robot at waypoint
         */
        double x;
        /**
         * @brief y-coordinate of robot at waypoint
         */
        double y;
        /**
         * @brief heading of robot at waypoint
         */
        double heading;
        /**
         * @brief whether or not the optimizer should constrain the x-coordinate of the robot at waypoint
         */
        bool xConstrained;
        /**
         * @brief whether or not the optimizer should constrain the y-coordinate of the robot at waypoint
         */
        bool yConstrained;
        /**
         * @brief whether or not the optimizer should constrain the heading of the robot at waypoint
         */
        bool headingConstrained;
        /**
         * @brief the points used to construct the initial trajectory guess for the next trajectory segment
         */
        std::vector<InitialGuessPoint> initialGuessPoints;
        
        virtual ~Waypoint();

        virtual bool IsValid() const noexcept;
        bool IsPositionStateKnown() const noexcept;
        virtual bool IsVelocityStateKnown() const noexcept = 0;
        bool IsStateKnown() const noexcept;

    protected:
        Waypoint(double x, double y, double heading, bool xConstrained, bool yConstrained, bool headingConstrained, const std::vector<InitialGuessPoint>& initialGuessPoints);
    };

    class Path {
    public:

        virtual ~Path();

        /**
         * @brief Gets the number of waypoints that make up this path.
         * 
         * @return the length of this path
         */
        inline virtual size_t Length() const noexcept = 0;
        /**
         * @brief Get the waypoint at the specified index.
         * 
         * @param index the index
         * @return a reference to the waypoint
         */
        inline virtual Waypoint& GetWaypoint(size_t index) = 0;
        /**
         * @brief Get the waypoint at the specified index.
         * 
         * @param index the index
         * @return a const reference to the waypoint
         */

        inline virtual const Waypoint& GetWaypoint(size_t index) const = 0;
        bool IsValid() const noexcept;
    };
}