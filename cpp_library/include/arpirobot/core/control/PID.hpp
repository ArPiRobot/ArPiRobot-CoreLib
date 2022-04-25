/*
 * Copyright 2022 Marcus Behel
 *
 * This file is part of ArPiRobot-CoreLib.
 * 
 * ArPiRobot-CoreLib is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * ArPiRobot-CoreLib is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public License
 * along with ArPiRobot-CoreLib.  If not, see <https://www.gnu.org/licenses/>. 
 */

#pragma once

namespace arpirobot{

    /**
     * \class PID PID.hpp arpirobot/core/action/PID.hpp
     * 
     * Implementation of a simple PID controller with feedforward gain support.
     */
    class PID{
    public:

        /**
         * Construct a new PID object
         * 
         * @param kp Proportional gain
         * @param ki Integral gain
         * @param kd Derivative gain
         * @param kf Feedforward gain (often not used, but useful for some systems)
         * @param min Minimum value to cap PID output at
         * @param max Maximum value to cap PID output at
         */
        PID(double kp = 0, double ki = 0, double kd = 0,
            double kf = 0, double min = -1.0, double max = 1.0);

        /**
         * Get the proportional gain
         * 
         * @return proportional gain (kp) 
         */
        double getKp();

        /**
         * Set the proportional gain (kp)
         * 
         * @param kp New value of kp
         */
        void setKp(double kp);

        /**
         * Get the integral gain
         * 
         * @return integral gain (ki) 
         */
        double getKi();

        /**
         * Set the integral gain (ki)
         * 
         * @param ki New value of ki
         */
        void setKi(double ki);

        /**
         * Get the derivitive gain
         * 
         * @return derivative gain (kd)
         */
        double getKd();

        /**
         * Set the derivative gain
         * 
         * @param kd New value of kd
         */
        void setKd(double kd);

        /**
         * Get the feedforward gain
         * 
         * @return feedforward gain (kf) 
         */
        double getKf();

        /**
         * Set the feedforward gain
         * 
         * @param kf New value of kf
         */
        void setKf(double kf);

        /**
         * Get the minimum allowed PID output
         * 
         * @return min allowed PID output
         */
        double getMin();

        /**
         * Set the minimum allowed PID output
         * 
         * @param min New min allowed PID output
         */
        void setMin(double min);

        /**
         * Get the maximum allowed PID output
         * 
         * @return max allowed PID output
         */
        double getMax();

        /**
         * Set the maximum allowed PID output
         * 
         * @param max New max allowed PID output
         */
        void setMax(double max);


        /**
         * Get the controller's current setpoint
         * 
         * @return Current setpoint
         */
        double getSetpoint();

        /**
         * Set the controller's setpoint
         * 
         * @param setpoint New setpoint for the controller
         */
        void setSetpoint(double setpoint);

        /**
         * Reset the PID controller's internal state.
         * This resets the accumulated integral value and the previous error
         * used by the derivative gain.
         */
        void reset();

        /**
         * Calculate the current output of the PID controller.
         * 
         * @param currentPv Current value of the process variable.
         * @return The PID controller's output, capped at the set min and max values 
         */
        double getOutput(double currentPv);


    private:
        double kp, ki, kd, kf;          // Gains (P, I, D, and feedforward)
        double min, max;                // Values to cap PID output at
        double setpoint = 0;            // Current setpoint

        double integral = 0;            // Current accumulated (integral) value
        double lastError = 0;           // Previous error used in derivative value calculation
    };

}
