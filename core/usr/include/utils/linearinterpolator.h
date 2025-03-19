#ifndef LINEARINTERPOLATOR_H
#define LINEARINTERPOLATOR_H

#include "configs.h"
#include "utils.h"

namespace zerr {

/**
 * @class LinearInterpolator
 * @brief Performs linear interpolation between two values over a specified number of steps
 * 
 * This class implements linear interpolation to smoothly transition between a start and end value
 * over a specified number of steps. It is useful for creating gradual parameter changes and
 * smooth transitions in audio processing.
 */
class LinearInterpolator {
  public:
    /**
     * @brief Set the start and end values and number of interpolation steps
     * @param start Starting value for interpolation
     * @param stop Ending value for interpolation  
     * @param len Number of interpolation steps
     * 
     * Initializes the interpolator with the given parameters. The interpolation will
     * move from start to stop value over len number of steps.
     */
    void set_value(Param start, Param stop, int len);

    /**
     * @brief Get the current interpolated value
     * @return Param The current interpolated value
     * 
     * Returns the interpolated value at the current position between start and stop values.
     */
    Param get_value();

    /**
     * @brief Advance to the next interpolation step
     * 
     * Increments the internal position counter and calculates the next interpolated value.
     * Should be called once per interpolation step.
     */
    void next_step();

  private:
    Param inter_val;   ///< Current interpolated value calculated based on position
    Param start_val;   ///< Starting value of interpolation range
    Param stop_val;    ///< Ending value of interpolation range

    int n_steps;       ///< Total number of interpolation steps to reach stop value
    int position;      ///< Current step position in the interpolation sequence
};

}  // namespace zerr

#endif  // LINEARINTERPOLATOR_H
