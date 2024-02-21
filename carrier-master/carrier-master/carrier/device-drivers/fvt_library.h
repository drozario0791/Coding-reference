/******************************************************************************
 *
 *        Name: fvt_library.h
 *
 * Description: A collection of enums, defines, etc. for all FVT
 *              library modules.
 *
 *      Author: Cyrus
 *        Date: Thursday, 20 June 2019
 *
 ******************************************************************************
 */

#ifndef FVT_LIBRARY_H_
#define FVT_LIBRARY_H_


/******************************************************************************
 *
 *        Name: Set_Reset()
 *
 * Description: This is a basic flip flop function Typical use would
 *              be to assign the result to the input someBoolean =
 *              Set_Reset(someBoolean, set, reset)
 *
 *              The result of someBoolean remains unchanged unless:
 *              SET is true - someBoolean becomes true.  RESET is true
 *              - someBoolean becomes false.  SET and RESET both true
 *              - someBoolean becomes true (set takes priority)
 *
 *
 *      Author: Cyrus
 *        Date: Friday, 05 July 2019
 *
 ******************************************************************************
 */
static inline bool_t set_reset(bool_t	input,
							   bool_t	set,
							   bool_t	reset)
{
	bool_t	result	= input;

	if (set)
	{
		result	= TRUE;
	}
	else if (reset)
	{
		result	= FALSE;
	}
	else
	{
		// do nothing
	}

	return (result);
}

/*
 * The functionality of this function is the same as Set_Reset
 * EXCEPT when SET and RESET are both true, the result will be false (reset takes priority)
 */
static inline bool_t reset_set(bool_t	input,
							   bool_t	set,
							   bool_t	reset)
{
	bool_t	result	= input;

	if (reset)
	{
		result	= FALSE;
	}
	else if (set)
	{
		result	= TRUE;
	}
	else
	{
		// do nothing
	}

	return (result);
}

/******************************************************************************
 *
 *        Name: hysteresis()
 *
 * Description:This function created hysteresis for an input variable
 *              inputBool: the input boolean - the previous value. The
 *              result of the function must be assigned to the same
 *              variable as inputBool inputVal: the value used to
 *              determine what the output should be (ie temperature)
 *              upper and lower: limits for hysteresis.  If input goes
 *              above upper, it will return true until it falls below
 *              lower Typical use would be fanOn = hysteresis(fanOn,
 *              systemTemperature, 80, 50);
 *
 *
 *      Author: Cyrus
 *        Date: Friday, 05 July 2019
 *
 ******************************************************************************
 */
static inline bool_t hysteresis(bool_t	input_bool,
								int32_t	input_val,
								int32_t	upper,
								int32_t	lower)
{
	bool_t	result	= input_bool;

	if (input_val < lower)
	{
		result	= FALSE;
	}
	else if (input_val >= upper)
	{
		result	= TRUE;
	}
	else
	{
		// do nothing
	}

	return (result);
}

/******************************************************************************
 *
 *        Name: Misc Functions
 *
 * Description: Functions that are used very frequently to find the
 *              max/min between two or three variables.
 *
 *      Author: Cyrus
 *        Date: Friday, 05 July 2019
 *
 ******************************************************************************
 */
static inline bool_t return_value_at_bit_position(uint32_t num, uint8_t position)
{

    //
    // return the value at the bit position specified in the argument.
    //
    return (num & (1 << position)) != 0;

}


static inline uint16_t max_of_two_uint16_t_values(uint16_t a, uint16_t b)
{

    //
    // return the max unsigned number
    //
    return a > b ? a : b;

}


static inline int16_t max_of_two_int16_t_values(int16_t a, int16_t b)
{

    //
    // return the max signed number
    //
    return a > b ? a : b;

}


static inline uint16_t min_of_two_uint16_t_values(uint16_t a, uint16_t b)
{

    //
    // return the min unsigned number
    //
    return a < b ? a : b;

}


static inline int16_t min_of_two_int16_t_values(int16_t a, int16_t b)
{

    //
    // return the min signed number
    //
    return a < b ? a : b;

}


static inline uint16_t max_of_three_uint16_t_values(uint16_t a, uint16_t b, uint16_t c)
{

    //
    // return the max unsigned number
    //
    return a > b ? (a > c ? a : c) : (b > c ? b : c);

}


static inline int16_t max_of_three_int16_t_values(int16_t a, int16_t b, int16_t c)
{

    //
    // return the max signed number
    //
    return a > b ? (a > c ? a : c) : (b > c ? b : c);

}


static inline uint16_t min_of_three_uint16_t_values(uint16_t a, uint16_t b, uint16_t c)
{

    //
    // return the min unsigned number
    //
    return a < b ? (a < c ? a : c) : (b < c ? b : c);

}


static inline int16_t min_of_three_int16_t_values(int16_t a, int16_t b, int16_t c)
{

    //
    // return the min signed number
    //
    return a < b ? (a < c ? a : c) : (b < c ? b : c);

}


static inline int8_t min_of_three_int8_t_values(int8_t a, int8_t b, int8_t c)
{

    //
    // return the min signed number
    //
    return a < b ? (a < c ? a : c) : (b < c ? b : c);

}

static inline int8_t max_of_three_int8_t_values(int8_t a, int8_t b, int8_t c)
{

    //
    // return the max signed number
    //
    return a > b ? (a > c ? a : c) : (b > c ? b : c);

}


#endif // FVT_LIBRARY_H_
