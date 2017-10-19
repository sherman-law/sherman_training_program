/*
 *	UAV Task Scheduler 
 *	Version: 0.1.0
 *	A scheduler is the framework of the UAV program. After initialization the program will run a
 *	main loop which in turn starts this scheduler until exit condition met. 
 *	This main loop (also called outter loop) consists of two parts:
 *		A. an inner loop:  Also called attitude control loop. 
 *				All time critical tasks are executed at highest priority within this loop.
 *				Inner loop is mission critical thus will be guaranteed to run, or say, un-schedulable.
 *				While any other task which is left outside of the inner loop will be scheduled by 
 *				the scheduler, depending on time left available after inner loop.
 *				Currently inner loop is required to run at a frequency of 400Hz and is changable.
 *
 *		B. anything left outside inner loop: 
 *				The main loop itself is also called position control loop. 
 *				All low priority time insensitive tasks are executed outside the 
 *				inner loop. These tasks are regarded as schedulable and will be managed by the scheduler.
 *				Currently outter loop is required to run at a frequency of 100Hz.
 *				inner loop will be executed 4 times in one main loop procedure.
 *
 *	Author:	 Sherman Liu <420741957ming@gmail.com>
 *		 Lewis Liu <rhinoceromirinda@gmail.com>
 *
 */

#ifndef __SCHEDULER_H
#define __SCHEDULER_H

#include "common.h"	/* convenient debug macros used in all headers */

#define ARRAY_LEN(ARRAY) (sizeof(ARRAY)/sizeof(ARRAY[0]))
#define TASK_MAX_NUMBER	      20 

/* 
 * \brief  The Scheduler Class 
 * 
 * \note   A Scheduler is a time-slots assigner to all "tasks" who need CPU time to run. Scheduler 
 * 	   distributes time-slots in a round-robin fanshion with two prior conditions:
 *
 * 	  	A. There is an inner loop who is "first class citizen": CPU time must first be fully
 * 	  	assigned to this inner loop before it can be sliced into slots and given out to tasks.
 *
 * 	  	B. If the inner loop consumes too much time, and there is no enough time slots avail-
 * 	  	able to tasks, certain task will be sacrificed. In this condition, the sacrified task
 * 	  	have to wait until next round to run.
 *
 */
struct sched_t;

/* declare a scheduler object as global variable */
extern sched_t g_sched;

/**
 * \brief  The Task Class 
 * 
 * \note   Tasks are schedulable objects used to encapsulate a procedure being executed 
 *         periodically in outer loop (position control loop). They have two major attributes:
 *
 * 		A. execution frequency: for how long the task should be executed again.
 *
 *		B. max allowable execution time: in how much time the task should finish it's job.
 */
typedef struct _task
{
        const fp_task_func run;       	/* function pointer to task function */
	const uint8_t	 *_name;	/* task name */
	const uint16_t    _rate_hz;	/* run rate of task */
	const uint16_t    _max_time;    /* max allowable execution time in micros */
	
	uint32_t    _last_ticks;	/* last tick number when this task been scheduled */
}task_t; 

/*
 * \brief   Inner Loop Interface
 *
 * \param[in] thiz: this pointer
 * \param[in] ctx: execution context passed into inner loop, set to NULL when not used.
 * 		    user must explicitly convert there context pointer to void type before
 * 		    pass it through this function call and convert it back within this call.
 *
 * \note    Inner loop runs time critical tasks periodically such as update inertial data 
 * 	    and control the motors, which are vital to vehicle's attitude control and flight
 * 	    state update. Since all vital tasks are put into inner loop, they're not like 
 * 	    normal tasks in the way that they CAN NOT be scheduled. 
 * 	    Inner loop is also called attitude control loop.
 * 
 */
typedef void (* fp_innerloop) (sched_t *thiz, void* ctx);

/**
 * \brief  Inner Loop running count function pointer
 *
 * \param[in]  thiz:this pointer
 */
typedef void (* fp_count) (sched_t *thiz);


/**
 * \brief  timer callback, used to decide if it is time for inner loop to run
 *
 * \param[in] thiz: this pointer
 * \param[in] time_available: time left after inner loop. This is the time available for 
 * 		all tasks to run, under scheduling of the scheduler.
 *
 * \note    Inner loop runs time critical tasks periodically such as update inertial data 
 * 	    and control the motors, which are vital to vehicle's attitude control and flight
 * 	    state update. Since all vital tasks are put into inner loop, they're not like 
 * 	    normal tasks in the way that they CAN NOT be scheduled. 
 * 	    Inner loop is also called attitude control loop.
 * 
 */
void timer_tick_callback (sched_t *thiz);

/**
 * \brief  Default Scheduler Constructor 
 *
 * \param[in] thiz: this pointer
 * \param[in] task_num: task number in task array
 * \param[in] task_array: pointer to task_array
 *
 * \note   user can override this function to initialize their own inner loop
 * 	   and other scheduler properties.
 */
void default_init (sched_t *thiz, task_t *task_array, uint8_t task_num);

/**
 * \brief  Default Scheduler Destructor 
 *
 * \param[in] thiz: this pointer
 *
 */
extern void default_destroy (sched_t *thiz);		

/*
 * \brief  The Task Interface 
 *
 * \param[in] thiz: this pointer 
 * \param[in] ctx: task function's context 
 *
 * \note   User must define their own task function and pass a this pointer which
 * 	   points to the task object it binds to, if the task doesn't require any
 *	   execution context from caller's environment, ctx could be set NULL.
 */
typedef void (* fp_task_func)(task_t *thiz, void* ctx); 		

/*
 * \brief UAV Task Scheduler Platform Porting Interface
 *
 * \note  Users are responsible for implementing these interfaces in
 * 	  order to run the scheduler on different platforms.
 *
 * 	  CAUTION: THIS PROGRAM WON'T RUN IF NO IMPLEMENTATION OF THIS FUNCTION PROVIDED BY USER!
 * 
 */
typedef uint32_t (* fp_get_micros) (void);

/*
 * \brief The Main Loop of The framework
 *
 * \note  Also called outter loop. The main loop is the routines been 
 * 	  performed after system been booted and initialized.
 * 	  This function must be put into a while(1) loop and should never exit. 
 *
 */
extern void main_loop (sched_t *thiz);

/*
 * \brief set the method scheduler uses to get time values (in micros )
 *
 */
void set_get_micros(sched_t *thiz, fp_get_micros fp);

/**
 * \brief  timer callback, used to decide if it is time for inner loop to run
 *
 * \note   this function should be called from a 1ms timer's ISR
 *
 */
void timer_tick_callback (void);

/*
 * \brief get inner loop rate or period
 *
 */
inline uint16_t get_loop_rate_hz(sched_t *thiz) const {
	_RETURN_MINUS_IF_FAIL(thiz);
	return thiz->_loop_rate;
}

inline uint32_t get_loop_period_us(sched_t *thiz) const {
	_RETURN_MINUS_IF_FAIL(thiz);
	// TODO: better idea to avoid integer division?
	return 1000000UL / thiz->_loop_rate;
}

#endif //__SCHEDULER_H
